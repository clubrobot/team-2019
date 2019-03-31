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
        self.actionPoint    = self.geogebra.get('Distrib_{}_{}'.format(self.side,self.distrib_pos))
        self.theta          = pi/2

    def realize(self):
        self.log("TakeSyncDistrib")

        self.armFront.start_pump()
        self.armBack.start_pump()

        self.wheeledbase.turnonthespot(self.theta)
        while not self.wheeledbase.is_arrived():
            time.sleep(0.1)

        self.armFront.take()
        self.armBack.take()

        while not (self.armFront.take.end() and self.armBack.take.end()):
            time.sleep(0.1)

        self.armFront.take()
        self.armBack.take()

        while not (self.armFront.take.end() and self.armBack.take.end()):
            time.sleep(0.1)

    #override
    def getAction(self):
        return [Action(self.actionPoint, lambda : self.realize(), 'Distrib_{}_{}'.format(self.color,self.distrib_pos))]



class TakeSpecificArm(Actionnable):
    def __init__(self, geogebra, daughter_cards, arm, color, distrib_pos, log):
        self.geogebra       = geogebra
        self.log            = log
        self.color          = color
        self.distrib_pos    = distrib_pos
        self.arm            = daughter_cards[arm]
        self.wheeledbase    = daughter_cards['wheeledbase']
        self.actionPoint    = self.geogebra.get('Distrib_{}_{}'.format(self.color,self.distrib_pos))

    def realize(self):
        self.log("TakeSpecificArm")

    #override
    def getAction(self):
        return [Action(self.actionPoint, lambda : self.realize(), 'Distrib_{}_{}'.format(self.color,self.distrib_pos))]

class PutBalance(Actionnable):
    def __init__(self, geogebra, daughter_cards, side, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side
        self.armFront       = daughter_cards['armFront']
        self.wheeledbase    = daughter_cards['wheeledbase']
        self.actionPoint    = self.geogebra.get('Balance_{}'.format(self.side))

    def realize(self):
        self.log("PutBalance")

    #override
    def getAction(self):
        return [Action(self.actionPoint, lambda : self.realize(), 'Balance_{}'.format(self.color))]

class PutRedZone(Actionnable):
    def __init__(self, geogebra, daughter_cards, armFront, color, log):
        self.geogebra       = geogebra
        self.log            = log
        self.color          = color
        self.armFront       = daughter_cards[armFront]
        self.wheeledbase    = daughter_cards['wheeledbase']
        self.actionPoint    = self.geogebra.get('Balance_{}'.format(self.color))

    def realize(self):
        self.log("PutRedZone")

    #override
    def getAction(self):
        return [Action(self.actionPoint, lambda : self.realize(), 'Red_Zone_{}'.format(self.color))]