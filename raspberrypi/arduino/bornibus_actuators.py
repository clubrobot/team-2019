#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
import math

from common.serialutils import Deserializer
from common.serialtalks import BYTE, INT, LONG, FLOAT, SerialTalks
from common.components import SerialTalksProxy


_SET_POSITION_GRIPPER_OPCODE    =   0x10
_SET_POSITION_PUSHERS_OPCODE    =   0x11
_SET_POSITION_ARM_OPCODE        =   0x12
OPEN_GRIPPER = 65
CLOSED_GRIPPER = 120
UP_PUSHER1 = 135
DOWN_PUSHER1 = 5
UP_PUSHER2 = 15
DOWN_PUSHER2 = 135
DEPLOYED_ARM = 80
UP_ARM = 0

class Gripper(SerialTalksProxy):
    def __init__(self,parent, uuid='actuators'):
        SerialTalksProxy.__init__(self,parent, uuid)

    def open(self):
        self.send(_SET_POSITION_GRIPPER_OPCODE,INT(OPEN_GRIPPER))
    def close(self):
        self.send(_SET_POSITION_GRIPPER_OPCODE,INT(CLOSED_GRIPPER))
    def write_position(self, position):
        self.send(_SET_POSITION_GRIPPER_OPCODE,INT(position))

class Pushers(SerialTalksProxy):
    def __init__(self,parent, uuid='actuators'):
        SerialTalksProxy.__init__(self,parent, uuid)

    def write_position(self, pos_p1, pos_p2):
        self.send(_SET_POSITION_PUSHERS_OPCODE,INT(pos_p1),INT(pos_p2))
    def up(self):
        self.send(_SET_POSITION_PUSHERS_OPCODE,INT(UP_PUSHER1),INT(UP_PUSHER2))
    def down(self):
        self.send(_SET_POSITION_PUSHERS_OPCODE,INT(DOWN_PUSHER1),INT(DOWN_PUSHER2))

class Arm(SerialTalksProxy):
    def __init__(self,parent, uuid='actuators'):
        SerialTalksProxy.__init__(self,parent, uuid)

    def write_position(self, position):
        self.send(_SET_POSITION_ARM_OPCODE,INT(position))
    def deploy(self):
        self.send(_SET_POSITION_ARM_OPCODE,INT(DEPLOYED_ARM))
    def up(self):
        self.send(_SET_POSITION_ARM_OPCODE,INT(UP_ARM))
