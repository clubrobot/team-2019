#!/usr/bin/python3
#-*- coding: utf-8 -*-

import time
from math import pi
from common.actions.action import *
from common.funcutils      import *
from daughter_cards.wheeledbase import *

class TakeSyncDistrib(Actionnable):
    def __init__(self, geogebra, daughter_cards, side, distrib_pos, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side
        self.distrib_pos    = distrib_pos
        self.armFront       = daughter_cards['armFront']
        self.armBack        = daughter_cards['armBack']
        self.wheeledbase    = daughter_cards['wheeledbase']
        self.point          = self.geogebra.get('Distrib{}_{}'.format(self.side,self.distrib_pos))
        self.actionPoint    = ActPoint(self.point, pi/2)

    # Main action
    def realize(self):
        self.armFront.start_pump()
        self.armBack.start_pump()

        self.wheeledbase.turnonthespot(self.actionPoint.theta)
        while not self.wheeledbase.isarrived():
            time.sleep(0.1)

        self.armFront.prepare_to_take_1()
        self.armBack.prepare_to_take_1()

        while not (self.armFront.is_arrived() and self.armBack.is_arrived()):
            time.sleep(0.1)

        self.armFront.take()
        self.armBack.take()

        while not (self.armFront.is_arrived() and self.armBack.is_arrived()):
            time.sleep(0.1)

        self.armFront.after_take()
        self.armBack.after_take()

        while not (self.armFront.is_arrived() and self.armBack.is_arrived):
                time.sleep(0.1)

    def prepare(self):
        self.armFront.prepare_to_take()
        self.armBack.prepare_to_take()

        while not (self.armFront.is_arrived() and self.armBack.is_arrived):
                time.sleep(0.1)

    def complete(self):
        self.armFront.go_home()
        self.armBack.go_home()

        while not (self.armFront.is_arrived() and self.armBack.is_arrived):
                time.sleep(0.1)


    #override
    def getAction(self):
        return [Action(self.actionPoint, 
                lambda : self.realize(), 
                'TakeSyncDistrib', 
                actionPrep = self.prepare,
                actionComp = self.complete)]

class PutBalance(Actionnable):
    def __init__(self, geogebra, daughter_cards, side, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side
        self.armFront       = daughter_cards['armFront']
        self.wheeledbase    = daughter_cards['wheeledbase']
        self.point    = self.geogebra.get('Balance{}'.format(self.side))
        self.actionPoint    = ActPoint(self.point, pi/2)

    def realize(self):
        pass
    
    def prepare(self):
        pass

    def complete(self):
        pass

    #override
    def getAction(self):
        return [Action(self.actionPoint, 
                lambda : self.realize(), 
                'PutBalance', 
                actionPrep = self.prepare,
                actionComp = self.complete)]

class PutRed(Actionnable):
    def __init__(self, geogebra, daughter_cards, side, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side
        self.armFront       = daughter_cards['armFront']
        self.wheeledbase    = daughter_cards['wheeledbase']
        self.point          = self.geogebra.get('Start{}'.format(self.side))
        self.actionPoint    = ActPoint(self.point, -pi/2)

    def realize(self):
        pass
    
    def prepare(self):
        pass

    def complete(self):
        pass

    #override
    def getAction(self):
        return [Action(self.actionPoint, 
                lambda : self.realize(), 
                'PutBalance', 
                actionPrep = self.prepare,
                actionComp = self.complete)]
