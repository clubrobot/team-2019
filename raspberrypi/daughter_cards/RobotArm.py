#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
import math

from common.serialtalks import BYTE, INT, LONG, FLOAT, SerialTalks
from common.components import SecureSerialTalksProxy


_ADD_MOVE_OPCODE 	 = 0X10
_RUN_BATCH_OPCODE	 = 0X11
_STOP_BATCH_OPCODE	 = 0X12
_IS_ARRIVED_OPCODE 	 = 0X13

class RobotArm(SecureSerialTalksProxy):
	def __init__(self, uuid='128'):
		SerialTalks.__init__(self, "/dev/arduino/{}".format(uuid))

	def move(self, x, y ,theta):
		self.send(_ADD_MOVE_OPCODE, FLOAT(x), FLOAT(y), FLOAT(theta))
	
	def run_batch(self):
		self.send(_RUN_BATCH_OPCODE)

	def stop_batch(self):
		self.send(_STOP_BATCH_OPCODE)

	def is_arrived(self):
		out = self.execute(_IS_ARRIVED_OPCODE)
		ret = out.read(INT)
		return ret