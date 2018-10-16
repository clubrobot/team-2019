#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
import math

from common.serialtalks import BYTE, INT, LONG, FLOAT
from common.components import SerialTalksProxy

# Instructions

_GET_COLOR_SENSOR_OPCODE = 0x10
_LED_ON_OPCODE           = 0x11
_LED_OFF_OPCODE          = 0x12

class ColorSensor(SerialTalksProxy):
	def __init__(self, parent, uuid='colorSensor'):
		SerialTalksProxy.__init__(self, parent, uuid)

	def get_mesure(self,**kwargs):
		output = self.execute(_GET_COLOR_SENSOR_OPCODE, **kwargs)
		r,g, b = output.read(INT,INT,INT)
		return r,g,b

	def led_on(self, **kwargs):
		self.send(_LED_ON_OPCODE, BYTE(1))
	
	def led_off(self, **kwargs):
		self.send(_LED_OFF_OPCODE, BYTE(1))

		

