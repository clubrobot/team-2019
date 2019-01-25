#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
import math

from common.serialtalks import INT
from common.components import SerialTalksProxy

# Instructions
GET_ROBOT_ACC_OPCODE 	 		=	0x10
CALIBRATE_COMPAS_OPCODE			=   0x11
GET_ORIENTATION_OPCODE   		=   0x12
GET_MEASURED_ORIENTATION_OPCODE =	0x13
GET_SENSOR_VALUE_OPCODE         =	0x14
GET_SENSOR_STATE_OPCODE         =	0x15

class AccelerationSensor(SerialTalksProxy):

	def __init__(self, parent, uuid='inertial'):
		SerialTalksProxy.__init__(self, parent, uuid)

	def get_acceleration(self, **kwargs):
		output = self.execute(GET_ROBOT_ACC_OPCODE)
		x, y = output.read(INT, INT)
		return x, y
	
	def set_orientation(self, o):
		self.send(CALIBRATE_COMPAS_OPCODE, INT(o))
	
	def get_orientation(self, **kwargs):
		output = self.execute(GET_ORIENTATION_OPCODE)
		pos = output.read(INT)
		return pos

	def get_measured_orientation(self, **kwargs):
		output = self.execute(GET_MEASURED_ORIENTATION_OPCODE)
		pos = output.read(INT) + 360
		return pos

	def get_sensor_mesure(self, **kwargs):
		output = self.execute(GET_SENSOR_VALUE_OPCODE)
		mx, my, mz, acx, acy, acz = output.read(INT,INT,INT,INT,INT,INT)
		return mx, my, mz, acx, acy, acz		

	def get_state(self, **kwargs):
		output = self.execute(GET_SENSOR_STATE_OPCODE)
		pos = output.read(INT)
		return pos
