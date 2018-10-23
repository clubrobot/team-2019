#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import math
from common.serialutils import Deserializer
from common.serialtalks import BYTE, INT, LONG, FLOAT, SerialTalks
from common.components import SecureSerialTalksProxy
# Instructions

_GET_SINGLE_MESURE_OPCODE     = 0x10
_GET_CONTINUOUS_MESURE_OPCODE = 0x11
_START_CONTINUOUS_OPCODE      = 0x12
_STOP_CONTINUOUS_OPCODE       = 0x13


class Sensors(SerialTalks):
    # Default execute result
    def __init__(self, path):
        SerialTalks.__init__(self, path)

    def get_single_mesure(self):
        output = self.execute(_GET_SINGLE_MESURE_OPCODE)
        dist = output.read(FLOAT)
        return dist

    def get_continuous_mesure(self):
        output = self.execute(_GET_CONTINUOUS_MESURE_OPCODE)
        dist = output.read(FLOAT)
        return dist

    def start_continuous(self, MG_activate = 0):
        """ MG_activate : Active ou non la MoyenneGlissante """
        self.send(_START_CONTINUOUS_OPCODE, INT(MG_activate))

    def stop_continuous(self):
        self.send(_STOP_CONTINUOUS_OPCODE)
