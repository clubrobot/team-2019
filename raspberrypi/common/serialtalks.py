#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import serial
from serial.serialutil import SerialException
import time
import random
import warnings
from common.CRC16 import *
from queue import Queue, Empty, LifoQueue
from threading import Thread, RLock, Event, current_thread
import re
from common.serialutils import Deserializer, IntegerType, FloatType, StringType

BAUDRATE = 115200

MASTER_BYTE = b'R'
SLAVE_BYTE = b'A'

PING_OPCODE = 0x00
GETUUID_OPCODE = 0x01
SETUUID_OPCODE = 0x02
DISCONNECT_OPCODE = 0x03
GETEEPROM_OPCODE = 0x04
SETEEPROM_OPCODE = 0x05
STDOUT_RETCODE = 0xFFFFFFFF
STDERR_RETCODE = 0xFFFFFFFE

WARNING_OPCODE = 0xFE

BYTEORDER = 'little'
ENCODING = 'utf-8'

CHAR = IntegerType(1, BYTEORDER, True)
UCHAR = IntegerType(1, BYTEORDER, False)
SHORT = IntegerType(2, BYTEORDER, True)
USHORT = IntegerType(2, BYTEORDER, False)
LONG = IntegerType(4, BYTEORDER, True)
ULONG = IntegerType(4, BYTEORDER, False)

FLOAT = FloatType('f')

STRING = StringType(ENCODING)

BYTE = UCHAR
INT = SHORT
UINT = USHORT
DOUBLE = FLOAT

SERIALTALKS_CRC_SIZE = 2


# Exceptions

class AlreadyConnectedError(ConnectionError): pass


class ConnectionFailedError(ConnectionError): pass


class NotConnectedError(ConnectionError): pass


class MuteError(TimeoutError): pass


class SerialTalksWarning(UserWarning, ConnectionError): pass


# Main class

class SerialTalks:

    def __init__(self, port):
        # Serial things
        self.port = port
        self.is_connected = False

        # Threading things
        self.queues_dict = dict()
        self.queues_lock = RLock()
        self.history = list()
        self.history_lock = RLock()
        # Instructions
        self.instructions = dict()
        self.instructions[WARNING_OPCODE] = self.launch_warning_

    def __enter__(self):
        self.connect()
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.disconnect()

    def connect(self, timeout=5):
        if self.is_connected:
            raise AlreadyConnectedError('{} is already connected'.format(self.port))

        # Connect to the serial port
        try:
            self.stream = serial.Serial(self.port, baudrate=BAUDRATE, bytesize=serial.EIGHTBITS,
                                        parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE)
            self.stream.timeout = 1
        except SerialException as e:
            raise ConnectionFailedError(str(e)) from None

        # Create a listening thread that will wait for inputs
        self.listener = SerialListener(self)
        self.listener.start()

        # Wait until the Arduino is operational
        startingtime = time.monotonic()
        while not self.is_connected:
            try:
                output = self.execute(PING_OPCODE, timeout=0.1)
                self.is_connected = True
                self.reset_queues()
            except TimeoutError:
                if time.monotonic() - startingtime > timeout:
                    self.disconnect()
                    raise MuteError(
                        '\'{}\' is mute. It may not be an Arduino or it\'s sketch may not be correctly loaded.'.format(
                            self.stream.port)) from None
                else:
                    continue

            except NotConnectedError:
                self.is_connected = False

    def disconnect(self):
        try:
            self.send(DISCONNECT_OPCODE)
        except NotConnectedError:
            pass
        # Stop the listening thread
        if hasattr(self, 'listener') and self.listener.is_alive():
            self.listener.stop.set()
            if self.listener is not current_thread():
                self.listener.join()

        # Close the serial port
        if hasattr(self, 'stream') and self.stream.is_open:
            self.stream.close()

        # Unset the connected flag
        self.is_connected = False

    def bind(self, opcode, instruction):
        if not opcode in self.instructions:
            self.instructions[opcode] = instruction
        else:
            raise KeyError('opcode {} is already bound to another instruction'.format(opcode))

    def rawsend(self, rawbytes):
        try:
            if hasattr(self, 'stream') and self.stream.is_open:
                sentbytes = self.stream.write(rawbytes)
                return sentbytes
        except SerialException:
            pass
        raise NotConnectedError('\'{}\' is not connected.'.format(self.port)) from None

    def send(self, opcode, *args):
        retcode = random.randint(0, 0xFFFFFFFF)
        content = BYTE(opcode) + ULONG(retcode) + bytes().join(args)
        # crc calculation
        crc = CRCprocessBuffer(content)
        prefix = MASTER_BYTE + BYTE(len(content)) + USHORT(crc)
        self.history_lock.acquire()
        self.history.append((crc, prefix+content))
        if len(self.history)>20:
            _  = self.history.pop(0)
        self.history_lock.release()
        self.rawsend(prefix + content)
        return retcode


    def get_queue(self, retcode):
        self.queues_lock.acquire()
        try:
            queue = self.queues_dict[retcode]
        except KeyError:
            queue = self.queues_dict[retcode] = Queue()
        finally:
            self.queues_lock.release()
        return queue

    def delete_queue(self, retcode):
        self.queues_lock.acquire()
        try:
            del self.queues_dict[retcode]
        finally:
            self.queues_lock.release()

    def reset_queues(self):
        self.queues_lock.acquire()
        self.queues_dict = dict()
        self.queues_lock.release()

    def process(self, message):
        retcode = message.read(ULONG)
        queue = self.get_queue(retcode)
        queue.put(message)

    def poll(self, retcode, timeout=0):
        queue = self.get_queue(retcode)
        block = (timeout is None or timeout > 0)
        try:
            output = queue.get(block, timeout)
        except Empty:
            if timeout is not None:
                raise TimeoutError('timeout exceeded') from None
            else:
                return None
        if queue.qsize() == 0:
            self.delete_queue(retcode)
        return output

    def flush(self, retcode):
        while self.poll(retcode) is not None:
            pass

    def execute(self, opcode, *args, timeout=5):
        retcode = self.send(opcode, *args)
        output = self.poll(retcode, timeout)
        return output

    def receive(self, input):
        opcode = input.read(BYTE)
        retcode = input.read(LONG)
        try:
            output = self.instructions[opcode](input)
            if output is None: return
            content = LONG(retcode) + output
            prefix = SLAVE_BYTE + BYTE(len(content))
            self.rawsend(prefix + content)
        except KeyError:
            pass

    def getuuid(self, timeout=5):
        output = self.execute(GETUUID_OPCODE, timeout=timeout)
        return output.read(STRING)

    def setuuid(self, uuid):
        return self.send(SETUUID_OPCODE, STRING(uuid))

    def getlog(self, retcode, timeout=0):
        log = str()
        while True:
            try:
                output = self.poll(retcode, 0)
                log += output.read(STRING)
            except TimeoutError:
                break
        if timeout > 0:
            try:
                output = self.poll(retcode, timeout)
                log += output.read(STRING)
            except TimeoutError:
                pass
        return log

    def save_eeprom(self, file=None, size=1024):
        binary_file = open(file, mode='w+b')
        for i in range(size):
            output = self.execute(GETEEPROM_OPCODE, INT(i))
            byte = output.read(BYTE)
            binary_file.write(bytes([byte]))
        binary_file.close()

    def load_eeprom(self, file=None):
        binary_file = open(file, mode='r+b')
        k = 0
        for byte in binary_file.read():
            self.send(SETEEPROM_OPCODE, INT(k), BYTE(byte))
            k += 1
        binary_file.close()

    def launch_warning_(self, message):
        warnings.warn("Message send corrupted !", SerialTalksWarning)
        crc_corrupted = message.read(USHORT)
        self.history_lock.acquire()
        for i in range(len(self.history)):
            if self.history[i][0] == crc_corrupted:
                print("Message resend !")
                self.rawsend(self.history[i][1])
                break
        self.history_lock.release()


    def getout(self, timeout=0):
        return self.getlog(STDOUT_RETCODE, timeout)

    def geterr(self, timeout=0):
        return self.getlog(STDERR_RETCODE, timeout)


class SerialListener(Thread):

    def __init__(self, parent):
        Thread.__init__(self)
        self.parent = parent
        self.stop = Event()
        self.daemon = True

    def run(self):
        state = 'waiting'  # ['waiting', 'starting', 'receiving']
        type_packet = SLAVE_BYTE
        buffer = bytes()
        crc_buf = bytes()
        msglen = 0
        while not self.stop.is_set():
            # Wait until new bytes arrive
            try:
                inc = self.parent.stream.read()
            except serial.serialutil.SerialException:
                self.parent.disconnect()
                break

            # Finite state machine
            if state == 'waiting' and inc in [SLAVE_BYTE, MASTER_BYTE]:
                type_packet = inc
                state = 'starting'
                continue

            elif state == 'starting' and inc:
                msglen = inc[0]
                state = 'crc'
                continue

            elif state == 'crc':
                crc_buf += inc
                if (len(crc_buf) >= SERIALTALKS_CRC_SIZE):
                    crc_val = (crc_buf[1] << 8) | crc_buf[0]
                    state = 'receiving'
                continue

            elif state == 'receiving':
                buffer += inc
                if (len(buffer) < msglen):
                    continue

            # Junk byte
            else:
                continue

            # Process the above message
            try:
                if (CRCcheck(buffer, crc_val)):
                    if type_packet == SLAVE_BYTE: self.parent.process(Deserializer(buffer))
                    if type_packet == MASTER_BYTE: self.parent.receive(Deserializer(buffer))
                else:
                    warnings.warn("Message receive corrupted !", SerialTalksWarning)
                    state = 'waiting'

            except NotConnectedError:
                self.parent.disconnect()
                break

            # Reset the finite state machine
            state = 'waiting'
            buffer = bytes()
            crc_buf = bytes()
