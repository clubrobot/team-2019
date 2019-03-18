#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import math
import time
from threading import Thread

from robots.action import *
from daughter_cards.arm.ArmController   import ArmController

class TakeSyncronized(Actionnable):
    typ = "Take two puck"
    POINTS = 0
    TIME   = 0
    def __init__(self, side, armController, logger):
        self.side        = side
        self.logger      = logger
        #arduinos
        self.arm         = armController
        self.arm.set_logger(self.logger)

    def realize(self):
        self.logger("TakeSyncronized :", "Realize")
        self.arm.take_puck_in_distributor()
        self.arm.put_in_tank()
        self.arm.go_home()

class PutBalance(Actionnable):
    typ = "Put in Balance"
    POINTS = 0
    TIME   = 0
    def __init__(self, side, armController, logger):
        self.side        = side
        self.logger      = logger
        #arduinos
        self.arm         = armController
        self.arm.set_logger(self.logger)

    def realize(self):
        self.logger("PutBalance :", "Realize")
        for i in range(0,2):
            self.arm.take_in_tank()
            self.arm.put_in_balance()
