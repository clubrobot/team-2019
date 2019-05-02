#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import math
from common.serialutils import Deserializer
from common.serialtalks import SHORT
from common.components import SecureSerialTalksProxy
# Instructions

GET_RANGE1_OPCODE     = 0x10
GET_RANGE2_OPCODE     = 0x11
CHECK_ERROR_OPCODE    = 0x12
GET_BOTH_RANGE_OPCODE     = 0x13

class Sensors(SecureSerialTalksProxy):
    # Default execute result
    DEFAULT = {GET_RANGE1_OPCODE: Deserializer(SHORT(1000)),
              GET_RANGE2_OPCODE:  Deserializer(SHORT(1000))}

    def __init__(self, parent, uuid='sensors'):
        SecureSerialTalksProxy.__init__(self, parent, uuid, default_result=self.DEFAULT)

    def get_range1(self):
        dist = self.execute(GET_RANGE1_OPCODE).read(SHORT)
        if dist == 0:
            return 1000
        return dist

    def get_range2(self):
        dist = self.execute(GET_RANGE2_OPCODE).read(SHORT)
        if dist == 0:
            return 1000
        return dist

    def check_errors(self):
        deser = self.execute(CHECK_ERROR_OPCODE)
        error1 = deser.read(SHORT)
        error2 = deser.read(SHORT)
        return error1, error2

    def get_both_range(self):
        deser = self.execute(GET_BOTH_RANGE_OPCODE)
        dist1 = deser.read(SHORT)
        dist2 = deser.read(SHORT)
        if dist1 == 0:
            dist1=1000
        if dist2 == 0:
            dist2=1000
        return dist1, dist2
