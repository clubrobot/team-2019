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
        self.points["Gold5"] = self.geogebra.get("Gold5"+color)
        self.points["Pal1"] = self.geogebra.get("Pal1"+color)
        self.points["Pal2"] = self.geogebra.get("Pal2"+color)
        self.points["Pal3"] = self.geogebra.get("Pal3"+color)
        self.points["Pal4"] = self.geogebra.get("Pal4"+color)
        self.points["Pal5"] = self.geogebra.get("Pal5"+color)
        self.points["Pal6"] = self.geogebra.get("Pal6"+color)


    def moving(self):
        self.wheeledbase.max_linvel.set(700)
        self.wheeledbase.max_linacc.set(500.0)
        self.wheeledbase.max_lindec.set(700.0)
        self.wheeledbase.max_angvel.set(10)
        self.wheeledbase.lookahead.set(150.0)
        self.wheeledbase.lookahead.set(150.0)
        self.wheeledbase.right_wheel_maxPWM.set(1)
        self.wheeledbase.left_wheel_maxPWM.set(1)
        
        # Premier palet
 #       if self.side == self.YELLOW:
 #           self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.points["Gold5"], self.points["Pal1"],
 #                                self.points["Pal6"]], direction="backward", finalangle=-pi/4, lookahead=150)
 #       if self.side == self.PURPLE:
 #           self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.points["Gold5"], self.points["Pal1"],
 #                                    self.points["Pal6"]], direction="backward", finalangle=pi/4, lookahead=150)
 #       self.wheeledbase.wait()
       
        # Vers palets
        self.log("TABATOMS ACTION : ", "Vers palets")
        self.wheeledbase.right_wheel_maxPWM.set(1)
        self.wheeledbase.left_wheel_maxPWM.set(1)

        #self.wheeledbase.goto("P")


        if self.side == self.YELLOW:
            self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.points["Gold5"],
                                     self.points["Pal4"]], direction="forward", finalangle=-3*pi/4, lookahead=150, lookaheadbis=150)
        if self.side == self.PURPLE:
            self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.points["Gold5"],
                                     self.points["Pal4"]], direction="forward", finalangle=3*pi/4, lookahead=150)

        self.wheeledbase.wait()

#        if self.side == self.YELLOW:
#            self.wheeledbase.turnonthespot(-3*pi/4)
#        else:
#            self.wheeledbase.turnonthespot(3 * pi / 4)
#        self.wheeledbase.wait()

        # Vers zone
        self.log("TABATOMS ACTION : ", "Vers zone")
#        self.pushers.down()
        self.pushers.down_l()
        self.pushers.down_r()
        if self.side == self.YELLOW:
            self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.points["Pal5"], self.points["Pal6"], self.points["Pal7"]],
                                    direction="forward", lookahead=50, lookaheadbis=10, finalangle=-pi/2)
        if self.side == self.PURPLE:
            self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.points["Pal5"], self.points["Pal6"], self.points["Pal7"]],
                                direction="forward", lookahead=50, lookaheadbis=10, finalangle=pi/2)
        self.wheeledbase.wait()

        self.display.addPoints(13)

    def realize(self):
         # fin
        self.pushers.up_l()
        self.pushers.up_r()
        self.wheeledbase.stop()
        self.display.stop()

    def before(self):
        pass

    def after(self):
        pass

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'tabAtomsAction')