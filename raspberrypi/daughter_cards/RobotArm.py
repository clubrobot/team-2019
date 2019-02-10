#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
from math import pi

from common.serialtalks import BYTE, INT, LONG, FLOAT
from common.components import SecureSerialTalksProxy
import json

def deg_to_rad(val):
	return (val * pi)/180

_ADD_MOVE_OPCODE 	 = 0X10
_RUN_BATCH_OPCODE	 = 0X11
_STOP_BATCH_OPCODE	 = 0X12
_IS_ARRIVED_OPCODE 	 = 0X13

_START_PUMP_OPCODE   = 0x14
_STOP_PUMP_OPCODE    = 0X15 
_START_SLUICE_OPCODE = 0X16 
_STOP_SLUICE_OPCODE  = 0x17

class RobotArm(SecureSerialTalksProxy):
	def __init__(self, manager, uuid='/dev/arduino/arm'):
		SecureSerialTalksProxy.__init__(self, manager, uuid, dict())
		with open('ArmPosition.json') as f:
			self.armPosition= json.load(f)

	def move(self, posID):
		self.send(_ADD_MOVE_OPCODE, FLOAT(self.armPosition[posID]['x']),\
									FLOAT(self.armPosition[posID]['y']), \
									FLOAT(deg_to_rad(self.armPosition[posID]['phi'])))

	def move_pos(self, x, y, phi):
		self.send(_ADD_MOVE_OPCODE, FLOAT(x), FLOAT(y), FLOAT(deg_to_rad(phi)))
		self.send(_RUN_BATCH_OPCODE)

	def go_home(self):
		self.move("HOME")
		self.run_batch()
	
	def run_batch(self):
		self.send(_RUN_BATCH_OPCODE)

	def stop_batch(self):
		self.send(_STOP_BATCH_OPCODE)

	def start_pump(self):
		self.send(_START_PUMP_OPCODE)

	def stop_pump(self):
		self.send(_STOP_PUMP_OPCODE)

	def start_sluice(self):
		self.send(_START_SLUICE_OPCODE)

	def stop_sluice(self):
		self.send(_STOP_SLUICE_OPCODE)

	def is_arrived(self):
		out = self.execute(_IS_ARRIVED_OPCODE)
		ret = out.read(INT)
		return ret