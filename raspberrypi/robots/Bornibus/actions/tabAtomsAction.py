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
    def __init__(self, geogebra, daughter_cards, mover, side, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side
        self.mover = mover
        self.wheeledbase    = daughter_cards['wheeledbase']
        self.display        = daughter_cards['display']
        self.pushers        = daughter_cards['pushers']
        self.master         = daughter_cards["master"]
        if self.side == self.YELLOW:
            color = "Y"
        else:
            color = "P"

        # action Points
        self.points = dict()
        
        self.points["Tab1"] = self.geogebra.get("Tab1"+color)
        self.points["Tab2"] = self.geogebra.get("Tab2"+color)

    def moving(self):
        self.wheeledbase.reset_parameters()
        self.wheeledbase.linpos_threshold.set(10)
        self.wheeledbase.angpos_threshold.set(0.1)
    
        # Vers palets
        self.log("TABATOMS ACTION : ", "Vers palets")

        self.mover.goto(*self.points["Tab2"], safe_mode=True, direction='forward')
        self.display.sleep()
        if self.master.is_active():
            while not self.master.get_ressource("passage"):
                time.sleep(0.3)


    def realize(self):

        if self.side == self.YELLOW:
            self.pushers.down_r()
        if self.side == self.PURPLE:
            self.pushers.down_l()
        
        self.mover.turnonthespot(-pi)

        self.display.happy()
        self.master.release_ressource("passage")
        self.mover.goto(*self.points["Tab1"], safe_mode=True, direction='forward')

        #se tourner pour mettre les palets dans le tableau
        if self.side == self.YELLOW:
            self.mover.turnonthespot(-3*pi/4)
        else:
            self.mover.turnonthespot(3*pi/4)

        self.display.love()
        self.display.addPoints(13)
        

    def before(self):
        pass


    def after(self):
        self.pushers.up_l()
        self.pushers.up_r()

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'tabAtomsAction')