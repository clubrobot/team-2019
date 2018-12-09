#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
import math

from common.serialtalks import BYTE, INT, CHAR, STRING
from common.components import SecureSerialTalksProxy

# Instructions

SET_EEPROM_DEFAULT_MESSAGE_OPCODE   = 0x09
SET_SPEED_MATRIX_OPCODE             = 0x0A
SET_EEPROM_CHAR_LEDMATRIX_OPCODE    = 0x0B
SET_EEPROM_CHAR_IPDISPLAY_OPCODE    = 0x0C
SET_MATRIX_MESSAGE_OPCODE           = 0x0D
SET_IPDISPLAY_MESSAGE_OPCODE        = 0x0E

SLIDE_MODE          = 0
ANIMATION_MODE      = 1
RIGHT_ROTATION_MODE = 2
LEFT_ROTATION_MODE  = 3
UPSIDEDOWN_MODE     = 4


class LEDMatrix(SecureSerialTalksProxy):
	
	def __init__(self, parent, matrix_id, uuid='display'):
		SecureSerialTalksProxy.__init__(self, parent, uuid, dict())
		self.matrix_id = matrix_id

	def set_message(self, message, mode=None, speed=None):
		if not len(message) < 20:
			raise ValueError('message length must be less than 20 characters')
		if mode is None and len(message) > 1:
			mode = SLIDE_MODE
		elif mode is None:
			mode = ANIMATION_MODE
		self.send(SET_MATRIX_MESSAGE_OPCODE, BYTE(self.matrix_id), BYTE(mode), STRING(message))
		if speed is not None:
			self.set_speed(speed)
	
	def set_speed(self, speed): # `speed` in milliseconds
		self.send(SET_SPEED_MATRIX_OPCODE, BYTE(self.matrix_id), INT(speed))

	def set_default_message(self, message, mode, speed):
		if not len(message) < 8:
			raise ValueError('default message length must be less than 8 characters')
		self.send(SET_EEPROM_DEFAULT_MESSAGE_OPCODE, BYTE(self.matrix_id), INT(speed), BYTE(mode), STRING(message)) 
	
	def upload_char_pattern(self, char, pattern):
		lines = []
		pattern = pattern.convert('L')
		for y in range(8):
			line = 0
			for x in range(pattern.width):
				digit = ~(pattern.getpixel((x, y)) // 255)
				line = (line << 1) | (digit & 1)
			line <<= 8 - pattern.width
			lines.append(BYTE(line))
		self.send(SET_EEPROM_CHAR_LEDMATRIX_OPCODE, CHAR(ord(char)), *lines, BYTE(pattern.width))

	def upload_charset(self, charset):
		if isinstance(charset, str):
			from PIL import Image
			charset = Image.open(charset)
		charset = charset.convert('L')
		for i in range(16):
			for j in range(6):
				x0, y0 = 8 * i, 8 * j
				x1, y1 = x0 + 8, y0 + 8
				while charset.getpixel((x1 - 1, y0)) not in (0, 255): x1 -= 1
				while charset.getpixel((x0, y1 - 1)) not in (0, 255): y1 -= 1
				pattern = charset.crop((x0, y0, x1, y1))
				char = chr(i + 16 * j + ord(' '))
				self.upload_char_pattern(char, pattern)
				time.sleep(0.1)
		for i in range(7): # Bad upload for the seven first characters
			j = 0
			x0, y0 = 8 * i, 8 * j
			x1, y1 = x0 + 8, y0 + 8
			while charset.getpixel((x1 - 1, y0)) not in (0, 255): x1 -= 1
			while charset.getpixel((x0, y1 - 1)) not in (0, 255): y1 -= 1
			pattern = charset.crop((x0, y0, x1, y1))
			char = chr(i + 16 * j + ord(' '))
			self.upload_char_pattern(char, pattern)
			time.sleep(0.1)


class SevenSegments(SecureSerialTalksProxy):
	
	def __init__(self, parent, uuid='display'):
		SecureSerialTalksProxy.__init__(self, parent, uuid, dict())

	def set_message(self, message):
		if len(message) > 12:
			raise ValueError('message length must 12 characters or less')
		self.send(SET_IPDISPLAY_MESSAGE_OPCODE, STRING(message))

	def upload_char_pattern(self, char, pattern):
		segments = 0
		pattern = pattern.convert('L')
		for i, (x, y) in enumerate([(1, 0), (2, 1), (2, 3), (1, 4), (0, 3), (0, 1), (1, 2)]):
			digit = ~(pattern.getpixel((x, y)) // 255)
			segments = (segments << 1) | (digit & 1)
		segments = (segments << 1) | (char == '.')
		self.send(SET_EEPROM_CHAR_IPDISPLAY_OPCODE, CHAR(ord(char)), BYTE(segments))

	def upload_charset(self, charset):
		if isinstance(charset, str):
			from PIL import Image
			charset = Image.open(charset)
		for i in range(16):
			for j in range(6):
				pattern = charset.crop((4 * i, 6 * j, 4 * (i + 1), 6 * (j + 1)))
				char = chr(i + 16 * j + ord(' '))
				self.upload_char_pattern(char, pattern)
