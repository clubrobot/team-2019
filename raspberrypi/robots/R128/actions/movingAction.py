#!/usr/bin/python3
#-*- coding: utf-8 -*-

import time
from math import pi
from common.actions.action import *
from common.funcutils      import *
from daughter_cards.wheeledbase import *
from daughter_cards.arm.ArmPosition import *

class MovingAfterLittle(Actionnable):
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
        self.path.reverse()

    def moving(self):
        self.wheeledbase.turnonthespot(-pi)
        while not self.wheeledbase.isarrived():
            time.sleep(0.1)

        if self.side == self.YELLOW:
            self.wheeledbase.purepursuit(self.path, direction='backward')
            while not self.wheeledbase.isarrived():
                time.sleep(0.1)
        else:
            self.wheeledbase.purepursuit(self.path, direction='forward')
            while not self.wheeledbase.isarrived():
                time.sleep(0.1)
    
    def realize(self):
        pass

    def before(self):
        pass

    def after(self):
        pass

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'MovingToLittle')
