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

_SET_PARAMETERS_OPCODE       = 0X19
_GET_PARAMETERS_OPCODE       = 0x1A

MOTOR1_ID_ID                 = 0x10
MOTOR1_OFFSET_ID             = 0x11
MOTOR2_ID_ID                 = 0x12
MOTOR2_OFFSET_ID             = 0x13
MOTOR3_ID_ID                 = 0x14
MOTOR3_OFFSET_ID             = 0x15
WORKSPACE_FRONT_ID           = 0x16
WORKSPACE_BACK_ID            = 0x17
ORIGIN_ID                    = 0x18
JOINTS_ID                    = 0x19
LINK_1_ID                    = 0x1A
LINK_2_ID                    = 0x1B
LINK_3_ID                    = 0x1C
ELBOW_ID                     = 0x1D
TIMESTEP_ID                  = 0x1E

Workspace = namedtuple('Workspace', ['x_min', 'x_max', 'y_min', 'y_max', 'elbow_or'])

SpacePoint = namedtuple('SpacePoint', ['x', 'y', 'phi'])
JointPoint = namedtuple('JointPoint', ['th1', 'th2', 'th3'])

class RobotArm(SecureSerialTalksProxy):
	def __init__(self, manager, uuid='/dev/arduino/arm'):
		SecureSerialTalksProxy.__init__(self, manager, uuid, dict())

	def move(self, x, y, phi):
		out = self.execute(_ADD_MOVE_OPCODE, FLOAT(x), FLOAT(y), FLOAT(math.radians(phi)))
		ret = out.read(BYTE)
		if bool(ret):
			raise RuntimeError('Error on move command : Position unreachable')

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

	def set_parameter_value(self, id, value, valuetype):
		self.send(_SET_PARAMETERS_OPCODE, BYTE(id), valuetype(value))
		time.sleep(0.01)

	def get_parameter_value(self, id, valuetype):
		output = self.execute(_GET_PARAMETERS_OPCODE, BYTE(id))
		value = output.read(valuetype)
		return value

	def set_workspace(self, id, ws):
		self.send(_SET_PARAMETERS_OPCODE, BYTE(id), \
										  FLOAT(ws.x_min), \
										  FLOAT(ws.x_max), \
										  FLOAT(ws.y_min), \
										  FLOAT(ws.y_max), \
										  FLOAT(ws.elbow_or))
		time.sleep(0.01)
	
	def set_origin(self, origin):
		self.send(_SET_PARAMETERS_OPCODE, BYTE(ORIGIN_ID), FLOAT(origin.x), FLOAT(origin.y), FLOAT(origin.phi))
		time.sleep(0.01)
	
	def set_joint(self, joint):
		self.send(_SET_PARAMETERS_OPCODE, BYTE(JOINTS_ID), FLOAT(joint.th1), FLOAT(joint.th2), FLOAT(joint.th3))
		time.sleep(0.01)

	def get_workspace(self, id):
		output = self.execute(_GET_PARAMETERS_OPCODE, BYTE(id))
		value = output.read(FLOAT, FLOAT, FLOAT, FLOAT, FLOAT)
		return Workspace(*value)
	
	def get_origin(self):
		output = self.execute(_GET_PARAMETERS_OPCODE, BYTE(ORIGIN_ID))
		value = output.read(FLOAT, FLOAT, FLOAT)
		return SpacePoint(*value)
	
	def get_joint(self):
		output = self.execute(_GET_PARAMETERS_OPCODE, BYTE(JOINTS_ID))
		value = output.read(FLOAT, FLOAT, FLOAT)
		return JointPoint(*value)