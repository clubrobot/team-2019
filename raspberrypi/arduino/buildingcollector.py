#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
import math

from common.serialtalks import BYTE, INT, LONG, FLOAT
from common.components import SerialTalksProxy

_GRAB_LEFT_SIDE_OPCODE   = 0x10
_GRAB_RIGHT_SIDE_OPCODE   = 0x20
_GRAB_CENTER_SIDE_OPCODE = 0x30

_DROP_TOWER_OPCODE       = 0x40

class BuildingCollector(SerialTalksProxy):	
    def __init__(self, parent, uuid='buildingcollector'):
        SerialTalksProxy.__init__(self, parent, uuid)

    def grab_left(self):
        self.send(_GRAB_LEFT_SIDE_OPCODE)
    
    def grab_right(self):
        self.send(_GRAB_RIGHT_SIDE_OPCODE)

    def grab_center(self):
        self.send(_GRAB_CENTER_SIDE_OPCODE)

    def drop_tower(self):
        self.send(_DROP_TOWER_OPCODE)