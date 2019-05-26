#!/usr/bin/python3
#-*- coding: utf-8 -*-

import time
from math import pi
from common.actions.action import *
from common.funcutils      import *
from daughter_cards.wheeledbase import *
from daughter_cards.arm.ArmPosition import *

class PutRedZone(Actionnable):
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
        #self.point          = self.geogebra.get('Ini{}'.format(self.side))
        # self.actionPoint    = ActPoint(self.point, -pi/2)
        self.path = self.geogebra.getall('PathRed{}_*'.format(self.side))    

        #armPos
        self.beforeTankPos  = [BEFORE_TAKE_TANK_PUCK1, BEFORE_TAKE_TANK_PUCK2, BEFORE_TAKE_TANK_PUCK3]
        self.TankPos        = [TAKE_TANK_PUCK1, TAKE_TANK_PUCK2, TAKE_TANK_PUCK3]
        self.afterTankPos   = [AFTER_TAKE_TANK_PUCK1, AFTER_TAKE_TANK_PUCK2, AFTER_TAKE_TANK_PUCK3]

        self.redPos         = [RED_ZONE1, RED_ZONE2, RED_ZONE3]

        self.handeledPuck1   = None
        self.handeledPuck2   = None

        # Taking error event
        self.takeErrorPuck1  = False
        self.takeErrorPuck2  = False

        # taking state event
        self.arm1TakingState   = Event()
        self.arm2TakingState   = Event()


    def moving(self):
        # Start Path
        if self.side == self.YELLOW:
            self.wheeledbase.purepursuit(self.path, direction='backward')
        else:
            self.wheeledbase.purepursuit(self.path, direction='forward')
        
        self.wheeledbase.wait()

        self.wheeledbase.turnonthespot(-pi/2)
        self.wheeledbase.wait()

    def realize(self):
        
        self.arm1.move(RED_ZONE1)
        self.arm2.move(RED_ZONE1)

        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

        if not self.arm1.get_atmosphere_pressure():
            self.display.addPoints(self.handeledPuck1.getPoints().Tab)
            self.log("REDZONE", "Add {} points".format(self.handeledPuck1.getPoints().Tab))
        if not self.arm2.get_atmosphere_pressure():
            self.display.addPoints(self.handeledPuck2.getPoints().Tab)
            self.log("REDZONE", "Add {} points".format(self.handeledPuck2.getPoints().Tab))

        time.sleep(1)
        self.arm1.stop_pump()
        self.arm2.stop_pump()
        time.sleep(1)

        self.arm1.move(TANK_POS_INTER)
        while not self.arm1.is_arrived():
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

            while not self.arm1TakingState.is_set():
                if not self.arm1.get_atmosphere_pressure():
                    # move after take
                    self.log("REDZONE", "Take Puck {}... OK".format(self.arm2.tank.index()-1))
                    self.arm1.move(self.afterTankPos[self.arm1.tank.index()-1])
                    self.arm1TakingState.set()
                else:
                    # retry with new pos
                    self.log("REDZONE", "Take Puck... Retry")
                    self.new_arm_pos = ArmPos(self.TankPos[self.arm1.tank.index()-1].x - 1 ,self.TankPos[self.arm1.tank.index()-1].y, self.TankPos[self.arm1.tank.index()-1].phi)
                    self.arm1.move(self.new_arm_pos)

                    while not (self.arm1.is_arrived()):
                        time.sleep(0.1)
                    # check presure
                    if not self.arm1.get_atmosphere_pressure():
                        # wait for taking secure
                        time.sleep(0.5)
                        self.log("REDZONE", "Take Puck {}... OK".format(self.arm1.tank.index()-1))
                        self.arm1.move(self.afterTankPos[self.arm1.tank.index()-1])
                        self.arm1TakingState.set()
                    else:
                        # error stop pump
                        self.takeErrorPuck1 = True
                        self.arm1TakingState.set()
                        self.log("REDZONE", "Take Puck... ERROR")

            self.arm1TakingState.clear()

            self.handeledPuck = self.arm1.tank.get_puck()

            while not self.arm1.is_arrived():
                time.sleep(0.1)

            # put the remaning pucks
            self.arm1.move(self.redPos[i+1])
        
            while not self.arm1.is_arrived():
                time.sleep(0.1)

            if not self.arm1.get_atmosphere_pressure():
                self.display.addPoints(self.handeledPuck.getPoints().Tab)
                self.log("REDZONE", "Add {} points".format(self.handeledPuck.getPoints().Tab))
            
            time.sleep(0.5)
            self.arm1.stop_pump()
            time.sleep(0.5)

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
                self.log("REDZONE", "Take Puck {}... OK".format(self.arm2.tank.index()-1))
                self.arm1.move(self.afterTankPos[self.arm1.tank.index()-1])
                self.arm1TakingState.set()
            else:
                # retry with new pos
                self.log("REDZONE", "Take Puck... Retry")
                self.new_arm_pos = ArmPos(self.TankPos[self.arm1.tank.index()-1].x - 1 ,self.TankPos[self.arm1.tank.index()-1].y, self.TankPos[self.arm1.tank.index()-1].phi)
                self.arm1.move(self.new_arm_pos)

                while not (self.arm1.is_arrived()):
                    time.sleep(0.1)
                # check presure
                if not self.arm1.get_atmosphere_pressure():
                    # wait for taking secure
                    time.sleep(0.5)
                    self.log("REDZONE", "Take Puck {}... OK".format(self.arm1.tank.index()-1))
                    self.arm1.move(self.afterTankPos[self.arm1.tank.index()-1])
                    self.arm1TakingState.set()
                else:
                    # error stop pump
                    self.takeErrorPuck1 = True
                    self.arm1TakingState.set()
                    self.log("REDZONE", "Take Puck... ERROR")
            
            if not self.arm2.get_atmosphere_pressure():
                # move after take
                self.log("REDZONE", "Take Puck {}... OK".format(self.arm2.tank.index()-1))
                self.arm2.move(self.afterTankPos[self.arm2.tank.index()-1])
                self.arm2TakingState.set()
            else:
                # retry with new pos
                self.log("REDZONE", "Take Puck... Retry")
                self.new_arm_pos = ArmPos(self.TankPos[self.arm2.tank.index()-1].x - 1 ,self.TankPos[self.arm2.tank.index()-1].y, self.TankPos[self.arm2.tank.index()-1].phi)
                self.arm2.move(self.new_arm_pos)

                while not (self.arm2.is_arrived()):
                    time.sleep(0.1)
                # check presure
                if not self.arm2.get_atmosphere_pressure():
                    # wait for taking secure
                    time.sleep(0.5)
                    self.log("REDZONE", "Take Puck {}... OK".format(self.arm2.tank.index()-1))
                    self.arm2.move(self.afterTankPos[self.arm2.tank.index()-1])
                    self.arm2TakingState.set()
                else:
                    # error stop pump
                    self.takeErrorPuck2 = True
                    self.arm2TakingState.set()
                    self.log("REDZONE", "Take Puck... ERROR")

        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

        self.handeledPuck1 = self.arm1.tank.get_puck()
        self.handeledPuck2 = self.arm2.tank.get_puck()

        self.arm1TakingState.clear()
        self.arm2TakingState.clear()


    def after(self):
        self.arm1.move(TANK_POS_INTER)
        while not self.arm1.is_arrived():
            time.sleep(0.1)

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'PutRedZone')