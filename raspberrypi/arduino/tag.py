#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
import math

from common.serialtalks import INT
from common.components import SerialTalksProxy

# Instructions

GET_POSITION_OPCODE  = 0x10


class Tag(SerialTalksProxy):

	def __init__(self, parent, uuid='tag'):
		SerialTalksProxy.__init__(self, parent, uuid)

	def get_position(self, **kwargs):
		output = self.execute(GET_POSITION_OPCODE, **kwargs)
		x, y = output.read(INT, INT)
		return x, y
	

