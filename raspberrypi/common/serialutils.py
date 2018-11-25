#!/usr/bin/python3
#-*- coding: utf-8 -*-
"""
This library is free software from Club robot Insa Rennes sources; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.
"""
import struct
from threading import RLock
import warnings

class SerialBufferTimeout(UserWarning, TimeoutError): pass


class SerialBuffer:
    def __init__(self, send_method, extern_buffer_size):
        self.stack_bytes = bytearray()
        self.rawsend = send_method
        self.buffer_size = extern_buffer_size
        self.bytes_sended = 0
        self.lock = RLock()

    def send(self, bytes, timeout=1):
        if not self.lock.acquire(blocking=True, timeout=timeout):
            warnings.warn("Bytes : {} can't be sended, timeout !".format(bytes), SerialBufferTimeout)
            return 0
        if self.bytes_sended >= self.buffer_size:
            self.stack_bytes += bytes
            res = 0
        else:
            to_send, to_store = bytearray(bytes)[:min(self.buffer_size-self.bytes_sended,len(bytes))], bytearray(bytes)[min(self.buffer_size-self.bytes_sended,len(bytes)):]
            self.stack_bytes+=to_store
            self.bytes_sended += len(to_send)
            res = self.rawsend(to_send)
        self.lock.release()
        return res

    def direct_send(self, bytes, timeout=1):
        if not self.lock.acquire(blocking=True, timeout=timeout):
            warnings.warn("Bytes : {} can't be sended, timeout !".format(bytes), SerialBufferTimeout)
            raise TimeoutError()
            return 0
        res = self.rawsend(bytes)
        self.lock.release()

    def reset(self, *args):
        self.lock.acquire(blocking=True)
        self.bytes_sended = 0
        bytes_to_send = bytes(self.stack_bytes)
        self.stack_bytes = bytearray()
        self.send(bytes_to_send, timeout=-1)
        self.lock.release()



class AbstractType:

    def __call__(self, value):
        return self.to_bytes(value)


class IntegerType(AbstractType):

    def __init__(self, length, byteorder, signed):
        self.length    = length
        self.byteorder = byteorder
        self.signed    = signed

    def to_bytes(self, integer):
        return int.to_bytes(integer, length = self.length, byteorder = self.byteorder, signed = self.signed)

    def from_bytes(self, rawbytes):
        return int.from_bytes(rawbytes[:self.length], byteorder = self.byteorder, signed = self.signed)


class FloatType(AbstractType):

    def __init__(self, standard):
        self.standard = standard # May be 'f' (float) or 'd' (double)

    def to_bytes(self, real):
        return struct.pack(self.standard, real)

    def from_bytes(self, rawbytes):
        length = {'f': 4, 'd': 8}[self.standard]
        return struct.unpack(self.standard, rawbytes[:length])[0]


class StringType(AbstractType):

    def __init__(self, encoding):
        self.encoding = encoding

    def to_bytes(self, string):
        return string.encode(self.encoding) + b'\0'

    def from_bytes(self, rawbytes):
        index = rawbytes.index(b'\0')
        return rawbytes[:index].decode(self.encoding)


class Deserializer:

    def __init__(self, rawbytes):
        self.remaining = rawbytes

    def __copy__(self):
        return Deserializer(self.remaining)

    def read(self, *types):
        output = list()

        for T in types:
            data   = T.from_bytes(self.remaining)
            length = len(T.to_bytes(data))
            output.append(data)
            self.remaining = self.remaining[length:]

        return tuple(output) if len(output) > 1 else output[0]


if __name__ == '__main__':
    from pprint import pprint

    char_t   = IntegerType(1, 'little', True)
    byte_t   = IntegerType(1, 'little', False)
    int_t    = IntegerType(2, 'little', True)
    uint_t   = IntegerType(4, 'little', False)
    string_t = StringType('utf-8')
    float_t  = FloatType('f')

    out = Deserializer(byte_t  (10) +
                       char_t  (ord('X')) +
                       uint_t  (123456) +
                       int_t   (-789) +
                       string_t('hello') +
                       float_t (987.654))

    b, c, u, i = out.read(byte_t, char_t, uint_t, int_t)
    s, f       = out.read(string_t, float_t)

    print(b, c, u, i, s, f, len(out.remaining))
    print(char_t(82))
