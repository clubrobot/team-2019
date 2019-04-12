#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import math
from common.serialutils import Deserializer
from common.serialtalks import BYTE, INT, LONG, FLOAT, STRING, SerialTalks
from common.components import SerialTalksProxy
# Instructions

GET_DIST_OPCODE     = 0x10
GET_STATE_OPCODE    = 0x11
GET_VL53_OPCODE     = 0x12
GET_VL61_OPCODE     = 0x13



class Sensors(SerialTalksProxy):
    # Default execute result
    def __init__(self, parent, uuid='sensors_IR'):
        SerialTalksProxy.__init__(self, parent, uuid)

    def get_dist(self):
        output = self.execute(GET_DIST_OPCODE)
        dist = output.read(INT)
        return dist
    
    def get_state(self):
        output = self.execute(GET_STATE_OPCODE)
        st = output.read(INT)
        return st
    
    def get_VL53(self):
        output = self.execute(GET_VL53_OPCODE)
        dist = output.read(INT)
        return dist

    def get_VL61(self):
        output = self.execute(GET_VL61_OPCODE)
        dist = output.read(INT)
        return dist