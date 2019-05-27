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
    def __init__(self, geogebra, daughter_cards, mover, side, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side

        self.wheeledbase    = daughter_cards['wheeledbase']
        self.display        = daughter_cards['display']
        self.gripper        = daughter_cards['gripper']
        self.endstops       = daughter_cards['endstops']
        self.pushers        = daughter_cards['pushers']
        self.master         = daughter_cards['master']

        self.mover = mover

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
        # Waiting for mutex

        if self.master.is_active():
            while not self.master.get_ressource("balance"):
                time.sleep(0.4)
        # Vers balance
        self.log("BALANCE ACTION :", "Vers la balance")
        self.wheeledbase.linpos_threshold.set(10)
        self.mover.purepursuit([self.wheeledbase.get_position()[:2], self.points["Bal1"], self.points["Bal2"], self.points["Bal3"]],
                                direction="forward", lookahead=200, lookaheadbis=120)
        self.wheeledbase.linpos_threshold.set(3)
        self.wheeledbase.max_linacc.set(300)
        self.wheeledbase.max_lindec.set(300)
        self.wheeledbase.max_linvel.set(400)
        
        #degagement de l'espace devant la balance
        do_freeing = self.display.get_time_remaining()>17
        if do_freeing :
            self.log("BALANCE ACTION :", "Dégagement de la balance car le temps le permet")
        else:
            self.log("BALANCE ACTION :", "Pas de dégagement car pas le temps !")
        ###
        if do_freeing:
            if self.side == self.YELLOW:
                self.mover.turnonthespot(pi/4)
                self.wheeledbase.wait()
                self.pushers.down_r()
            else :
                self.mover.turnonthespot(-pi/4)
                self.wheeledbase.wait()
                self.pushers.down_l()
                ###
        try :
            self.wheeledbase.goto(*self.points["Bal4"], lookahead=150)
        except Exception as e:
            pass

        ###
        
        if self.side == self.YELLOW:
            if do_freeing:
                self.wheeledbase.set_velocities(-0, 10)
                time.sleep(0.8)
                self.wheeledbase.stop()
                self.pushers.up()
            self.wheeledbase.turnonthespot(-pi/2) 
        else:
            if do_freeing:
                self.wheeledbase.set_velocities(-0, -10)
                time.sleep(0.8)
                self.wheeledbase.stop()
                self.pushers.up()
            self.wheeledbase.turnonthespot(pi/2) 
        self.wheeledbase.wait()
        ###


                # Recalage contre le bord pour la balance
        self.log("BALANCE ACTION :", "Positionnement pour la balance")
        self.wheeledbase.right_wheel_maxPWM.set(0.5)
        self.wheeledbase.left_wheel_maxPWM.set(0.5)


        try :
            self.wheeledbase.goto_delta(-300, 0)
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

        self.wheeledbase.right_wheel_maxPWM.set(1)
        self.wheeledbase.left_wheel_maxPWM.set(1)

        #positionnement contre la balance en attendant un spin
        try :
            self.wheeledbase.goto_delta(500, 0)
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
        
        if self.endstops.get_ES3():
            self.display.addPoints(24)
        time.sleep(0.5)

        self.gripper.open()

        time.sleep(0.5)
        self.wheeledbase.goto_delta(-220,0)
        self.wheeledbase.wait()

    def before(self):
        pass

    def after(self):
        self.master.release_ressource("balance")
        #  self.wheeledbase.stop()

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'balanceGAction')