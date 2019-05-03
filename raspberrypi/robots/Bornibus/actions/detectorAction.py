#!/usr/bin/python3
#-*- coding: utf-8 -*-

import time
from math import pi
from common.actions.action import *
from common.funcutils      import *
from common.geogebra import Geogebra
from daughter_cards.wheeledbase import *

class detector(Actionnable):
    YELLOW  = 0
    PURPLE  = 1
    def __init__(self, geogebra, daughter_cards, side, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side

        self.wheeledbase    = daughter_cards['wheeledbase']
        self.display        = daughter_cards['display']
        self.arm            = daughter_cards['arm']

        if self.side == self.YELLOW:
            color = "Y"
        else:
            color = "P"

        # action Points
        self.points = dict()
        self.points["Dep1"] = self.geogebra.get("Dep1"+color)
        self.points["Dep2"] = self.geogebra.get("Dep2"+color)
        self.points["Dep3"] = self.geogebra.get("Dep3"+color)
        self.points["Gold2"] = self.geogebra.get("Gold2"+color)


    def moving(self):
        self.wheeledbase.max_linvel.set(700)
        self.wheeledbase.max_angvel.set(10)
        self.wheeledbase.lookahead.set(150.0)
        self.wheeledbase.max_linacc.set(500.0)
        self.wheeledbase.max_lindec.set(500.0)
        self.wheeledbase.right_wheel_maxPWM.set(1)
        self.wheeledbase.left_wheel_maxPWM.set(1)

        # Vers l'accélérateur
        self.log("DETECTOR ACTION : ", "Vers l'accélérateur")

        self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.points["Dep1"], self.points["Dep2"],
                                 self.points["Dep3"], self.points["Gold2"]], direction="forward", lookahead=150, lookaheadbis=3)
        self.wheeledbase.wait()
        self.wheeledbase.turnonthespot(0)
        self.wheeledbase.wait()
        self.wheeledbase.right_wheel_maxPWM.set(0.2)
        self.wheeledbase.left_wheel_maxPWM.set(0.2)
        try :
            self.wheeledbase.set_velocities(-200, 0)
        except :
            pass
        time.sleep(2)
        self.wheeledbase.set_velocities(0, 0)
        self.wheeledbase.right_wheel_maxPWM.set(1)
        self.wheeledbase.left_wheel_maxPWM.set(1)

        if self.side == self.YELLOW :
            self.wheeledbase.goto_delta(90, 30)
            
        if self.side == self.PURPLE :
            self.wheeledbase.goto_delta(90, -30)            
        self.wheeledbase.wait()
        self.wheeledbase.lookaheadbis.set(150)

    def realize(self):
        # Prépare le bras
        self.log("DETECTOR ACTION : ", "Prépare le bras")

        if self.side == self.YELLOW:
            self.arm.deploy()
            self.wheeledbase.turnonthespot(-2*pi/3)
            while not self.wheeledbase.isarrived():
                time.sleep(0.1)
        if self.side == self.PURPLE:
            self.wheeledbase.turnonthespot(-3*pi/4)
            self.wheeledbase.wait()
            self.arm.deploy()
            self.wheeledbase.turnonthespot(-pi/3)
            while not self.wheeledbase.isarrived():
                time.sleep(0.1)

        time.sleep(0.5)
        self.arm.up()

        # Pousse le Blueium
        self.log("DETECTOR ACTION : ", "Pouse le Blueium")
        self.wheeledbase.turnonthespot(-pi/2)
        while not self.wheeledbase.isarrived():
            time.sleep(0.1)
        self.arm.up()
        self.display.addPoints(20)
    
    def before(self):
        pass

    def after(self):
        pass

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'detectorAction')