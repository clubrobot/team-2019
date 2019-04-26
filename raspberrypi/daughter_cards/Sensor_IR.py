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


class Sensors(SecureSerialTalksProxy):
    # Default execute result
    def __init__(self, parent, uuid='sensors'):
        SecureSerialTalksProxy.__init__(self, parent, uuid, dict())

    def get_range1(self):
        return self.execute(GET_RANGE1_OPCODE).read(SHORT)

    def get_range2(self):
        return self.execute(GET_RANGE2_OPCODE).read(SHORT)
