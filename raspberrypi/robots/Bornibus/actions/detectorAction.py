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

    def __init__(self, geogebra, daughter_cards, mover, side, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side

        self.wheeledbase    = daughter_cards['wheeledbase']
        self.display        = daughter_cards['display']
        self.arm            = daughter_cards['arm']

        self.mover = mover

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

        # Vers l'accélérateur
        self.log("DETECTOR ACTION : ", "Vers l'accelerateur")
        self.wheeledbase.linpos_threshold.set(20)
        self.wheeledbase.angpos_threshold.set(0.1)
        self.mover.purepursuit([self.wheeledbase.get_position()[:2], self.points["Det1"], self.points["Det2"],
                                      self.points["Det3"], self.points["Det4"]], direction="forward")
        self.log("DETECTOR ACTION : ", "PurePursuit lance")
        #self.wheeledbase.wait()
        self.log("DETECTOR ACTION : ", "Arrive")

        self.mover.turnonthespot(0)
        self.log("DETECTOR ACTION : ", "Oriente vers le mur")
        self.display.sleep()

        self.wheeledbase.set_velocities(-200, 0)
        self.wheeledbase.right_wheel_maxPWM.set(0.2)
        self.wheeledbase.left_wheel_maxPWM.set(0.2)

        try:
            self.wheeledbase.wait()
        except:
            pass

        self.log("DETECTOR ACTION : ", "Contre le mur")
        self.display.surprised()
        self.wheeledbase.reset_parameters()
        if self.side == self.YELLOW :
            self.wheeledbase.goto_delta(90, self.points["Det4"][1] - self.wheeledbase.get_position()[1] + 30)
            
        if self.side == self.PURPLE :
            self.wheeledbase.goto_delta(90, self.points["Det4"][1] - self.wheeledbase.get_position()[1] - 30)
        try:
            self.wheeledbase.wait()
            # TODO Géré ??
        except:
            pass
        self.log("DETECTOR ACTION : ", "Prepare pour pousser")

    def realize(self):
        # Pousse le Blueium
        self.log("DETECTOR ACTION : ", "Pousse le Blueium")
        self.arm.deploy()
        if self.side == self.YELLOW:
            self.mover.turnonthespot_dir(-2*pi/3, direction="clock", try_limit=1)

        if self.side == self.PURPLE:
            self.mover.turnonthespot_dir(pi , direction="clock", try_limit=1)
            self.mover.turnonthespot_dir(-pi/3, direction="trig", try_limit=1)

        self.display.sick()
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
