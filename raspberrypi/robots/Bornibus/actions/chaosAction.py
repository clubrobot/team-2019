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

        self.points["Cha1"] = self.geogebra.get("Cha1"+color)
        self.points["Cha2"] = self.geogebra.get("Cha2"+color)
        self.points["Cha3"] = self.geogebra.get("Cha3"+color)
        self.points["Cha4"] = self.geogebra.get("Cha4"+color)
        self.points["Cha5"] = self.geogebra.get("Cha5"+color)
        self.points["Cha6"] = self.geogebra.get("Cha6"+color)
        self.points["Cha7"] = self.geogebra.get("Cha7"+color)


    def moving(self):
        self.wheeledbase.reset_parameters()
        self.wheeledbase.linpos_threshold.set(10)
        self.wheeledbase.lookaheadbis.set(10)

        if self.side == self.YELLOW:
            self.wheeledbase.goto(*self.points["Cha1"], theta=-pi/6)
        else:
            self.wheeledbase.goto(*self.points["Cha1"], theta=pi/6)
        self.log("CHAOS ACTION : ", "Arrive zone chaos")
        self.wheeledbase.wait()

    def realize(self):
        self.wheeledbase.max_angvel.set(2)
        self.wheeledbase.lookahead.set(300)
        self.wheeledbase.lookaheadbis.set(150)
        self.wheeledbase.linpos_threshold.set(5)
        if self.side == self.YELLOW:
            self.pushers.down_l()
            self.wheeledbase.goto(*self.points["Cha2"], theta=-2*pi/3)
            self.wheeledbase.wait()
            self.pushers.down_r()
            self.wheeledbase.turnonthespot(-pi/2)
            self.wheeledbase.wait()
        else :
            self.pushers.down_r()
            self.wheeledbase.goto(*self.points["Cha2"], theta=2*pi/3)
            self.wheeledbase.wait()
            self.pushers.down_l()
            self.wheeledbase.turnonthespot(pi/2)
            self.wheeledbase.wait()

        self.wheeledbase.max_linvel.set(100)

        if self.side == self.YELLOW:
            self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.points["Cha4"], self.points["Cha5"], self.points["Cha6"]], direction='forward', finalangle=-pi/2)#,lookahead=150, lookaheadbis=150)
        else :
            self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.points["Cha4"], self.points["Cha5"], self.points["Cha6"]], direction='forward', finalangle=pi/2 )#,lookahead=150, lookaheadbis=150)
        self.wheeledbase.wait()
        self.pushers.up_r()
        self.pushers.up_l()
        self.wheeledbase.goto(*self.points["Cha7"], direction='backward')
        

    def before(self):
        pass

    def after(self):
        pass

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'chaosAction')