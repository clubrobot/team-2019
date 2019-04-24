#!/usr/bin/python3
#-*- coding: utf-8 -*-

import time
from math import pi
from common.actions.action import *
from common.funcutils      import *
from daughter_cards.wheeledbase import *
from daughter_cards.arm.ArmPosition import *

class BalanceAfter6(Actionnable):
    YELLOW  = 0
    PURPLE  = 1

    def __init__(self, geogebra, daughter_cards, side, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side

        if self.side == self.YELLOW:
            self.arm        = daughter_cards['armFront']
        else:
            self.arm        = daughter_cards['armBack']

        self.display        = daughter_cards['display']

        # action Points
        self.point          = self.geogebra.get('Balance{}'.format(self.side))
        self.actionPoint    = ActPoint(self.point, pi/2)

        #armPos
        self.beforeTankPos  = [BEFORE_TAKE_TANK_PUCK1, BEFORE_TAKE_TANK_PUCK2, BEFORE_TAKE_TANK_PUCK3]
        self.TankPos        = [TAKE_TANK_PUCK1, TAKE_TANK_PUCK2, TAKE_TANK_PUCK3]
        self.afterTankPos   = [AFTER_TAKE_TANK_PUCK1, AFTER_TAKE_TANK_PUCK2, AFTER_TAKE_TANK_PUCK3]

        self.handeledPuck1   = None
        self.handeledPuck2   = None

    def realize(self):
        # put the first handled puck
        self.arm.move(BALANCE)
        
        while not self.arm.is_arrived():
            time.sleep(0.1)

        self.display.addPoints(self.handeledPuck.getPoints().Balance)
        self.log("BALANCE6", "Add {} points".format(self.handeledPuck.getPoints().Balance))
        time.sleep(0.5)
        self.arm.stop_pump()

        self.arm.move(TANK_POS_INTER)
        while not self.arm.is_arrived():
            time.sleep(0.1)

        self.remainingPuck = self.arm.tank.index()

        for i in range(0,self.remainingPuck):
            self.arm.start_pump()
            self.arm.move(self.beforeTankPos[self.arm.tank.index()-1])

            while not self.arm.is_arrived():
                time.sleep(0.1)

            self.arm.move(self.TankPos[self.arm.tank.index()-1])

            while not self.arm.is_arrived():
                time.sleep(0.1)
    
            self.arm.move(self.afterTankPos[self.arm.tank.index()-1])
            self.handeledPuck = self.arm.tank.get_puck()
            self.display.addPoints(self.handeledPuck.getPoints().Balance)
            self.log("BALANCE6", "Add {} points".format(self.handeledPuck.getPoints().Balance))
            while not self.arm.is_arrived():
                time.sleep(0.1)

            # put the remaning pucks
            self.arm.move(BALANCE)
        
            while not self.arm.is_arrived():
                time.sleep(0.1)

            self.arm.stop_pump()
            time.sleep(0.5)
            if(self.arm.tank.index() > 0):
                self.arm.move(TANK_POS_INTER)
                while not self.arm.is_arrived():
                    time.sleep(0.1)

    def before(self):
        self.arm.start_pump()
        if(self.arm.sucker.index() > 0):
            self.arm.move(GLOBAL_POS_INTER)

            while not self.arm.is_arrived():
                time.sleep(0.1)
            
            self.handeledPuck = self.arm.sucker.get_puck()
            
            
        elif (self.arm.tank.index() > 0):
            self.arm.move(self.beforeTankPos[self.arm.tank.index()-1])

            while not self.arm.is_arrived():
                time.sleep(0.1)

            self.arm.move(self.TankPos[self.arm.tank.index()-1])
            while not self.arm.is_arrived():
                time.sleep(0.1)
    
            self.arm.move(self.afterTankPos[self.arm.tank.index()-1])
            self.handeledPuck = self.arm.tank.get_puck()
            while not self.arm.is_arrived():
                time.sleep(0.1)
        
    def after(self):
        self.arm.move(GLOBAL_POS_INTER)
        while not self.arm.is_arrived():
            time.sleep(0.1)

    #override
    def getAction(self):
        return Action(self.actionPoint, lambda : self.realize(), self.before, self.after, 'PutBalance6')

class BalanceAfter3(Actionnable):
    YELLOW  = 0
    PURPLE  = 1

    def __init__(self, geogebra, daughter_cards, side, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side

        if self.side == self.YELLOW:
            self.arm1       = daughter_cards['armFront']
            self.arm2       = daughter_cards['armBack']
        else:
            self.arm1       = daughter_cards['armBack']
            self.arm2       = daughter_cards['armFront']
        
        self.wheeledbase    = daughter_cards['wheeledbase']
        self.display        = daughter_cards['display']
        
        # action Points
        self.point          = self.geogebra.get('Balance{}'.format(self.side))
        if self.side == self.YELLOW:
            self.actionPoint    = ActPoint(self.point, pi/2)
        else:
            self.actionPoint    = ActPoint(self.point, pi/2)

        #armPos
        self.beforeTankPos  = [BEFORE_TAKE_TANK_PUCK1, BEFORE_TAKE_TANK_PUCK2, BEFORE_TAKE_TANK_PUCK3]
        self.TankPos        = [TAKE_TANK_PUCK1, TAKE_TANK_PUCK2, TAKE_TANK_PUCK3]
        self.afterTankPos   = [AFTER_TAKE_TANK_PUCK1, AFTER_TAKE_TANK_PUCK2, AFTER_TAKE_TANK_PUCK3]

        self.handeledPuck1   = None
        self.handeledPuck2   = None

    def realize(self):
        # put the first handled puck
        self.arm1.move(BALANCE)
        while not self.arm1.is_arrived():
            time.sleep(0.1)

        self.arm1.stop_pump()
        time.sleep(0.5)

        self.display.addPoints(self.handeledPuck1.getPoints().Balance)
        self.log("BALANCE3", "Add {} points".format(self.handeledPuck1.getPoints().Balance))

        self.arm1.move(TANK_POS_INTER)
        while not self.arm1.is_arrived():
            time.sleep(0.1)

        self.wheeledbase.turnonthespot(pi/3)
        while not self.wheeledbase.isarrived():
            time.sleep(0.1)

        # put the first handled puck
        self.arm2.move(BALANCE)
        while not self.arm2.is_arrived():
            time.sleep(0.1)

        time.sleep(0.5)
        self.arm2.stop_pump()

        self.display.addPoints(self.handeledPuck2.getPoints().Balance)
        self.log("BALANCE3", "Add {} points".format(self.handeledPuck2.getPoints().Balance))

        self.arm2.move(TANK_POS_INTER)
        while not self.arm2.is_arrived():
            time.sleep(0.1)
    
    def before(self):
        if(self.arm1.sucker.index() > 0):
            self.arm1.move(GLOBAL_POS_INTER)

            while not self.arm1.is_arrived():
                time.sleep(0.1)
            
            self.handeledPuck1 = self.arm1.sucker.get_puck()

        if(self.arm2.sucker.index() > 0):
            self.arm2.move(GLOBAL_POS_INTER)

            while not self.arm2.is_arrived():
                time.sleep(0.1)
            
            self.handeledPuck2 = self.arm2.sucker.get_puck()
        
    def after(self):
        pass

    #override
    def getAction(self):
        return Action(self.actionPoint, lambda : self.realize(), self.before, self.after, 'PutBalance3')