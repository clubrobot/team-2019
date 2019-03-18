#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
from math import pi
from daughter_cards.arm.RobotArm import *
from daughter_cards.arm.ArmPosition import *
import math

class ArmController():
    def __init__(self, arm, name):
        self.TankPosList 		= [TANK_POS_1, TANK_POS_2, TANK_POS_3]
        self.TankPosListTake 	= [TANK_POS_INTER_TAKE_PUCK1, TANK_POS_INTER_TAKE_PUCK2, TANK_POS_INTER_TAKE_PUCK3]
        self.TankPosListTakeBis = [TANK_POS_INTER_TAKE_PUCK1_BIS, TANK_POS_INTER_TAKE_PUCK2_BIS, TANK_POS_INTER_TAKE_PUCK3_BIS]
        
        self.name               = name
        self.logger             = None
        self.arm                = arm
        self.CurrentTankPos     = 0
        self.tankSize		    = 2	# size 3 by default [0, 1, 2]
        self.Tankfull		    = False
        self.Tankempty		    = True

    def set_logger(self, loggger):
        self.logger = loggger

    def arrived(self):
        return self.arm.is_arrived()

    def wait(self, timestep = 0.1, timeout = 200):
        self.logger(self.name, "WAIT...")
        init_time = time.time()
        while not self.arrived():
            time.sleep(timestep)
            if (time.time()-init_time > timeout):
                print("Arm Timeout !")
                return
            
    # private func
    def move(self, PosID):
        self.logger(self.name, "Sending Move command: {}".format(PosID))
        self.arm.move(PosID.x, PosID.y, PosID.phi)

    def start_pump(self):
        self.logger(self.name, "Start Pump")
        self.arm.start_pump()

    def stop_pump(self):
        self.logger(self.name, "Stop Pump")
        self.arm.stop_pump()

    def start_sluice(self):
        self.logger(self.name, "Start Sluice")
        self.arm.start_sluice()

    def stop_sluice(self):
        self.logger(self.name, "Stop Sluice")
        self.arm.stop_sluice()

    def setup_tank_size(self, size):
        self.tankSize = size - 1

    def go_home(self):

        self.move(GLOBAL_POS_INTER)
        self.wait()
        self.logger(self.name, "Arrived to : {}".format(GLOBAL_POS_INTER))

        self.move(HOME)
        self.wait()
        self.logger(self.name, "Arrived to : {}".format(HOME))

        time.sleep(1)

    def take_puck_in_distributor(self):

        self.start_pump()

        self.move(TAKE_PUCK_INTER_BEFORE)
        self.wait()
        self.logger(self.name, "Arrived to : {}".format(TAKE_PUCK_INTER_BEFORE))

        self.move(TAKE_PUCK)
        self.wait()
        self.logger(self.name, "Arrived to : {}".format(TAKE_PUCK))

        time.sleep(1)
        
        self.move(TAKE_PUCK_INTER_AFTER_2)
        self.wait()
        self.logger(self.name, "Arrived to : {}".format(TAKE_PUCK_INTER_AFTER_2))

    def put_in_balance(self):
        self.move(BALANCE)
        self.wait()
        self.logger(self.name, "Arrived to : {}".format(BALANCE))

        time.sleep(0.5)

        self.stop_pump()
        self.start_sluice()

        time.sleep(0.5)

    def put_in_tank(self):
        if not self.Tankfull:
            self.move(TANK_POS_INTER_PUT)
            self.wait()
            self.logger(self.name, "Arrived to : {}".format(TANK_POS_INTER_PUT))

            self.move(self.TankPosList[self.CurrentTankPos])
            self.wait()
            self.logger(self.name, "Arrived to : {}".format(self.TankPosList[self.CurrentTankPos]))

            time.sleep(1)
            self.stop_pump()
            self.start_sluice()
            time.sleep(1)
            
            self.move(TANK_POS_INTER)
            self.wait()
            self.logger(self.name, "Arrived to : {}".format(TANK_POS_INTER))
            
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
            self.wait()
            self.logger(self.name, "Arrived to : {}".format(self.TankPosListTake[self.CurrentTankPos]))

            self.move(self.TankPosList[self.CurrentTankPos])
            self.wait()
            self.logger(self.name, "Arrived to : {}".format(self.TankPosList[self.CurrentTankPos]))

            self.move(self.TankPosListTakeBis[self.CurrentTankPos])
            self.wait()
            self.logger(self.name, "Arrived to : {}".format(self.TankPosListTakeBis[self.CurrentTankPos]))

            time.sleep(0.5)
            self.move(TANK_POS_INTER_TAKE)
            self.wait()
            self.logger(self.name, "Arrived to : {}".format(TANK_POS_INTER_TAKE))

            self.CurrentTankPos -= 1
            if(self.CurrentTankPos < 0):
                self.CurrentTankPos = 0
                self.Tankempty		= True
			
            self.Tankfull = False
        else:
            raise RuntimeError('Tank empty')