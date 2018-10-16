#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
import math

from common.serialtalks import BYTE, INT, LONG, FLOAT, SerialTalks
#from common.components import SerialTalksProxy


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
CROSS1 = [(540  , 792  , 0),(598  , 850  , -90),(540  , 895  , 180),(482  , 850  , 90),(540  , 850  , 90)]
CROSS2 = [(1190 , 242  , 0),(1248 , 300  , -90),(1190 , 358  , 180),(1132 , 300  , 90),(1190 , 300  , 90)]
CROSS3 = [(1500 , 1042 , 0),(1558 , 1100 , -90),(1500 , 1158 , 180),(1442 , 1100 , 90),(1500 , 1100 , 90)]

CROSS4 = [(1500 , 1958 , 0),(1558 , 1900 , -90),(1500 , 1942 , 180),(1442 , 1900 , 90),(1500 , 1900 , 90)]
CROSS5 = [(1190 , 2758 , 0),(1248 , 2700 , -90),(1190 , 2642 , 180),(1132 , 2700 , 90),(1190 , 2700 , 90)]
CROSS6 = [(540  , 2208 , 0),(598  , 2150 , -90),(540  , 2092 , 180),(482  , 2150 , 90),(540  , 2150 , 90)]

CROSS_LIST = [CROSS1, CROSS2, CROSS3, CROSS4, CROSS5, CROSS6]


class RobotArm(SerialTalks):
	def __init__(self, uuid='128'):
		SerialTalks.__init__(self, "/dev/arduino/{}".format(uuid))
		r_x = 0
		r_y = 0
		r_t = 0

	def begin(self):
		self.send(_BEGIN_OPCODE)

	def set_robot_pos(x, y, theta):
		r_x = x
		r_y = y
		r_t = theta

	def set_pos(self, x, y ,z ,theta, z_o):
		self.send(_SET_POS_OPCODE, FLOAT(x), FLOAT(y), FLOAT(z), FLOAT(theta),INT(z_o))

	def put_in_tank(self):
		output = self.execute(_SET_POS_OPCODE, FLOAT(10), FLOAT(10), FLOAT(10), FLOAT(90))
		ret = output.read(INT);
		return ret

	def process_cubes(self, num_cubes, cross_num, order_list):
		for i in range(0,num_cubes):
			x, y, th = CROSS_LIST[cross_num][order_list[i]]
			self.set_pos(x,y)

	def set_x(self,x):
		output = self.execute(_SET_X_OPCODE, FLOAT(x))
		ret = output.read(INT);
		if(ret):
			return "Move to x = "+str(x)
		else:
			return "Postition unreachable, try new pos"

	def set_y(self,y):
		output = self.execute(_SET_Y_OPCODE, FLOAT(y))
		ret = output.read(INT);
		if(ret):
			return "Move to y = "+str(y)
		else:
			return "Postition unreachable, try new pos"

	def set_z(self,z):
		self.send(_SET_Z_OPCODE, FLOAT(z))

	def set_theta(self,theta):
		output = self.execute(_SET_THETA_OPCODE, FLOAT(theta))
		ret = output.read(INT);
		if(ret):
			return "Move to theta = "+str(theta)
		else:
			return "Postition unreachable, try new pos"

	def set_speed(self,speed):
		self.send(_SET_SPEED_OPCODE, FLOAT(speed))

	def get_pos(self):
		output = self.execute(_GET_POS_OPCODE)
		x, y, z = output.read(FLOAT, FLOAT, FLOAT)
		return (x,y,z)

	def get_pos_theo(self):
		output = self.execute(_GET_POS_THEO_OPCODE)
		x, y, z = output.read(FLOAT, FLOAT, FLOAT)
		return (x,y,z)

	def set_angles(self,a,b,c):
		self.send(_SET_ANGLES_OPCODE, FLOAT(a),FLOAT(b),FLOAT(c))

	def open_gripper(self):
		self.send(_OPEN_GRIPPER_OPCODE)

	def close_gripper(self):
		self.send(_CLOSE_GRIPPER_OPCODE)