#!/usr/bin/env python3
#-*- coding: utf-8 -*-
import time
import math

from common.serialtalks import *


# Instructions

UPDATE_ANCHOR_NUMBER_OPCODE = 0x10
UPDATE_ANTENNA_DELAY_OPCODE = 0x11
CALIBRATION_ROUTINE_OPCODE 	= 0x12
UPDATE_COLOR_OPCODE 		= 0x13
GET_POSITION_OPCODE 		= 0x14
GET_PANEL_STATUS_OPCODE 	= 0x17
CHANGE_CHANNEL_OPCODE       = 0x18


class Anchor(SerialTalks):

	def __init__(self, uuid='anchor'):
		SerialTalks.__init__(self, "/dev/arduino/{}".format(uuid))

	def update_beacon_number(self, number):
		self.send(UPDATE_ANCHOR_NUMBER_OPCODE,BYTE(number))
		print(self.getout(timeout=1))
	
	def update_antenna_delay(self, delay):
		self.send(UPDATE_ANTENNA_DELAY_OPCODE,INT(delay))
		print(self.getout(timeout=1))

#	real_distance in mm, timeout in ms
	def calibrate(self,real_distance, timeout):
		self.send(CALIBRATION_ROUTINE_OPCODE,INT(real_distance),ULONG(timeout))
		print(self.getout(timeout=1))

#green : 0, orange : 1
	def update_color(self, color):
		self.send(UPDATE_COLOR_OPCODE,INT(color))

	def get_position(self, robotID):
		output = self.execute(GET_POSITION_OPCODE,INT(robotID))
		x, y = output.read(INT, INT)
		return x, y		

	def is_panel_on(self):
		output = self.execute(GET_PANEL_STATUS_OPCODE)
		ret = output.read(BYTE)
		return ret

	def change_channel(self):
		self.send(CHANGE_CHANNEL_OPCODE)
		


