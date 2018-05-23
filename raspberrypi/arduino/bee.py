#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
import math

from common.serialtalks import BYTE, INT, LONG, FLOAT, SerialTalks
from common.components import SecureSerialTalksProxy
from robots.get_robot_name import *

_WRITE_BEEACTIVATOR_OPCODE		=   0x27

class BeeActuator(SecureSerialTalksProxy):
    def __init__(self,parent, uuid='watershooter'):
        if uuid=='watershooter' and ROBOT_ID==R128_ID:
            uuid = "128"
        SecureSerialTalksProxy.__init__(self,parent, uuid, dict())
        if ROBOT_ID==BORNIBUS_ID:
            self.closed_position = 0
            self.open_position = 65
        else:
            self.closed_position = 40
            self.open_position = 140

    def write(self, value):
        self.send(_WRITE_BEEACTIVATOR_OPCODE,INT(value))

    def open(self):
        self.write(self.open_position)

    def close(self):
        self.write(self.closed_position)

    def detach(self):
        self.write(-1)