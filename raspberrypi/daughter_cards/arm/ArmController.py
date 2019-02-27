#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
from math import pi
from daughter_cards.arm.RobotArm import *
from daughter_cards.arm.ArmPosition import *
import json
import math

class ArmController():
    def __init__(self, arm):
        self.TankPosList 		= [TANK_POS_1, TANK_POS_2, TANK_POS_3]
        self.TankPosListTake 	= [TANK_POS_INTER_TAKE_PUCK1, TANK_POS_INTER_TAKE_PUCK2, TANK_POS_INTER_TAKE_PUCK3]
        self.TankPosListTakeBis = [TANK_POS_INTER_TAKE_PUCK1_BIS, TANK_POS_INTER_TAKE_PUCK2_BIS, TANK_POS_INTER_TAKE_PUCK3_BIS]

        self.arm = arm
        self.CurrentTankPos = 0
        self.tankSize		= 2	# size 3 by default [0, 1, 2]
        self.Tankfull		= False
        self.Tankempty		= True

    # private func
    def move(self, PosID):
        self.arm.move(PosID.x, PosID.y, PosID.phi)

    def arrived(self):
        return self.arrived()

    def start_pump(self):
        self.arm.start_pump()

    def stop_pump(self):
        self.arm.stop_pump()

    def start_sluice(self):
        self.arm.start_sluice()

    def stop_sluice(self):
        self.arm.stop_sluice()

    def setup_tank_size(self, size):
        self.tankSize = size - 1

    def go_home(self):
        self.move(GLOBAL_POS_INTER)
        while not self.arrived():
            time.sleep(0.1)
        self.move(HOME)
        while not self.arrived():
            time.sleep(0.1)

    def take_puck_in_distributor(self):
        self.start_pump()
        self.move(TAKE_PUCK_INTER_BEFORE)
        while not self.arrived():
            time.sleep(0.1)

        self.move(TAKE_PUCK)
        while not self.arrived():
            time.sleep(0.1)
        time.sleep(1)

        self.move(TAKE_PUCK_INTER_AFTER_1)
        while not self.arrived():
            time.sleep(0.1)

        self.move(TAKE_PUCK_INTER_AFTER_2)
        while not self.arrived():
            time.sleep(0.1)

    def put_in_balance(self):
        self.move(BALANCE)
        while not self.arrived():
            time.sleep(0.1)
        self.stop_pump()
        self.start_sluice()
        time.sleep(0.5)

    def put_in_tank(self):
        if not self.Tankfull:
            self.move(TANK_POS_INTER_PUT)
            while not self.arrived():
                time.sleep(0.1)
			
            self.move(self.TankPosList[self.CurrentTankPos])

            while not self.arrived():
                time.sleep(0.1)

            self.stop_pump()
            self.start_sluice()
            time.sleep(0.5)
            self.move(TANK_POS_INTER)

            self.CurrentTankPos += 1
            if(self.CurrentTankPos > self.tankSize):
                self.CurrentTankPos = self.tankSize
                self.Tankfull		= True

            self.Tankempty		= False
        else:
            raise RuntimeError('Tank Full')

    def take_in_tank(self):
        if not self.Tankempty:
            self.stop_sluice()
            self.start_pump()

            self.move(self.TankPosListTake[self.CurrentTankPos])

            while not self.arrived():
                time.sleep(0.1)

            self.move(self.TankPosList[self.CurrentTankPos])

            while not self.arrived():
                time.sleep(0.1)

            self.move(self.TankPosListTakeBis[self.CurrentTankPos])

        while not self.arrived():
            time.sleep(0.1)

            time.sleep(0.5)
            self.move(TANK_POS_INTER)

            self.CurrentTankPos -= 1
            if(self.CurrentTankPos < 0):
                self.CurrentTankPos = 0
                self.Tankempty		= True
			
            self.Tankfull = False
        else:
            raise RuntimeError('Tank empty')