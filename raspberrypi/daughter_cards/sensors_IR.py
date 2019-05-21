#!/usr/bin/env python3
#-*- coding: utf-8 -*-

from common.serialutils import Deserializer
from common.serialtalks import SHORT
from common.components import SecureSerialTalksProxy
import time

# Instructions
GET_RANGE1_OPCODE     = 0x10
GET_RANGE2_OPCODE     = 0x11
CHECK_ERROR_OPCODE    = 0x12
GET_BOTH_RANGE_OPCODE     = 0x13


class SensorsIR(SecureSerialTalksProxy):
    MAX_PERIOD = 0.1
    ERROR_DIST = 1000
    DEFAULT = {GET_RANGE1_OPCODE: Deserializer(SHORT(ERROR_DIST)),
               GET_RANGE2_OPCODE: Deserializer(SHORT(ERROR_DIST))}

    # Default execute result

    def __init__(self, parent, uuid='sensors'):
        SecureSerialTalksProxy.__init__(self, parent, uuid, default_result=self.DEFAULT)
        self.last_time = None
        self.last_both = (SensorsIR.ERROR_DIST, SensorsIR.ERROR_DIST)

    def get_range1(self):
        return self.get_both_range()[0]

    def get_range2(self):
        return self.get_both_range()[1]

    def check_errors(self):
        deser = self.execute(CHECK_ERROR_OPCODE)
        error1 = deser.read(SHORT)
        error2 = deser.read(SHORT)
        return error1, error2

    def get_both_range(self):
        current_time = time.time()
        if self.last_time is not None and current_time - self.last_time < SensorsIR.MAX_PERIOD:
            return self.last_both

        deser = self.execute(GET_BOTH_RANGE_OPCODE)
        dist1 = deser.read(SHORT)
        dist2 = deser.read(SHORT)

        if dist1 == 0:
            dist1 = SensorsIR.ERROR_DIST
        if dist2 == 0:
            dist2 = SensorsIR.ERROR_DIST

        self.last_both = (dist1, dist2)
        self.last_time = time.time()
        return self.last_both
