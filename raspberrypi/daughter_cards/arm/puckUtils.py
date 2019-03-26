#!/usr/bin/env python3
#-*- coding: utf-8 -*-

from collections import namedtuple
import time

# Named Tuple for puck definition
Puck        = namedtuple('Puck', ['color'])

# Named Tuple for Distributors definition
Ditrib6 = namedtuple('Ditrib6', ['Puck1','Puck2','Puck3','Puck4','Puck5','Puck6'])
Ditrib3 = namedtuple('Ditrib3', ['Puck1','Puck2','Puck3'])

# Puck Type
RedPuck     = Puck('RED')
GreenPuck   = Puck('GREEN')
BluePuck    = Puck('BLUE')
EmptyPuck   = Puck('EMPTY')   

DISTRIB_6   = Ditrib6(RedPuck, GreenPuck, RedPuck, BluePuck, RedPuck, GreenPuck)
DISTRIB_3   = Ditrib3(BluePuck, GreenPuck, RedPuck)

class PuckTank():
    def __init__(self, name, size, logger):
        self.name   = name
        self.size   = size
        self.logger = logger
        self.tank   = []

    def put_puck(self, puck):
        if len(self.tank) < self.size :
            self.logger(self.name, "Put : {} Puck".format(puck.color))
            self.tank.append(puck)
        else :
            raise RuntimeError('Tank Full')

    def get_puck(self):
        if len(self.tank) > 0 :
            puck = self.tank.pop()
            self.logger(self.name, "Get : {} Puck".format(puck.color))
            return puck
        else :
            raise RuntimeError('Tank empty')

    def get_index(self):
        return len(self.tank)

if __name__ == '__main__':
    from common.logger      import Logger
    # Configure wheeledbase
    log = Logger(Logger.SHOW)

    tank = PuckTank('TANK FRONT', 3, log)

    tank.put_puck(DISTRIB_6.Puck2)
    tank.put_puck(DISTRIB_6.Puck4)
    tank.put_puck(DISTRIB_6.Puck6)

    tank.get_puck()
    tank.get_puck()
    tank.get_puck()