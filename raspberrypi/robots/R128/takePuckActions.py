#!/usr/bin/python3
#-*- coding: utf-8 -*-

import time
from math import pi
from common.actions.action import *
from common.funcutils      import *
from daughter_cards.wheeledbase import *
from daughter_cards.arm.ArmPosition import *
from daughter_cards.arm.puckUtils import *
class TakePuckSync(Actionnable):
    YELLOW  = 0
    PURPLE  = 1

    def __init__(self, geogebra, daughter_cards, side, distrib_pos, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side
        self.distrib_pos    = distrib_pos

        if self.side == self.YELLOW:
            self.arm1       = daughter_cards['armFront']
            self.arm2       = daughter_cards['armBack']
        else:
            self.arm1       = daughter_cards['armBack']
            self.arm2        = daughter_cards['armFront']
            
        self.wheeledbase    = daughter_cards['wheeledbase']
        # action Points
        self.point          = self.geogebra.get('Distrib{}_{}'.format(self.side,self.distrib_pos))
        self.actionPoint    = ActPoint(self.point, pi/2)

        self.TankPos        = [TAKE_TANK_PUCK1, TAKE_TANK_PUCK2, TAKE_TANK_PUCK3]

        self.handeledPuck   = None

    def realize(self):
        self.arm1.move(PREPARE_TAKING_POS_STATIC)
        self.arm2.move(PREPARE_TAKING_POS_STATIC)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived):
            time.sleep(0.1)

        self.arm1.move(TAKE_PUCK_STATIC)
        self.arm2.move(TAKE_PUCK_STATIC)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived):
            time.sleep(0.1)
    
        time.sleep(1)

        self.arm1.move(TAKE_PUCK_INTER_AFTER_STATIC)
        self.arm2.move(TAKE_PUCK_INTER_AFTER_STATIC)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived):
            time.sleep(0.1)

    def before(self):
        self.arm1.start_pump()
        self.arm2.start_pump()

        self.arm1.move(PREPARE_TAKING_POS_ROAD)
        self.arm2.move(PREPARE_TAKING_POS_ROAD)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived):
            time.sleep(0.1)

    def after(self):
        self.arm1.move(TANK_POS_INTER)
        self.arm2.move(TANK_POS_INTER)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived):
            time.sleep(0.1)

        self.arm1.move(self.TankPos[self.arm1.tank.index()])
        self.arm2.move(self.TankPos[self.arm2.tank.index()])
        while not (self.arm1.is_arrived() and self.arm2.is_arrived):
            time.sleep(0.1)

        self.arm1.tank.put_puck(GreenPuck)
        self.arm2.tank.put_puck(GreenPuck)

        self.arm1.stop_pump()
        self.arm2.stop_pump()

        time.sleep(0.5)
        self.arm1.go_home()
        self.arm2.go_home()
        while not (self.arm1.is_arrived() and self.arm2.is_arrived):
            time.sleep(0.1)

    #override
    def getAction(self):
        return Action(self.actionPoint, lambda : self.realize(), self.before, self.after, 'TakePuck')

class TakePuckSingle(Actionnable):
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
        
        self.wheeledbase    = daughter_cards['wheeledbase']
        # action Points
        self.point          = self.geogebra.get('Balance{}'.format(self.side))
        self.actionPoint    = ActPoint(self.point, pi/2)

        self.handeledPuck   = None

    def realize(self):
        pass
    
    def before(self):
        pass

    def after(self):
        pass

    #override
    def getAction(self):
        return Action(self.actionPoint, lambda : self.realize(), self.before, self.after, 'TakePuck')