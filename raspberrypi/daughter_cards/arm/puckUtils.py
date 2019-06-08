#!/usr/bin/env python3
#-*- coding: utf-8 -*-
from collections import namedtuple
import time

Point = namedtuple('Point',['Balance','Tab','Accelerator'])

class Puck():
    def __init__(self, name, points):
        self.name   = name
        self.points = points

    def getPoints(self):
        return self.points

#Puck definition
RedPuck     = Puck('red'    ,Point(4    ,5+1,10))
GreenPuck   = Puck('green'  ,Point(8    ,5+1,10))
BluePuck    = Puck('blue'   ,Point(12   ,5+1,10))
NonePuck    = Puck('None'   ,Point(0   ,0, 0))


class PuckTank():
    def __init__(self, name, size, logger):
        self.name   = name
        self.size   = size
        self.logger = logger
        self.tank   = []

    def put_puck(self, puck):
        if len(self.tank) < self.size :
            self.logger(self.name, "Put : {} Puck".format(puck.name))
            self.tank.append(puck)
        else :
            self.logger("puckTank", "tank full")

    def get_puck(self):
        if len(self.tank) > 0 :
            puck = self.tank.pop()
            self.logger(self.name, "Get : {} Puck".format(puck.name))
            return puck
        else :
            return NonePuck

    def index(self):
        return len(self.tank)

class Sucker():
    def __init__(self, name, logger):
        self.name   = name
        self.logger = logger
        self.tank   = []
        self.full   = False

    def put_puck(self, puck):
        if not self.full:
            self.logger(self.name, "Put : {} Puck".format(puck.name))
            self.tank.append(puck)
            self.full = True
        else :
            self.logger("sucker", "sucker full")


    def get_puck(self):
        if self.full:
            self.full = False
            puck = self.tank.pop()
            self.logger(self.name, "Get : {} Puck".format(puck.name))
            return puck
        else :
            return NonePuck

    def index(self):
        return len(self.tank)

if __name__ == '__main__':
    from common.logger      import Logger
    # Configure wheeledbase
    log = Logger(Logger.SHOW)

    tank = PuckTank('TANK FRONT', 3, log)

    print(tank.get_puck().getPoints().Balance)
    print(tank.get_puck().getPoints().Tab)
    print(tank.get_puck().getPoints().Accelerator)
    print(tank.get_puck().getPoints().Balance)
    print(tank.get_puck().getPoints().Tab)
    print(tank.get_puck().getPoints().Accelerator)
