#!/usr/bin/python3
#-*- coding: utf-8 -*-

import time
from math import pi
from common.actions.action import *
from common.funcutils      import *
from daughter_cards.wheeledbase import *
from daughter_cards.arm.ArmPosition import *


class MovingToLittle(Actionnable):
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
        self.path           = self.geogebra.getall('PathDistrib{}_*'.format(self.side))
        self.actionPoint    = None

        self.handeledPuck   = None

    def realize(self):
        self.wheeledbase.purepursuit(self.path)
        while not self.wheeledbase.isarrived():
            time.sleep(0.1)

        self.wheeledbase.turnonthespot(pi/2)
        while not self.wheeledbase.isarrived():
            time.sleep(0.1)
    
    def before(self):
        pass

    def after(self):
        pass

    #override
    def getAction(self):
        return Action(self.actionPoint, lambda : self.realize(), self.before, self.after, 'MovingToLittle')


class MovingAfterStart(Actionnable):
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
        self.point          = self.geogebra.get('Inter{}'.format(self.side))
        self.actionPoint    = None

        self.handeledPuck   = None

    def realize(self):
        self.wheeledbase.goto(*self.point, pi/2)
    
    def before(self):
        pass

    def after(self):
        pass

    #override
    def getAction(self):
        return Action(self.actionPoint, lambda : self.realize(), self.before, self.after, 'MovingAfterStart')