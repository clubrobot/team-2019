#!/usr/bin/python3
# -*- coding: utf-8 -*-

import socket
import pickle
import os, sys
import traceback
import warnings
import time
import random
import string
from queue import Queue, Empty
from threading import Thread, RLock, Event, current_thread

MASTER_BYTE = b'R'
SLAVE_BYTE = b'A'

AUTHENTIFICATION_OPCODE = 0xAA
ID_GETTER_OPCODE = 0xBB

ID_LENGTH = 6


# Exceptions

class ForeverAloneError(TimeoutError): pass


class AlreadyConnectedError(ConnectionError): pass


class ConnectionFailedError(ConnectionError): pass


class NotConnectedError(ConnectionError): pass


class AuthentificationError(Exception): pass


class NoSuchConnectionAvailable(Exception): pass


class NoIDGiven(RuntimeWarning): pass


# Main class

class TCPTalks:

    @staticmethod
    def _serversocket(port, timeout):
        # Create a server
        serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        serversocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        serversocket.bind(('', port))
        serversocket.listen(1)

        # Wait for the other to connect
        serversocket.settimeout(timeout)
        try:
            clientsocket = serversocket.accept()[0]
            return clientsocket
        except socket.timeout:
            raise ForeverAloneError('no connection request') from None
        finally:
            serversocket.close()  # The server is no longer needed

    @staticmethod
    def _clientsocket(ip, port, timeout):
        # Create a client
        clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        # Connect to the other
        startingtime = time.monotonic()
        while timeout is None or time.monotonic() - startingtime < timeout:
            try:
                clientsocket.connect((ip, port))
                return clientsocket
            except ConnectionRefusedError:
                continue
        raise ForeverAloneError('no server found') from None

    @staticmethod
    def _loads(rawbytes):
        a, b = 0, len(rawbytes)
        while (b - a > 1):
            i = (a + b) // 2
            try:
                output = pickle.loads(rawbytes[:i])
            except (EOFError, pickle.UnpicklingError, AttributeError):
                a = i
            else:
                b = i
        return pickle.loads(rawbytes[:b]), rawbytes[b:]

    _timeouterror = socket.timeout

    def __init__(self, ip=None, port=25565, id=None, password=None):
        # Instructions
        self.instructions = dict()

        # Socket things
        self.ip = ip
        self.port = port
        self.is_connected = False
        self.socket_lock = RLock()

        # Password
        self.password = password

        # Id
        self.id = id if not id is None else ''.join(
            random.choice(string.ascii_uppercase + string.digits) for _ in range(ID_LENGTH))
        self.bind(ID_GETTER_OPCODE, self.get_id)

        # Thread-safe inputs polling
        self.queues_dict = dict()
        self.queues_lock = RLock()

    def __enter__(self):
        self.connect()
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.disconnect()

    def connect(self, timeout=5):
        if self.is_connected:
            raise AlreadyConnectedError('already connected')

        # The remote controller is not able to execute custom instructions
        # as long as the following flag remains unset
        self.is_authentificated = False

        # This will let the Raspberry Pi try to reconnect itself after a
        # bad authentification
        while not self.is_connected:
            # Create a socket instance depending of what was given during the
            # instanciation
            if self.ip is None:  # Raspberry Pi
                self.socket = self._serversocket(self.port, timeout)
            else:  # Remote controller (Windows or Linux)
                self.socket = self._clientsocket(self.ip, self.port, timeout)
            self.socket.settimeout(1)

            # Create a listening thread that will wait for inputs
            self.listener = TCPListener(self)
            self.listener.start()

            # This flag is a read-only attribute indicating whether or not the
            # current instance is connected to the other
            self.is_connected = True

            # Password authentification
            if self.ip is None:  # Raspberry Pi
                if not self.wait_for_authentification(1):
                    self.disconnect()
            else:  # Remote controller
                if not self.authentificate(1):
                    self.disconnect()
                    raise AuthentificationError('authentification failed')
            time.sleep(1)

    def authentificate(self, timeout):
        self.sendback(AUTHENTIFICATION_OPCODE, self.password)

        self.is_authentificated = self.poll(AUTHENTIFICATION_OPCODE, timeout)

        return self.is_authentificated

    def wait_for_authentification(self, timeout):
        password = self.poll(AUTHENTIFICATION_OPCODE, timeout)
        self.is_authentificated = self.password in (None, password)
        self.sendback(AUTHENTIFICATION_OPCODE, self.is_authentificated)
        return self.is_authentificated

    def get_id(self):
        return self.id

    def disconnect(self):
        # Stop the listening thread
        if hasattr(self, 'listener') and self.listener.is_alive():
            self.listener.stop.set()
            if self.listener is not current_thread():
                self.listener.join()

        # Close the socket
        if hasattr(self, 'socket'):
            self.socket.close()
            del self.socket

        # Unset the connected flag
        self.is_connected = False

    def bind(self, opcode, instruction):
        if not opcode in self.instructions:
            self.instructions[opcode] = instruction
        else:
            raise KeyError('opcode {} is already bound to another instruction'.format(opcode))

    def rawsend(self, rawbytes):
        try:
            self.socket_lock.acquire()
            if hasattr(self, 'socket'):
                sentbytes = 0
                while (sentbytes < len(rawbytes)):
                    sentbytes += self.socket.send(rawbytes[sentbytes:])
                return sentbytes
        finally:
            self.socket_lock.release()
        raise NotConnectedError('not connected') from None

    def send(self, opcode, *args, **kwargs):
        retcode = random.randint(0, 0xFFFFFFFF)
        content = (opcode, retcode, args, kwargs)
        prefix = (MASTER_BYTE,)
        self.rawsend(pickle.dumps(prefix + content))
        return retcode

    def sendback(self, retcode, *args):
        content = (retcode, args)
        prefix = (SLAVE_BYTE,)
        self.rawsend(pickle.dumps(prefix + content))

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
        role = message[0]
        if (role == MASTER_BYTE):
            opcode, retcode, args, kwargs = message[1:]
            self.execinstruction(opcode, retcode, *args, **kwargs)
        elif (role == SLAVE_BYTE):
            retcode, args = message[1:]
            queue = self.get_queue(retcode)
            queue.put(args)

    def execinstruction(self, opcode, retcode, *args, **kwargs):
        try:
            # Make sure that the authentification was well performed
            if not self.is_authentificated:
                raise AuthentificationError('you are not authentificated')
                # AJOUT
                self.parent.disconnect()

            # Get the function or method associated with the received opcode
            try:
                instruction = self.instructions[opcode]
            except KeyError:
                raise KeyError('opcode {} is not bound to any instruction'.format(opcode)) from None

            # Execute the instruction
            output = instruction(*args, **kwargs)

        except Exception:
            etype, value, tb = sys.exc_info()
            output = (etype, value, traceback.extract_tb(tb))

        # Send back the output
        self.sendback(retcode, output)

    def poll(self, retcode, timeout=0):
        queue = self.get_queue(retcode)
        block = (timeout is None or timeout > 0)
        try:
            assert (current_thread() is not self.listener)
            output = queue.get(block, timeout)
        except Empty:
            if block:
                raise TimeoutError('timeout exceeded') from None
            else:
                return None
        if queue.qsize() == 0:
            self.delete_queue(retcode)
        if len(output) > 1:
            return output  # Return as a tuple
        else:
            return output[0]  # Return as a single variable

    def flush(self, retcode):
        while self.poll(retcode) is not None:
            pass

    def execute(self, opcode, *args, timeout=5, **kwargs):
        retcode = self.send(opcode, *args, **kwargs)
        output = self.poll(retcode, timeout=timeout)
        if isinstance(output, tuple) and isinstance(output[0], type) and issubclass(output[0], Exception):
            etype, value, tb = output
            output = ('{2}\n' +
                      '\nThe above exception was first raised by the distant TCPTalks instance:\n\n' +
                      'Distant traceback (most recent call last):\n' +
                      '{0}' +
                      '{1}: {2}''').format(''.join(traceback.format_list(tb)), etype.__name__, str(value))
            raise etype(output)
        else:
            return output

    def sleep_until_disconnected(self):
        if self.listener is not current_thread():
            self.listener.join()
        else:
            raise RuntimeError('cannot call the \'sleep_until_disconnected\' method from within the listening thread')


class TCPListener(Thread):

    def __init__(self, parent):
        Thread.__init__(self)
        self.parent = parent
        self.stop = Event()
        self.daemon = True

    def run(self):
        buffer = bytes()
        while not self.stop.is_set():

            # Wait until new bytes arrive

            try:
                inc = self.parent.socket.recv(256)
            except (ConnectionResetError, AttributeError):
                inc = None

            except self.parent._timeouterror:
                continue

            # Disconnect if the other is no longer connected
            if not inc:  # May be None or b''
                self.parent.disconnect()
                break

            buffer += inc
            try:
                while True:
                    # Process the above message
                    message, buffer = self.parent._loads(buffer)

                    # Try to decode the message using the pickle protocol
                    self.parent.process(message)

            except (EOFError, pickle.UnpicklingError, AttributeError):
                continue  # The message is not complete

            except NotConnectedError:
                self.parent.disconnect()
                break


class TCPTalksServer:
    class Client(TCPTalks):
        _timeouterror = socket.timeout

        def __init__(self, parent, socket, password, id=None):

            self.socket = socket
            self.socket.settimeout(1)
            self.password = password
            self.parent = parent
            self.id = id if not id is None else ''.join(
                random.choice(string.ascii_uppercase + string.digits) for _ in range(ID_LENGTH))

            self.is_authentificated = True
            self.queues_lock = RLock()
            self.queues_dict = dict()
            self.instructions = dict()
            self.socket_lock = RLock()
            self.listener = TCPListener(self)
            self.listener.start()

        def __del__(self):

            if self.listener.is_alive():
                self.listener.stop.set()
            if self.listener is not current_thread():
                self.listener.join()

            self.socket.close()

        def authentificate(self, timeout):
            password = self.poll(AUTHENTIFICATION_OPCODE, timeout)
            self.is_authentificated = self.password in (None, password)
            self.sendback(AUTHENTIFICATION_OPCODE, self.is_authentificated)
            return self.is_authentificated

        def get_id(self, timeout):

            try:
                self.id = self.execute(ID_GETTER_OPCODE, timeout=timeout)
            except AuthentificationError:
                return self.get_id(timeout)
            except TimeoutError:
                warnings.warn("No id receive from the new client, we will used this \' {} \' id ".format(self.id),
                              NoIDGiven)
            return self.id

        def disconnect(self):

            self.parent.disconnect(id=self.id)

        def execinstruction(self, opcode, retcode, *args, **kwargs):
            try:
                # Make sure that the authentification was well performed
                if not self.is_authentificated:
                    raise AuthentificationError('you are not authentificated')

                # Get the function or method associated with the received opcode
                try:
                    instruction = self.parent.instructions[opcode]
                except KeyError:
                    try:
                        instruction = self.instructions[opcode]
                    except KeyError:
                        raise KeyError('opcode {} is not bound to any instruction'.format(opcode)) from None

                # Execute the instruction
                output = instruction(*args, **kwargs)

            except Exception:
                etype, value, tb = sys.exc_info()
                output = (etype, value, traceback.extract_tb(tb))

            # Send back the output
            self.sendback(retcode, output)

    def __init__(self, port=25565, password=None, NbClients=4):
        # Socket things
        self.port = port
        self.password = password

        # Create a server
        self.serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.serversocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.serversocket.bind(('', self.port))
        self.serversocket.listen(1)

        # Common instructions
        self.instructions = dict()

        # Client
        self.NbClients = NbClients
        self.client = dict()
        self.disconnect_event = Event()
        self.disconnect_event.clear()

        # Lock
        self.connect_lock = RLock()

    def connect(self, timeout=5):
        # Look availiable slot
        if len(list(self.client.keys())) >= self.NbClients:
            raise NoSuchConnectionAvailable() from None

        # Lock the socket for other Thread
        self.connect_lock.acquire()

        self.serversocket.settimeout(timeout)
        # Wait for the other to connect
        try:
            clientsocket = self.serversocket.accept()[0]
        except socket.timeout:
            self.connect_lock.release()
            raise ForeverAloneError('no connection request') from None

        # Unlock the socket for other Thread
        self.connect_lock.release()

        # Generate new temp ID
        temp_name = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(ID_LENGTH))

        # Create new Client object
        new_client = TCPTalksServer.Client(self, clientsocket, self.password, temp_name)

        # Try to authentificate with the new instance
        try:
            if not new_client.authentificate(1):
                del new_client
                raise AuthentificationError('Bad password receive') from None

        except TimeoutError:
            del new_client
            raise AuthentificationError('No password receive') from None

        # Ask the client ID
        new_id = new_client.get_id(1)

        # Save Client to dictionary
        self.client[new_id] = new_client

        return new_id

    def remove_client(self, id):
        old_client = self.client.pop(id)
        del old_client
        self.disconnect_event.set()

    def disconnect(self, id=None):
        if id is None:
            while len(self.client) > 0:
                self.disconnect_event.set()
                self.client.pop(list(self.client.keys())[0]).__del__()
        else:
            try:
                self.disconnect_event.set()
                self.client.pop(id).__del__()
            except KeyError:
                pass

    def bind(self, opcode, instruction, id=None):
        if not id is None:
            if self.client.get(id, False):
                if not self.instructions.get(opcode) is None:
                    raise KeyError('opcode {} is already bound to another general instruction'.format(opcode))
                self.client[id].bind(opcode, instruction)
        else:
            if not self.bind_not_exist(opcode):
                raise KeyError('opcode {} is already bound to another instruction'.format(opcode))
            self.instructions[opcode] = instruction

    def bind_not_exist(self, opcode):
        for _, client in self.client.items():
            if not client.instructions.get(opcode) is None:
                return False
        # TO TEST
        if self.instructions.get(opcode) is None:
            return True
        return False

    def sleep_until_one_disconnected(self):
        self.disconnect_event.clear()
        self.disconnect_event.wait()
        self.disconnect_event.clear()

    def full(self):
        return len(self.client) >= self.NbClients

    def send(self, opcode, *args, id=None, broadcast=False, **kwargs):
        try:
            id = id if not id is None else list(self.client.keys())[0]
        except IndexError:
            raise NotConnectedError('No more client to talks')

        if broadcast:
            for id in self.client.keys():
                return self.client[id].send(opcode, *args, **kwargs)

        return self.client[id].send(opcode, *args, **kwargs)

    def execute(self, opcode, *args, id=None, **kwargs):
        try:
            id = id if not id is None else list(self.client.keys())[0]
        except IndexError:
            raise NotConnectedError('No more client to talks')

        return self.client[id].execute(opcode, *args, **kwargs)

    def poll(self, opcode, *args, id=None, **kwargs):
        try:
            id = id if not id is None else list(self.client.keys())[0]
        except IndexError:
            raise NotConnectedError('No more client to talks')

        self.client[id].send(opcode, *args, **kwargs)
