#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
import math

from common.serialtalks import BYTE, INT, LONG, FLOAT, SerialTalks
from common.components import SecureSerialTalksProxy
from common.serialutils import Deserializer


_BEGIN_OPCODE 		 = 0X11
_SET_POS_OPCODE 	 = 0X12

_SET_X_OPCODE 		 = 0X13
_SET_Y_OPCODE 		 = 0X14
_SET_Z_OPCODE 		 = 0X15
_SET_THETA_OPCODE 	 = 0X16
_SET_SPEED_OPCODE 	 = 0X17

_GET_POS_OPCODE 	 = 0X18
_GET_POS_THEO_OPCODE = 0X19
_SET_ANGLES_OPCODE	 = 0X1A
_128_GET_EMERGENCY_OPCODE = 0X22
_OPEN_GRIPPER_OPCODE = 0X1B
_CLOSE_GRIPPER_OPCODE = 0X1C

Z_FIRST = 1
Z_LAST  = 0

CROSS_1 = 0
CROSS_2 = 1
CROSS_3 = 2
CROSS_4 = 3
CROSS_5 = 4
CROSS_6 = 5

GREEN_CUBE  = 0
BLUE_CUBE	= 1
ORANGE_CUBE = 2
BLACK_CUBE	= 3
YELLOW_CUBE = 4

#  		         GREEN     |        BLUE       |       ORANGE      |       BLACK      |      YELLOW
CROSS1 = [(540  , 792  , 0), (598  , 850  , -90), (540  , 895  , 180), (482  , 850  , 90), (540  , 850  , 90)]
CROSS2 = [(1190 , 242  , 0), (1248 , 300  , -90), (1190 , 358  , 180), (1132 , 300  , 90), (1190 , 300  , 90)]
CROSS3 = [(1500 , 1042 , 0), (1558 , 1100 , -90), (1500 , 1158 , 180), (1442 , 1100 , 90), (1500 , 1100 , 90)]
CROSS4 = [(1500 , 1958 , 0), (1558 , 1900 , -90), (1500 , 1942 , 180), (1442 , 1900 , 90), (1500 , 1900 , 90)]
CROSS5 = [(1190 , 2758 , 0), (1248 , 2700 , -90), (1190 , 2642 , 180), (1132 , 2700 , 90), (1190 , 2700 , 90)]
CROSS6 = [(540  , 2208 , 0), (598  , 2150 , -90), (540  , 2092 , 180), (482  , 2150 , 90), (540  , 2150 , 90)]

CROSS_LIST = [CROSS1, CROSS2, CROSS3, CROSS4, CROSS5, CROSS6]


class RobotArm(SecureSerialTalksProxy):
    MAX_Z = 15
    MIN_Z = 2
    CLOSER_CUBE     = (-17.5, 6,  MIN_Z, 180, 0)
    LEFT_CUBE       = (-22.5, 12, MIN_Z, 270, 0)
    RIGHT_CUBE      = (-22.5, -1, MIN_Z, 90, 0)
    MIDDLE_CUBE     = (-22.5, 6,  MIN_Z, 180, 0)
    AWAY_CUBE       = (-27.5, 6,  MIN_Z, 0, 0)
    TANK = (25, 15, MAX_Z, 0, 1)
    TEMP = (0, 0, 0, MIN_Z, 1)
    ABOVE_CROSS = (-17.5, 6,  MAX_Z/2, 180, 0)
    CUBES_POS = [CLOSER_CUBE, LEFT_CUBE, RIGHT_CUBE, MIDDLE_CUBE]

    _DEFAULT = {
        _128_GET_EMERGENCY_OPCODE : Deserializer(BYTE(0)),
        _GET_POS_OPCODE: Deserializer(BYTE(0)+BYTE(0)+BYTE(0)),
        _GET_POS_THEO_OPCODE: Deserializer(BYTE(0)+BYTE(0)+BYTE(0))
    }

    def __init__(self, manager, uuid='128'):
        SecureSerialTalksProxy.__init__(self, manager, uuid, RobotArm._DEFAULT)
        self.logger = None

    def set_logger(self, logger):
        self.logger = logger

    def begin(self):
        self.send(_BEGIN_OPCODE)

    def set_pos(self, x, y, z, theta, z_o):
        self.send(_SET_POS_OPCODE, FLOAT(x), FLOAT(y), FLOAT(z), FLOAT(theta), INT(z_o))

    def set_x(self, x):
        self.send(_SET_X_OPCODE, FLOAT(x))

    def set_y(self, y):
        self.send(_SET_Y_OPCODE, FLOAT(y))

    def set_z(self, z):
        self.send(_SET_Z_OPCODE, FLOAT(z))

    def set_theta(self, theta):
        self.send(_SET_THETA_OPCODE, FLOAT(theta))

    def set_speed(self, speed):
        self.send(_SET_SPEED_OPCODE, FLOAT(speed))

    def get_pos(self):
        output = self.execute(_GET_POS_OPCODE)
        x, y, z = output.read(FLOAT, FLOAT, FLOAT)
        return x, y, z

    def get_pos_theo(self):
        output = self.execute(_GET_POS_THEO_OPCODE)
        x, y, z = output.read(FLOAT, FLOAT, FLOAT)
        return x, y, z

    def set_angles(self, a, b, c):
        self.send(_SET_ANGLES_OPCODE, FLOAT(a), FLOAT(b), FLOAT(c))

    def open_gripper(self):
        self.send(_OPEN_GRIPPER_OPCODE)

    def close_gripper(self):
        self.send(_CLOSE_GRIPPER_OPCODE)

    def grab_cube(self, cube_pos):
        self.logger("ROBOT ARM : ", "Grab cube starts")
        self.set_pos(*cube_pos)
        time.sleep(2)
        self.close_gripper()
        time.sleep(1)
        self.set_z(RobotArm.MAX_Z/2)
        time.sleep(1)
        self.logger("ROBOT ARM : ", "Grab cube ended")

    def put_in_tank(self, cube_pos, height):
        self.logger("ROBOT ARM", "Put in tank starts")
        self.grab_cube(cube_pos)
        self.set_z(RobotArm.MAX_Z)
        self.set_pos(*RobotArm.TANK)
        time.sleep(2)
        self.set_z(height*5+RobotArm.MIN_Z)
        time.sleep(2)
        self.open_gripper()
        time.sleep(1)
        self.set_pos(*RobotArm.TANK)
        time.sleep(2)
        self.logger("ROBOT ARM", "Put in tank ended")

    def put_in_temp(self, cube_pos):
        self.logger("ROBOT ARM", "Put in temp starts")
        self.grab_cube(cube_pos)
        self.set_pos(*RobotArm.TEMP)
        time.sleep(2)
        self.open_gripper()
        time.sleep(1)
        self.set_pos(*RobotArm.ABOVE_CROSS)
        time.sleep(2)
        self.logger("ROBOT ARM", "Put in temp ended")

    def put_from_temp_to_tank(self, height):
        self.logger("ROBOT ARM", "Put from temp to tank starts")
        self.grab_cube(RobotArm.TEMP)
        self.set_z(RobotArm.MAX_Z)
        self.set_pos(*RobotArm.TANK)
        time.sleep(2)
        self.set_z(height*5+RobotArm.MIN_Z)
        time.sleep(2)
        self.open_gripper()
        time.sleep(1)
        self.set_pos(*RobotArm.TANK)
        time.sleep(2)
        self.logger("ROBOT ARM", "Put from temp to tank ended")

    def get_emergency_state(self):
        output = self.execute(_128_GET_EMERGENCY_OPCODE)
        return output.read(INT)

