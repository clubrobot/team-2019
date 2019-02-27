#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
from math import pi
from collections import namedtuple
from common.serialtalks import BYTE, INT, LONG, FLOAT
from common.components import SecureSerialTalksProxy
import math

ELBOW_BACK = -1
ELBOW_FRONT = 1

_ARM_BEGIN_OPCODE		     = 0X10
_ADD_MOVE_OPCODE             = 0x11
_RUN_BATCH_OPCODE            = 0X12 
_STOP_BATCH_OPCODE           = 0X13 
_IS_ARRIVED_OPCODE           = 0x14

_START_PUMP_OPCODE           = 0x15
_STOP_PUMP_OPCODE            = 0X16 
_START_SLUICE_OPCODE         = 0X17 
_STOP_SLUICE_OPCODE          = 0x18

_SET_MOTORS_ID_OPCODE        = 0X19
_SET_MOTORS_OFFSET_OPCODE    = 0X1A
_SET_WORKSPACE_OPCODE        = 0X1B
_SET_ORIGIN_OPCODE           = 0X1C
_SET_LINK_LEN_OPCODE         = 0X1D

Workspace = namedtuple('Workspace', ['x_min', 'x_max', 'y_min', 'y_max', 'elbow_or'])

RobotSpacePoint = namedtuple('RobotSpacePoint', ['x', 'y', 'phi'])

class RobotArm(SecureSerialTalksProxy):
	def __init__(self, manager, uuid='/dev/arduino/arm'):
		SecureSerialTalksProxy.__init__(self, manager, uuid, dict())

	def begin(self):
		self.send(_ARM_BEGIN_OPCODE)

	def move(self, x, y, phi):
		out = self.execute(_ADD_MOVE_OPCODE, FLOAT(x), FLOAT(y), FLOAT(math.radians(phi)))
		ret = out.read(BYTE)
		if bool(ret):
			raise RuntimeError('Error on move command : Position unreachable')
		self.send(_RUN_BATCH_OPCODE)

	def run_batch(self):
		self.send(_RUN_BATCH_OPCODE)

	def stop_batch(self):
		self.send(_STOP_BATCH_OPCODE)

	def is_arrived(self):
		out = self.execute(_IS_ARRIVED_OPCODE)
		ret, err = out.read(BYTE, BYTE)
		if bool(err):
			err = self.get_motors_state()
			if bool(err[1]) or bool(err[4]) or bool(err[7]):
				raise RuntimeError('Timeout error on AX12')
			else:
				raise RuntimeError('Error on AX12 motors')
		return bool(ret)

	def start_pump(self):
		self.send(_START_PUMP_OPCODE)

	def stop_pump(self):
		self.send(_STOP_PUMP_OPCODE)

	def start_sluice(self):
		self.send(_START_SLUICE_OPCODE)

	def stop_sluice(self):
		self.send(_STOP_SLUICE_OPCODE)

	def set_motors_id(self, id1, id2, id3):
		self.send(_SET_MOTORS_ID_OPCODE, INT(id1), INT(id2), INT(id3))

	def set_motors_offsets(self, off1, off2, off3):
		self.send(_SET_MOTORS_OFFSET_OPCODE, FLOAT(off1), FLOAT(off2), FLOAT(off3))
	
	def set_workspaces(self, ws_front, ws_back):
		self.send(_SET_WORKSPACE_OPCODE, FLOAT(ws_front.x_min), FLOAT(ws_front.x_max),\
										 FLOAT(ws_front.y_min), FLOAT(ws_front.y_max),\
										 INT(ws_front.elbow_or), \
										 FLOAT(ws_back.x_min), FLOAT(ws_back.x_max),\
									     FLOAT(ws_back.y_min), FLOAT(ws_back.y_max),\
										 INT(ws_back.elbow_or))

	def set_origin(self, origin):
		self.send(_SET_ORIGIN_OPCODE, FLOAT(origin.x), FLOAT(origin.y), FLOAT(origin.phi))
	
	def set_links_len(self, link1, link2, link3):
		self.send(_SET_LINK_LEN_OPCODE, FLOAT(link1), FLOAT(link2), FLOAT(link3))