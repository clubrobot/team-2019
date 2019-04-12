#!/usr/bin/env python3
#-*- coding: utf-8 -*-
import time
import math

from common.serialtalks import *


# Instructions

ISONTOP_OPCODE 				= 0x17
START_EXPERIENCE_OPCODE		= 0x19


class Electron(SerialTalks):

	def __init__(self, uuid='expServ'):
		SerialTalks.__init__(self, "/dev/arduino/{}".format(uuid))

	def isOnTop(self):
		output = self.execute(ISONTOP_OPCODE)
		ret = output.read(BYTE)
		return ret

	def start(self):
		self.send(START_EXPERIENCE_OPCODE)
