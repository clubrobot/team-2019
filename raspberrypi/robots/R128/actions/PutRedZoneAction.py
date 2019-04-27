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
            self.arm        = daughter_cards['armBack']
        else:
            self.arm        = daughter_cards['armFront']
        
        self.display        = daughter_cards['display']
        # action Points
        self.point          = self.geogebra.get('Ini{}'.format(self.side))
        self.actionPoint    = ActPoint(self.point, -pi/2)

        #armPos
        self.beforeTankPos  = [BEFORE_TAKE_TANK_PUCK1, BEFORE_TAKE_TANK_PUCK2, BEFORE_TAKE_TANK_PUCK3]
        self.TankPos        = [TAKE_TANK_PUCK1, TAKE_TANK_PUCK2, TAKE_TANK_PUCK3]
        self.afterTankPos   = [AFTER_TAKE_TANK_PUCK1, AFTER_TAKE_TANK_PUCK2, AFTER_TAKE_TANK_PUCK3]

        self.handeledPuck   = None

    def realize(self):
        self.arm.move(RED_ZONE)
        
        while not self.arm.is_arrived():
            time.sleep(0.1)
        self.display.addPoints(self.handeledPuck.getPoints().Tab)
        self.log("REDZONE", "Add {} points".format(self.handeledPuck.getPoints().Tab))
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
            self.display.addPoints(self.handeledPuck.getPoints().Tab)
            self.log("REDZONE", "Add {} points".format(self.handeledPuck.getPoints().Tab))
            while not self.arm.is_arrived():
                time.sleep(0.1)

            # put the remaning pucks
            self.arm.move(RED_ZONE)
        
            while not self.arm.is_arrived():
                time.sleep(0.1)
            
            time.sleep(0.5)
            self.arm.stop_pump()

            self.arm.move(TANK_POS_INTER)
            while not self.arm.is_arrived():
                time.sleep(0.1)
    
    def before(self):
        self.arm.start_pump()
        self.arm.move(self.beforeTankPos[self.arm.tank.index()-1])

        while not self.arm.is_arrived():
            time.sleep(0.1)

        time.sleep(0.5)

        self.arm.move(self.TankPos[self.arm.tank.index()-1])

        while not self.arm.is_arrived():
            time.sleep(0.1)
        
        self.arm.move(self.afterTankPos[self.arm.tank.index()-1])
        self.handeledPuck = self.arm.tank.get_puck()
        while not self.arm.is_arrived():
            time.sleep(0.1)


    def after(self):
        self.arm.move(TANK_POS_INTER)
        while not self.arm.is_arrived():
            time.sleep(0.1)

    #override
    def getAction(self):
        return Action(self.actionPoint, lambda : self.realize(), self.before, self.after, 'PutRedZone')