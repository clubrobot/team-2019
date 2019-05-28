#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
import math

from common.serialutils import Deserializer
from common.serialtalks import BYTE, INT, LONG, FLOAT, SerialTalks
from common.components import SecureSerialTalksProxy

_IS_CONNECTED_OPCODE         = 0X15
_START_OPCODE                = 0X16

_EXPERIENCE_STATE_OPCODE     = 0X17
_ELECTRON_STATE_OPCODE       = 0X18


class Electron(SecureSerialTalksProxy):
    def __init__(self,parent, uuid='expServ'):
        SecureSerialTalksProxy.__init__(self, parent, uuid, dict())

    def connect_sate(self):
        output = self.execute(_IS_CONNECTED_OPCODE)
        ret = output.read(BYTE)
        return bool(ret)
        
    def start(self):
        self.send(_START_OPCODE)

    def experience_started(self):
        output = self.execute(_EXPERIENCE_STATE_OPCODE)
        ret = output.read(BYTE)
        return bool(ret)

    def electron_started(self):
        output = self.execute(_ELECTRON_STATE_OPCODE)
        ret = output.read(BYTE)
        return bool(ret)


if __name__ == "__main__":
    from robots.setup_serialtalks import *

    e = Electron(manager)