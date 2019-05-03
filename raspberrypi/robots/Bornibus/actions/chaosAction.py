#!/usr/bin/python3
#-*- coding: utf-8 -*-

import time
from math import pi
from common.actions.action import *
from common.funcutils      import *
from common.geogebra import Geogebra
from daughter_cards.wheeledbase import *

class chaos(Actionnable):
    YELLOW  = 0
    PURPLE  = 1
    def __init__(self, geogebra, daughter_cards, side, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side

        self.wheeledbase    = daughter_cards['wheeledbase']
        self.display        = daughter_cards['display']
        self.gripper        = daughter_cards['gripper']
        self.pushers        = daughter_cards['pushers']

        if self.side == self.YELLOW:
            color = "Y"
        else:
            color = "P"

        # action Points
        self.points = dict()
        self.points["Gold3"] = self.geogebra.get("Gold3"+color)
        self.points["Gold4"] = self.geogebra.get("Gold4"+color)
        self.points["Gold5"] = self.geogebra.get("Gold5"+color)


    def moving(self):
        self.wheeledbase.max_linvel.set(700)
        self.wheeledbase.max_linacc.set(500.0)
        self.wheeledbase.max_lindec.set(700.0)
        self.wheeledbase.max_angvel.set(10)
        self.wheeledbase.lookahead.set(150.0)
        self.wheeledbase.right_wheel_maxPWM.set(1)
        self.wheeledbase.left_wheel_maxPWM.set(1)
        pass

    def realize(self):
        pass

    def before(self):
        pass

    def after(self):
        pass

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'chaosAction')