#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from Server_utils.Components_virtual import SerialTalksComponent
from serialutils import *
from math import *
import time


_DROP_OPCODE   = 0x10
_FIRE_OPCODE   = 0x20
_GET_BALL_      = 0x30
_GET_BALL_DIRTY = 0x40


class WaterDispenser(SerialTalksComponent):
    def __init__(self,parent):
        SerialTalksComponent.__init__(self,"waterdispenser")
        self.add_method(_DROP_OPCODE, self.drop )
        self.add_method(_FIRE_OPCODE, self.fire )
        self.add_method(_GET_BALL_  , self.get_ball)
        self.add_method(_GET_BALL_DIRTY, self.get_ball_dirty)
        self.robot = parent

    def get_ball(self):
        return Deserializer(BYTE(self.robot.get_ball()))   

    def get_ball_dirty(self):
        return Deserializer(BYTE(self.robot.get_ball_dirty()))   

    def drop(self):
        self.robot.drop_ball()

    def fire(self):
        self.robot.fire_ball()

    def compute(self):
        pass


