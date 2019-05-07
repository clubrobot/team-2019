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
        self.points["Gold3"] = self.geogebra.get("Gold3"+color)
        self.points["Gold5"] = self.geogebra.get("Gold5"+color)
        self.points["Gold6"] = self.geogebra.get("Gold6"+color)

    def moving(self):
        self.wheeledbase.max_linvel.set(700)
        self.wheeledbase.max_linacc.set(500.0)
        self.wheeledbase.max_lindec.set(700.0)
        self.wheeledbase.max_angvel.set(10)
        self.wheeledbase.lookahead.set(150.0)
        self.wheeledbase.right_wheel_maxPWM.set(1)
        self.wheeledbase.left_wheel_maxPWM.set(1)

        # Vers balance
        self.log("BALANCE ACTION :", "Vers la balance")
        self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.points["Gold5"]],
                                direction="backward", finalangle=0, lookahead=150, lookaheadbis=10)
        self.wheeledbase.wait()

        self.wheeledbase.max_linacc.set(300)
        self.wheeledbase.max_lindec.set(300)
        self.wheeledbase.max_linvel.set(400)
        
        self.wheeledbase.turnonthespot(0)
        self.wheeledbase.wait()

        # Positionnement pour la balance
        self.log("BALANCE ACTION :", "Positionnement pour la balance")
        try:
            self.wheeledbase.goto(*self.points["Gold6"], theta=0, lookaheadbis=1)
        except:
            pass
            
        self.wheeledbase.right_wheel_maxPWM.set(0.5)
        self.wheeledbase.left_wheel_maxPWM.set(0.5)

        try :
            self.wheeledbase.set_velocities(200, 0)
            time.sleep(2)
        except :
            pass
        self.wheeledbase.set_velocities(0, 0)

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

    def before(self):
        pass

    def after(self):
        pass

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'balanceGAction')