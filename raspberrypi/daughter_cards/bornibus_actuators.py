#!/usr/bin/env python3
#-*- coding: utf-8 -*-

from common.serialtalks import BYTE, INT, LONG, FLOAT, SerialTalks
from common.components import SecureSerialTalksProxy

_SET_POSITION_GRIPPER_OPCODE    = 0x10
_SET_POSITION_PUSHERS_OPCODE    = 0x11
_SET_POSITION_ARM_OPCODE        = 0x12
_GET_ENDSTOP1_STATE_OPCODE      = 0x13
_GET_ENDSTOP2_STATE_OPCODE      = 0x14
_GET_ENDSTOP3_STATE_OPCODE      = 0x15

OPEN_GRIPPER = 90
CLOSED_GRIPPER = 140 ##128 avant #140 avec le premier proto
UP_PUSHER1 = 10
DOWN_PUSHER1 = 150
UP_PUSHER2 = 167
DOWN_PUSHER2 = 43
DEPLOYED_ARM = 83
UP_ARM = 8


class Gripper(SecureSerialTalksProxy):
    _DEFAULT = {}

    def __init__(self, parent, endstops, uuid='actuators'):
        SecureSerialTalksProxy.__init__(self, parent, uuid, Gripper._DEFAULT)
        self.goldsensor = endstops

    def open(self):
        self.send(_SET_POSITION_GRIPPER_OPCODE, INT(OPEN_GRIPPER))

    def close(self):
        self.send(_SET_POSITION_GRIPPER_OPCODE, INT(CLOSED_GRIPPER))

    def write_position(self, position):
        self.send(_SET_POSITION_GRIPPER_OPCODE, INT(position))

    def get_goldsensor_state(self):
        return self.goldsensor.get_ES3()


class Pushers(SecureSerialTalksProxy):
    _DEFAULT = {}
    left_pusher = UP_PUSHER1
    right_pusher = UP_PUSHER2

    def __init__(self, parent, uuid='actuators'):
        SecureSerialTalksProxy.__init__(self, parent, uuid, Pushers._DEFAULT)

    def write_position(self, pos_p2, pos_p1):
        self.send(_SET_POSITION_PUSHERS_OPCODE, INT(pos_p1), INT(pos_p2))

    def up_l(self):
        self.left_pusher = UP_PUSHER1
        self.send(_SET_POSITION_PUSHERS_OPCODE, INT(self.right_pusher), INT(self.left_pusher))

    def down_l(self):
        self.left_pusher = DOWN_PUSHER1
        self.send(_SET_POSITION_PUSHERS_OPCODE, INT(self.right_pusher), INT(self.left_pusher))

    def up_r(self):
        self.right_pusher = UP_PUSHER2
        self.send(_SET_POSITION_PUSHERS_OPCODE, INT(self.right_pusher), INT(self.left_pusher))

    def down_r(self):
        self.right_pusher = DOWN_PUSHER2
        self.send(_SET_POSITION_PUSHERS_OPCODE, INT(self.right_pusher), INT(self.left_pusher))
    
    def up(self):
        self.right_pusher = UP_PUSHER2
        self.left_pusher = UP_PUSHER1
        self.send(_SET_POSITION_PUSHERS_OPCODE, INT(self.right_pusher), INT(self.left_pusher))

    def down(self):
        self.right_pusher = DOWN_PUSHER2
        self.left_pusher = DOWN_PUSHER1
        self.send(_SET_POSITION_PUSHERS_OPCODE, INT(self.right_pusher), INT(self.left_pusher))

class Arm(SecureSerialTalksProxy):
    _DEFAULT = {}

    def __init__(self, parent, uuid='actuators'):
        SecureSerialTalksProxy.__init__(self, parent, uuid, Arm._DEFAULT)

    def write_position(self, position):
        self.send(_SET_POSITION_ARM_OPCODE,INT(position))

    def deploy(self):
        self.send(_SET_POSITION_ARM_OPCODE,INT(DEPLOYED_ARM))

    def up(self):
        self.send(_SET_POSITION_ARM_OPCODE,INT(UP_ARM))

class EndStops(SecureSerialTalksProxy):
    _DEFAULT = {}

    def __init__(self, parent, uuid='actuators'):
        SecureSerialTalksProxy.__init__(self, parent, uuid, EndStops._DEFAULT)

    def get_ES1(self):
        out = self.execute(_GET_ENDSTOP1_STATE_OPCODE)
        return out.read(INT)==0
    def get_ES2(self):
        out = self.execute(_GET_ENDSTOP2_STATE_OPCODE)
        return out.read(INT)==0
    def get_ES3(self):
        out = self.execute(_GET_ENDSTOP3_STATE_OPCODE)
        return out.read(INT)==0
