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
            raise RuntimeError('Tank Full')

    def get_puck(self):
        if len(self.tank) > 0 :
            puck = self.tank.pop()
            self.logger(self.name, "Get : {} Puck".format(puck.name))
            return puck
        else :
            raise RuntimeError('Tank empty')

    def index(self):
        return len(self.tank)

if __name__ == '__main__':
    from common.logger      import Logger
    # Configure wheeledbase
    log = Logger(Logger.SHOW)

    tank = PuckTank('TANK FRONT', 3, log)

    tank.put_puck(RedPuck)
    tank.put_puck(BluePuck)
    tank.put_puck(GreenPuck)

    tank.get_puck()
    tank.get_puck()
    tank.get_puck()