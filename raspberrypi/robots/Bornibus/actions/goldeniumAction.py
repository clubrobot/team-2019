#!/usr/bin/python3
#-*- coding: utf-8 -*-

import time
from math import pi,atan2
from common.actions.action import *
from common.funcutils      import *
from common.geogebra import Geogebra
from daughter_cards.wheeledbase import *

MAX_TIME_FOR_GOLDENIUM = 20

class gripperError(Exception) :
    def __init__(self,*args, **kwargs) :
        Exception.__init__(self,*args, kwargs)

class goldenium(Actionnable):
    YELLOW  = 0
    PURPLE  = 1
    def __init__(self, geogebra, daughter_cards, mover, side, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side
        self.mover = mover

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
        self.points["Gold1"] = self.geogebra.get("Gold1"+color)
        self.points["Gold2"] = self.geogebra.get("Gold2"+color)

    def moving(self):
        self.wheeledbase.reset_parameters()
        self.log("GOLDENIUM ACTION : ", "Vers goldenium")
        self.mover.goto(*self.points["Gold1"], theta=pi)

    def realize(self):
        self.wheeledbase.angpos_threshold.set(0.1)
        self.wheeledbase.linpos_threshold.set(1)
        self.wheeledbase.lookaheadbis.set(2)
        # Vers prise goldenium
        self.log("GOLDENIUM ACTION : ", "Prise goldenium")
        self.mover.turnonthespot(pi)
        goldenium = 0
        beginGoldeniumTime = time.time()
        while (goldenium!=1) & (time.time()-beginGoldeniumTime<=MAX_TIME_FOR_GOLDENIUM) :
            if self.endstops.get_ES3():
                    goldenium=1
            else :
                self.gripper.open()
                self.wheeledbase.right_wheel_maxPWM.set(0.2)
                self.wheeledbase.left_wheel_maxPWM.set(0.2)
                self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.points["Gold2"]], direction="forward", lookahead=150, lookaheadbis=1, finalangle=pi)
                try :
                    while not self.wheeledbase.isarrived() :
                        if self.endstops.get_ES2():
                            # Touched left
                            self.log("GOLDENIUM ACTION : ", "gripper touched left")
                            self.points["Gold2"] = (self.points["Gold2"][0], self.points["Gold2"][1]-4)
                            self.points["Gold1"] = (self.points["Gold1"][0], self.points["Gold1"][1]-4)
                            print(*self.points["Gold2"])
                            raise gripperError("Grripper left touched")
                        
                        elif self.endstops.get_ES1():
                            # Touched right
                            self.log("GOLDENIUM ACTION : ", "gripper touched right")
                            self.points["Gold2"] = (self.points["Gold2"][0], self.points["Gold2"][1]+4)
                            self.points["Gold1"] = (self.points["Gold1"][0], self.points["Gold1"][1]+4)
                            print(*self.points["Gold2"])
                            raise gripperError("Grripper right touched")
                        time.sleep(0.1)
                    raise BaseException("Reached position")
                
                except gripperError as e :
                    self.log("GOLDENIUM ACTION : ", "gripperException :", e)
                    try :
                        self.wheeledbase.right_wheel_maxPWM.set(1)
                        self.wheeledbase.left_wheel_maxPWM.set(1)
                        self.wheeledbase.goto(*self.points["Gold1"], theta=pi, lookaheadbis=1)
                    except :
                        pass

                except BaseException as e :
                    self.log("GOLDENIUM ACTION : ", "BaseException :", e)
                    self.gripper.close()
                    time.sleep(1)
                    self.wheeledbase.reset_parameters()
                    self.wheeledbase.set_velocities(-700, 0)
                    self.log("GOLDENIUM ACTION : ", "Recule")
                    time.sleep(0.5)
                    self.wheeledbase.set_velocities(0, 0)


        self.log("GOLDENIUM ACTION : ", "fin")

        if self.endstops.get_ES3():
            self.log("GOLDENIUM ACTION : ", "Goldenium pris : +24 PTS")
            self.display.addPoints(20)
    
    def before(self):
        pass

    def after(self):
        pass

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'goldeniumAction')