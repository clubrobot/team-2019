#!/usr/bin/python3
#-*- coding: utf-8 -*-

import time
from math import pi
from common.actions.action import *
from common.funcutils      import *
from common.geogebra import Geogebra
from daughter_cards.wheeledbase import *

class gripperError(Exception) :
    def __init__(self,*args, **kwargs) :
        Exception.__init__(self,*args, kwargs)

class goldenium(Actionnable):
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
        self.points["Gold4"] = self.geogebra.get("Gold4"+color)

    def moving(self):
        self.wheeledbase.max_linvel.set(700)
        self.wheeledbase.max_angvel.set(10)
        self.wheeledbase.lookahead.set(150.0)
        self.wheeledbase.max_linacc.set(500.0)
        self.wheeledbase.max_lindec.set(500.0)
        self.wheeledbase.right_wheel_maxPWM.set(1)
        self.wheeledbase.left_wheel_maxPWM.set(1)

        # Vers goldenium
        self.log("GOLDENIUM ACTION : ", "Vers goldenium")
        self.wheeledbase.goto(*self.points["Gold3"], theta=pi)
        while not self.wheeledbase.isarrived():
            print(self.wheeledbase.get_position())
            time.sleep(0.1)

    def realize(self):
        # Vers prise goldenium
        self.log("GOLDENIUM ACTION : ", "Prise goldenium")
        self.wheeledbase.right_wheel_maxPWM.set(0.2)
        self.wheeledbase.left_wheel_maxPWM.set(0.2)

        goldenium = 0

        while goldenium != 1 :
            self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.points["Gold4"]], direction="forward", lookaheadbis=1, finalangle=pi)
            try :
                while not self.wheeledbase.isarrived() :
                    if self.endstops.get_ES2():
                        # Touched left
                        self.log("GOLDENIUM ACTION : ", "gripper touched left")
                        self.points["Gold4"] = (self.points["Gold4"][0], self.points["Gold4"][1]-10)
                        self.points["Gold3"] = (self.points["Gold3"][0], self.points["Gold3"][1]-10)
                        print(*self.points["Gold4"])
                        raise gripperError("Grripper left touched")
                    
                    elif self.endstops.get_ES1():
                        # Touched right
                        self.log("GOLDENIUM ACTION : ", "gripper touched right")
                        self.points["Gold4"] = (self.points["Gold4"][0], self.points["Gold4"][1]+10)
                        self.points["Gold3"] = (self.points["Gold3"][0], self.points["Gold3"][1]+10)
                        print(*self.points["Gold4"])
                        raise gripperError("Grripper right touched")
                    time.sleep(0.1)
                goldenium = 1
            
            except gripperError as e :
                self.log("GOLDENIUM ACTION : ", "gripperException :", e)
                try :
                    self.wheeledbase.goto(*self.points["Gold3"], theta=pi, lookaheadbis=1)
                except :
                    pass
            except BaseException as e :
                self.log("GOLDENIUM ACTION : ", "BaseException :", e)
                self.gripper.close()
                time.sleep(1)
                if self.endstops.get_ES3():
                    goldenium=1
                else :
                    pass
        
        self.wheeledbase.right_wheel_maxPWM.set(1)
        self.wheeledbase.left_wheel_maxPWM.set(1)
        self.wheeledbase.lookaheadbis.set(150)
        self.log("GOLDENIUM ACTION", "goldenium pris")

        try :
            self.wheeledbase.goto_delta(-100, 0)
            self.wheeledbase.wait()
        except :
            self.wheeledbase.set_velocities(-700, 0)
            time.sleep(1)
            self.wheeledbase.set_velocities(0, 0)
            try :
                self.wheeledbase.goto(*self.points["Gold3"], theta=pi, lookaheadbis=150)
            except :
                # CODER LA STRAT DE RECOMMENCER SI PAS ES3 OK !!!
                pass


        if self.endstops.get_ES3():
            self.display.addPoints(20)
    
    def before(self):
        pass

    def after(self):
        pass

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'goldeniumAction')