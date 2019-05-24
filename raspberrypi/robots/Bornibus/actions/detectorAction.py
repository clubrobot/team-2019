# !/usr/bin/python3
# -*- coding: utf-8 -*-

import time
from math import pi
from common.actions.action import *
from common.funcutils      import *
from common.geogebra import Geogebra
from daughter_cards.wheeledbase import *


class detector(Actionnable):
    YELLOW = 0
    PURPLE = 1

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
        self.points["Det1"] = self.geogebra.get("Det1"+color)
        self.points["Det2"] = self.geogebra.get("Det2"+color)
        self.points["Det3"] = self.geogebra.get("Det3"+color)
        self.points["Det4"] = self.geogebra.get("Det4"+color)


    def moving(self):
        self.wheeledbase.reset_parameters()
        # self.wheeledbase.max_linvel.set(700)
        # self.wheeledbase.max_angvel.set(10)
        # self.wheeledbase.lookahead.set(150.0)
        # self.wheeledbase.max_linacc.set(500.0)
        # self.wheeledbase.max_lindec.set(500.0)
        # self.wheeledbase.right_wheel_maxPWM.set(1)
        # self.wheeledbase.left_wheel_maxPWM.set(1)

        # Vers l'accélérateur
        self.log("DETECTOR ACTION : ", "Vers l'accelerateur")
        self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.points["Det1"], self.points["Det2"],
                                      self.points["Det3"], self.points["Det4"]], direction="forward")
        self.log("DETECTOR ACTION : ", "PurePursuit lance")
        self.wheeledbase.linpos_threshold.set(20)
        self.wheeledbase.angpos_threshold.set(0.1)
        self.wheeledbase.wait()
        self.log("DETECTOR ACTION : ", "Arrive")

        self.wheeledbase.turnonthespot(0)
        self.wheeledbase.wait()
        self.log("DETECTOR ACTION : ", "Oriente vers le mur")

        self.wheeledbase.set_velocities(-200, 0)
        self.wheeledbase.right_wheel_maxPWM.set(0.2)
        self.wheeledbase.left_wheel_maxPWM.set(0.2)
        time.sleep(2)

        self.log("DETECTOR ACTION : ", "Contre le mur")

        self.wheeledbase.reset_parameters()
        if self.side == self.YELLOW :
            self.wheeledbase.goto_delta(90, self.points["Det4"][1] - self.wheeledbase.get_position()[1] + 30)
            
        if self.side == self.PURPLE :
            self.wheeledbase.goto_delta(90, self.points["Det4"][1] - self.wheeledbase.get_position()[1] - 30)

        self.wheeledbase.wait()

        self.log("DETECTOR ACTION : ", "Prepare pour pousser")

    def realize(self):
        # Pousse le Blueium
        self.log("DETECTOR ACTION : ", "Pousse le Blueium")
        if self.side == self.YELLOW:
            self.arm.deploy()
            self.wheeledbase.turnonthespot(-2*pi/3)
            self.wheeledbase.wait()

        if self.side == self.PURPLE:
            self.wheeledbase.turnonthespot(0)
            time.sleep(0.2)
            self.arm.deploy()
            self.wheeledbase.turnonthespot(-pi/3, direction="trig")
            self.wheeledbase.wait()

        time.sleep(0.5)
        self.arm.up()
        self.display.addPoints(20)
        self.log("DETECTOR ACTION : ", "Atome pousse")
    
    def before(self):
        pass

    def after(self):
        pass

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'detectorAction')
