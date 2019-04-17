#!/usr/bin/env python3
#-*- coding: utf-8 -*-
import time
import math

from common.serialtalks import *
from common.components import SerialTalksProxy



# Instructions

ISONTOP_OPCODE 				= 0x11
START_EXPERIENCE_OPCODE		= 0x10


class Electron(SerialTalksProxy):

	def __init__(self, parent, uuid='expServ'):
		SerialTalksProxy.__init__(self, parent, uuid)

	def isOnTop(self):
		output = self.execute(ISONTOP_OPCODE)
		ret = output.read(BYTE)
		return ret

	def start(self):
		self.send(START_EXPERIENCE_OPCODE)
