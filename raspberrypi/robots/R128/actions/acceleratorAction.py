#!/usr/bin/python3
#-*- coding: utf-8 -*-

import time
from math import pi
from common.actions.action import *
from common.funcutils      import *
from daughter_cards.arm.ArmPosition import *

class PutAccelerator(Actionnable):
    YELLOW  = 0
    PURPLE  = 1

    def __init__(self, geogebra, daughter_cards, side, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side

        if self.side == self.YELLOW:
            self.arm1        = daughter_cards['armBack']
            self.arm2        = daughter_cards['armFront']
        else:
            self.arm1        = daughter_cards['armFront']
            self.arm2        = daughter_cards['armBack']

        self.wheeledbase    = daughter_cards['wheeledbase']
        self.display        = daughter_cards['display']

        # action Points
        self.path          = self.geogebra.getall('PathAccel{}_*'.format(self.side))

        #armPos
        self.beforeTankPos  = [BEFORE_TAKE_TANK_PUCK1, BEFORE_TAKE_TANK_PUCK2, BEFORE_TAKE_TANK_PUCK3]
        self.TankPos        = [TAKE_TANK_PUCK1, TAKE_TANK_PUCK2, TAKE_TANK_PUCK3]
        self.afterTankPos   = [AFTER_TAKE_TANK_PUCK1, AFTER_TAKE_TANK_PUCK2, AFTER_TAKE_TANK_PUCK3]

        self.handeledPuck1   = None
        self.handeledPuck2   = None

        # Taking error event
        self.takeErrorPuck1  = False
        self.takeErrorPuck2  = False

        # taking state event
        self.arm1TakingState   = Event()
        self.arm2TakingState   = Event()

    def moving(self):
        if self.side == self.YELLOW:
            self.wheeledbase.purepursuit(self.path, direction='backward')
            while not self.wheeledbase.isarrived():
                time.sleep(0.1)
        else:
            self.wheeledbase.purepursuit(self.path, direction='forward')
            while not self.wheeledbase.isarrived():
                time.sleep(0.1)

        self.wheeledbase.turnonthespot(-pi)
        while not self.wheeledbase.isarrived():
            time.sleep(0.1)

        self.wheeledbase.set_velocities(75,0)
        time.sleep(3)

        self.wheeledbase.goto_delta(-85, 0)
        while not self.wheeledbase.isarrived():
            time.sleep(0.1)
        
        self.wheeledbase.turnonthespot(-pi/2)
        while not self.wheeledbase.isarrived():
            time.sleep(0.1)

    def realize(self):
        # put the first handled puck
        self.arm1.move(ACCELERATOR_BEFORE_ARM1)
        self.arm2.move(ACCELERATOR_BEFORE_ARM2)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

        self.arm1.move(ACCELERATOR_ARM1)
        self.arm2.move(ACCELERATOR_ARM2)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

        if not self.arm1.get_atmosphere_pressure():
            self.display.addPoints(self.handeledPuck1.getPoints().Tab)
            self.log("ACCELERATOR", "Add {} points".format(self.handeledPuck1.getPoints().Accelerator))
        if not self.arm2.get_atmosphere_pressure():
            self.display.addPoints(self.handeledPuck2.getPoints().Tab)
            self.log("ACCELERATOR", "Add {} points".format(self.handeledPuck2.getPoints().Accelerator))

        time.sleep(0.5)
        self.arm1.stop_pump()
        self.arm2.stop_pump()
        time.sleep(0.5)

        self.arm1.move(ACCELERATOR_AFTER_ARM1)
        self.arm1.move(ACCELERATOR_AFTER_ARM2)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

        self.arm1.move(TANK_POS_INTER)
        self.arm2.move(TANK_POS_INTER)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

        self.remainingPuck = self.arm1.tank.index()

        for i in range(0,self.remainingPuck):
            self.arm1.start_pump()
            self.arm1.move(self.beforeTankPos[self.arm1.tank.index()-1])

            while not self.arm1.is_arrived():
                time.sleep(0.1)

            self.arm1.move(self.TankPos[self.arm1.tank.index()-1])

            while not self.arm1.is_arrived():
                time.sleep(0.1)
    
            self.arm1.move(self.afterTankPos[self.arm1.tank.index()-1])
            self.handeledPuck = self.arm1.tank.get_puck()
            self.display.addPoints(self.handeledPuck.getPoints().Accelerator)
            self.log("ACCELERATOR", "Add {} points".format(self.handeledPuck.getPoints().Accelerator))
            while not self.arm1.is_arrived():
                time.sleep(0.1)

            self.arm1.move(TANK_POS_INTER_150)
            while not self.arm1.is_arrived():
                time.sleep(0.1)

            self.arm1.move(ACCELERATOR_BEFORE_ARM1)
            while not self.arm1.is_arrived():
                time.sleep(0.1)

            self.arm1.move(ACCELERATOR_ARM1)
            while not self.arm1.is_arrived():
                time.sleep(0.1)

            if not self.arm1.get_atmosphere_pressure():
                self.display.addPoints(self.handeledPuck1.getPoints().Tab)
                self.log("ACCELERATOR", "Add {} points".format(self.handeledPuck1.getPoints().Accelerator))

            time.sleep(0.5)
            self.arm1.stop_pump()
            time.sleep(0.5)

            self.arm1.move(ACCELERATOR_AFTER_ARM1)
            while not self.arm1.is_arrived():
                time.sleep(0.1)

            self.arm1.move(TANK_POS_INTER)
            while not self.arm1.is_arrived():
                time.sleep(0.1)

    def before(self):
        self.arm1.start_pump()
        self.arm2.start_pump()

        self.arm1.move(self.beforeTankPos[self.arm1.tank.index()-1])
        self.arm2.move(self.beforeTankPos[self.arm2.tank.index()-1])
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

        self.arm1.move(self.TankPos[self.arm1.tank.index()-1])
        self.arm2.move(self.TankPos[self.arm2.tank.index()-1])
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

# while not the taking sequence is end
        while not (self.arm1TakingState.is_set() and self.arm2TakingState.is_set()):
            if not self.arm1.get_atmosphere_pressure():
                # move after take
                self.log("ACCELERATOR", "Take Puck {}... OK".format(self.arm2.tank.index()-1))
                self.arm1.move(self.afterTankPos[self.arm1.tank.index()-1])
                self.arm1TakingState.set()
            else:
                # retry with new pos
                self.log("ACCELERATOR", "Take Puck... Retry")
                self.new_arm_pos = ArmPos(self.TankPos[self.arm1.tank.index()-1].x - 1 ,self.TankPos[self.arm1.tank.index()-1].y, self.TankPos[self.arm1.tank.index()-1].phi)
                self.arm1.move(self.new_arm_pos)

                while not (self.arm1.is_arrived()):
                    time.sleep(0.1)
                # check presure
                if not self.arm1.get_atmosphere_pressure():
                    # wait for taking secure
                    time.sleep(0.5)
                    self.log("ACCELERATOR", "Take Puck {}... OK".format(self.arm1.tank.index()-1))
                    self.arm1.move(self.afterTankPos[self.arm1.tank.index()-1])
                    self.arm1TakingState.set()
                else:
                    # error stop pump
                    self.takeErrorPuck1 = True
                    self.arm1TakingState.set()
                    self.log("ACCELERATOR", "Take Puck... ERROR")
            
            if not self.arm2.get_atmosphere_pressure():
                # move after take
                self.log("ACCELERATOR", "Take Puck {}... OK".format(self.arm2.tank.index()-1))
                self.arm2.move(self.afterTankPos[self.arm2.tank.index()-1])
                self.arm2TakingState.set()
            else:
                # retry with new pos
                self.log("ACCELERATOR", "Take Puck... Retry")
                self.new_arm_pos = ArmPos(self.TankPos[self.arm2.tank.index()-1].x - 1 ,self.TankPos[self.arm2.tank.index()-1].y, self.TankPos[self.arm2.tank.index()-1].phi)
                self.arm2.move(self.new_arm_pos)

                while not (self.arm2.is_arrived()):
                    time.sleep(0.1)
                # check presure
                if not self.arm2.get_atmosphere_pressure():
                    # wait for taking secure
                    time.sleep(0.5)
                    self.log("ACCELERATOR", "Take Puck {}... OK".format(self.arm2.tank.index()-1))
                    self.arm2.move(self.afterTankPos[self.arm2.tank.index()-1])
                    self.arm2TakingState.set()
                else:
                    # error stop pump
                    self.takeErrorPuck2 = True
                    self.arm2TakingState.set()
                    self.log("ACCELERATOR", "Take Puck... ERROR")

        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

        self.handeledPuck1 = self.arm1.tank.get_puck()
        self.handeledPuck2 = self.arm2.tank.get_puck()

        self.arm1TakingState.clear()
        self.arm2TakingState.clear()

        self.arm1.move(TANK_POS_INTER)
        self.arm2.move(TANK_POS_INTER)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

    def after(self):
        self.arm1.move(GLOBAL_POS_INTER)
        while not self.arm1.is_arrived():
            time.sleep(0.1)

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'accelerator')
