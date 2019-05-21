#!/usr/bin/python3
#-*- coding: utf-8 -*-

import time
from math import pi
from common.actions.action import *
from common.funcutils      import *
from common.geogebra import Geogebra
from daughter_cards.wheeledbase import *

class tabAtoms(Actionnable):
    YELLOW  = 0
    PURPLE  = 1
    def __init__(self, geogebra, daughter_cards, side, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side

        self.wheeledbase    = daughter_cards['wheeledbase']
        self.display        = daughter_cards['display']
        self.pushers        = daughter_cards['pushers']

        if self.side == self.YELLOW:
            color = "Y"
        else:
            color = "P"

        # action Points
        self.points = dict()
        
        self.points["Tab1"] = self.geogebra.get("Tab1"+color)
        self.points["Tab2"] = self.geogebra.get("Tab2"+color)
        self.points["Tab3"] = self.geogebra.get("Tab3"+color)

    def moving(self):
        self.wheeledbase.max_linvel.set(700)
        self.wheeledbase.max_linacc.set(500.0)
        self.wheeledbase.max_lindec.set(700.0)
        self.wheeledbase.max_angvel.set(10)
        self.wheeledbase.lookahead.set(150.0)
        self.wheeledbase.lookahead.set(150.0)
        self.wheeledbase.right_wheel_maxPWM.set(1)
        self.wheeledbase.left_wheel_maxPWM.set(1)
        
    
        # Vers palets
        self.log("TABATOMS ACTION : ", "Vers palets")
        self.wheeledbase.right_wheel_maxPWM.set(1)
        self.wheeledbase.left_wheel_maxPWM.set(1)

        try:
            self.wheeledbase.goto(*self.points["Tab2"])
        except:
            pass

        if self.side == self.YELLOW:
            self.pushers.down_r()
        if self.side == self.PURPLE:
            self.pushers.down_l()
        self.wheeledbase.turnonthespot(-pi)
        self.wheeledbase.wait()

        try:
            self.wheeledbase.goto(*self.points["Tab1"])
        except:
            pass

        if self.side == self.YELLOW:
            self.wheeledbase.turnonthespot(-pi/2)
        else:
            self.wheeledbase.turnonthespot(pi/2)
        self.wheeledbase.wait()

        try:
            self.wheeledbase.goto(*self.points["Tab3"])
        except:
            pass

        self.display.addPoints(13)

    def realize(self):
         # fin
        self.pushers.up_l()
        self.pushers.up_r()
        self.display.stop()

    def before(self):
        pass

    def after(self):
        pass

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'tabAtomsAction')