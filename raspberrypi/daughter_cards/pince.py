#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
import math

from common.serialutils import Deserializer
from common.serialtalks import BYTE, INT, LONG, FLOAT, SerialTalks
from common.components import SerialTalksProxy


_SET_POSITION_OPCODE      =  0x10

class Pince(SerialTalksProxy):
    def __init__(self,parent, uuid='pince'):
        SerialTalksProxy.__init__(self,parent, uuid)

    def write_outdoor(self, position):
        result = self.execute(_SET_POSITION_OPCODE,INT(position))
        return result.read(INT)