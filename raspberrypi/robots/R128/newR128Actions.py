#!/usr/bin/python3
#-*- coding: utf-8 -*-

import time
from math import pi
from common.actions.action import *
from common.funcutils      import *
from daughter_cards.wheeledbase import *

class AAAAAA(Actionnable):
    def __init__(self, geogebra, daughter_cards, side, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side
        self.armFront       = daughter_cards['armFront']
        self.wheeledbase    = daughter_cards['wheeledbase']
        self.point          = self.geogebra.get('Balance{}'.format(self.side))
        self.actionPoint    = ActPoint(self.point, pi/2)

    def realize(self):
        pass
    
    def before(self):
        pass

    def after(self):
        pass

    #override
    def getAction(self):
        return Action(self.actionPoint, lambda : self.realize(), self.before, self.after, 'PutBalance')