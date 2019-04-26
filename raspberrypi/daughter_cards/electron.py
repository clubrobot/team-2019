#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
import math

from common.serialutils import Deserializer
from common.serialtalks import BYTE, INT, LONG, FLOAT, SerialTalks
from common.components import SerialTalksProxy

_IS_CONNECTED_OPCODE    =  0X11
_START_OPCODE           =  0x10
_ISONTOP_OPCODE         =  0X12

class Pince(SerialTalksProxy):
    def __init__(self,parent, uuid='electronServer'):
        SerialTalksProxy.__init__(self,parent, uuid)

    def connected(self):
        output = self.execute(_IS_CONNECTED_OPCODE)
        ret = output.read(BYTE)
        return bool(ret)
        
    def start(self):
        self.send(_START_OPCODE)

    def is_on_top(self):
        output = self.execute(_ISONTOP_OPCODE)
        ret = output.read(BYTE)
        return bool(ret)
