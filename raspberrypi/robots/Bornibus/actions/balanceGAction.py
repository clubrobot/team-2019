#!/usr/bin/python3
#-*- coding: utf-8 -*-

import time
from math import pi
from common.actions.action import *
from common.funcutils      import *
from common.geogebra import Geogebra
from daughter_cards.wheeledbase import *

class balance(Actionnable):
    YELLOW  = 0
    PURPLE  = 1
    def __init__(self, geogebra, daughter_cards, side, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side

        self.wheeledbase    = daughter_cards['wheeledbase']
        self.display        = daughter_cards['display']
        self.gripper        = daughter_cards['gripper']
        self.endstops       = daughter_cards['endstops']

        if self.side == self.YELLOW:
            color = "Y"
        else:
            color = "P"

        # action Points
        self.points = dict()
        self.points["Bal1"] = self.geogebra.get("Bal1"+color)
        self.points["Bal2"] = self.geogebra.get("Bal2"+color)
        self.points["Bal3"] = self.geogebra.get("Bal3"+color)
        self.points["Bal4"] = self.geogebra.get("Bal4"+color)

    def moving(self):
        self.wheeledbase.reset_parameters()
        # self.wheeledbase.max_linvel.set(700)
        # self.wheeledbase.max_linacc.set(500.0)
        # self.wheeledbase.max_lindec.set(700.0)
        # self.wheeledbase.max_angvel.set(10)
        # self.wheeledbase.lookahead.set(150.0)
        # self.wheeledbase.right_wheel_maxPWM.set(1)
        # self.wheeledbase.left_wheel_maxPWM.set(1)

        # Vers balance
        self.log("BALANCE ACTION :", "Vers la balance")
        try :
            self.wheeledbase.linpos_threshold.set(10)
            self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.points["Bal1"], self.points["Bal2"], self.points["Bal3"], self.points["Bal4"]],
                                direction="backward", lookahead=200, lookaheadbis=120)
            self.wheeledbase.wait()
        except :
            pass
        self.wheeledbase.linpos_threshold.set(3)
        self.wheeledbase.max_linacc.set(300)
        self.wheeledbase.max_lindec.set(300)
        self.wheeledbase.max_linvel.set(400)
        
        # Positionnement pour la balance
        self.log("BALANCE ACTION :", "Positionnement pour la balance")
        #TODO COULEUR

        self.wheeledbase.turnonthespot(pi/2)
        self.wheeledbase.wait()
        self.wheeledbase.right_wheel_maxPWM.set(0.5)
        self.wheeledbase.left_wheel_maxPWM.set(0.5)


        try :
            self.wheeledbase.goto_delta(-200, 0)
            self.wheeledbase.wait()
        except :
            pass
        self.wheeledbase.stop()


        self.wheeledbase.right_wheel_maxPWM.set(1)
        self.wheeledbase.left_wheel_maxPWM.set(1)
        self.wheeledbase.goto_delta(80,0)
        self.wheeledbase.wait()

        self.wheeledbase.turnonthespot(0)
        self.wheeledbase.wait()

        self.wheeledbase.right_wheel_maxPWM.set(0.5)
        self.wheeledbase.left_wheel_maxPWM.set(0.5)

        try :
            self.wheeledbase.goto_delta(300, 0)
            self.wheeledbase.wait()
        except :
            pass
        self.wheeledbase.stop()

        self.wheeledbase.reset_parameters()
        self.wheeledbase.max_linacc.set(500.0)
        self.wheeledbase.max_lindec.set(500.0)

    def realize(self):
        # Depose
        self.log("BALANCE ACTION :", "Depose dans la balance")
        time.sleep(0.5)

        if self.endstops.get_ES3():
            self.display.addPoints(24)
        self.gripper.open()

        time.sleep(0.5)
        self.wheeledbase.goto(*self.points["Bal2"], theta=0)#, lookaheadbis=150)

    def before(self):
        pass

    def after(self):
        self.wheeledbase.stop()

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'balanceGAction')