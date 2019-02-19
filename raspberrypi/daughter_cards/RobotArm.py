#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
from math import pi

from common.serialtalks import BYTE, INT, LONG, FLOAT
from common.components import SecureSerialTalksProxy
import json

def deg_to_rad(val):
	return (val * pi)/180

_ADD_MOVE_OPCODE 	        = 0X10
_RUN_BATCH_OPCODE	        = 0X11
_STOP_BATCH_OPCODE	        = 0X12
_IS_ARRIVED_OPCODE 	        = 0X13
_GET_MOTORS_STATE_OPCODE 	= 0X14

_START_PUMP_OPCODE          = 0x15
_STOP_PUMP_OPCODE           = 0X16 
_START_SLUICE_OPCODE        = 0X17 
_STOP_SLUICE_OPCODE         = 0x18

class TankFull(Exception): pass
class TankEmpty(Exception): pass

class TakePuckError(Exception): pass

class RobotArm(SecureSerialTalksProxy):
	def __init__(self, manager, uuid='/dev/arduino/arm'):
		SecureSerialTalksProxy.__init__(self, manager, uuid, dict())
		with open('../daughter_cards/ArmPosition.json') as f:
			self.armPosition = json.load(f)
		
		self.TankPosList 	= ["TANK_POS_1", "TANK_POS_2", "TANK_POS_3"]
		self.CurrentTankPos = 0
		self.tankSize		= 2	# size 3 by default [0, 1, 2]
		self.Tankfull		= False
		self.Tankempty		= True

	def setup_tank_size(self, size):
		self.tankSize = size - 1
	
	def get_motors_state(self):
		out = self.execute(_GET_MOTORS_STATE_OPCODE)
		ret = out.read(BYTE, BYTE, BYTE, BYTE, BYTE, BYTE, BYTE, BYTE, BYTE)
		return ret

	def clear_motor_error(self, id):
		self.send(_RUN_BATCH_OPCODE, INT(id))

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

	def move(self, posID):
		self.send(_ADD_MOVE_OPCODE, FLOAT(self.armPosition[posID]['x']),\
									FLOAT(self.armPosition[posID]['y']), \
									FLOAT(deg_to_rad(self.armPosition[posID]['phi'])))
		self.send(_RUN_BATCH_OPCODE)

	def move_pos(self, x, y, phi):
		self.send(_ADD_MOVE_OPCODE, FLOAT(x), FLOAT(y), FLOAT(deg_to_rad(phi)))
		self.send(_RUN_BATCH_OPCODE)

	def go_home(self):
		self.move("GLOBAL_POS_INTER")
		while not self.is_arrived():
			time.sleep(0.1)
		self.move("HOME")
		while not self.is_arrived():
			time.sleep(0.1)

	def take_puck_in_distributor(self):
		self.start_pump()
		self.move("TAKE_PUCK_INTER")
		while not self.is_arrived():
			time.sleep(0.1)

		self.move("TAKE_PUCK")
		while not self.is_arrived():
			time.sleep(0.1)
		time.sleep(0.5)
		self.move("TAKE_PUCK_INTER")

		while not self.is_arrived():
			time.sleep(0.1)

	def put_in_balance(self):
		self.move("balance")
		while not self.is_arrived():
			time.sleep(0.1)
		self.stop_pump()
		self.start_sluice()
		time.sleep(0.5)
		

	def put_in_tank(self):
		if not self.Tankfull:
			self.move("TANK_POS_INTER")
			while not self.is_arrived():
				time.sleep(0.1)
			
			self.move(self.TankPosList[self.CurrentTankPos])

			while not self.is_arrived():
				time.sleep(0.1)

			self.stop_pump()
			self.start_sluice()
			time.sleep(0.5)
			self.move("TANK_POS_INTER")

			self.CurrentTankPos += 1
			if(self.CurrentTankPos > self.tankSize):
				self.CurrentTankPos = self.tankSize
				self.Tankfull		= True

			self.Tankempty		= False
		else:
			raise TankFull()

	def take_in_tank(self):
		if not self.Tankempty:

			self.stop_sluice()
			self.start_pump()

			self.move("TANK_POS_INTER")

			while not self.is_arrived():
				time.sleep(0.1)

			self.move(self.TankPosList[self.CurrentTankPos])

			while not self.is_arrived():
				time.sleep(0.1)

			time.sleep(0.5)
			self.move("TANK_POS_INTER")

			self.CurrentTankPos -= 1
			if(self.CurrentTankPos < 0):
				self.CurrentTankPos = 0
				self.Tankempty		= True
			
			self.Tankfull = False
		else:
			raise TankEmpty()