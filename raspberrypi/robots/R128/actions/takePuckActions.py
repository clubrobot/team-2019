#!/usr/bin/python3
#-*- coding: utf-8 -*-

import time
from math import pi
from common.actions.action import *
from common.funcutils      import *
from daughter_cards.arm.ArmPosition import *
from daughter_cards.arm.puckUtils import *

offset_x = 0
offset_y = 0

class TakePuckSync(Actionnable):
    YELLOW  = 0
    PURPLE  = 1

    def __init__(self, geogebra, daughter_cards, side, distrib_pos, puckFront, puckBack, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side
        self.distrib_pos    = distrib_pos

        if self.side == self.YELLOW:
            self.arm1       = daughter_cards['armFront']
            self.arm2       = daughter_cards['armBack']
        else:
            self.arm1       = daughter_cards['armBack']
            self.arm2       = daughter_cards['armFront']

        # action Points
        self.point          = self.geogebra.get('Distrib{}_{}'.format(self.side,self.distrib_pos))
        print(type(self.point))
        if self.distrib_pos == 1:
            self.RecalagePoint  = self.geogebra.get('Recal{}'.format(self.side))
            self.point = (self.point[0] + offset_x, self.point[1] + offset_y)
            self.actionPoint    = ActPoint(self.point, pi)
        else:
            self.point = (self.point[0] + offset_x, self.point[1] + offset_y)
            self.actionPoint    = ActPoint(self.point, pi/2)

        #wheeledbase
        self.wheeledbase    = daughter_cards['wheeledbase']

        self.TankPos        = [TAKE_TANK_PUCK1, TAKE_TANK_PUCK2, TAKE_TANK_PUCK3]

        self.handeledPuck   = None
        self.takeError      = False

        self.puck1          = puckFront
        self.puck2          = puckBack

    def realize(self):
        if self.distrib_pos == 1:
            self.wheeledbase.set_velocities(-100,0)
            time.sleep(2)
            self.correct_pos = self.wheeledbase.get_position()
            print(self.correct_pos)
            offset_x = self.RecalagePoint[0] - self.correct_pos[0]
            print('offset x ; {}'.format(offset_x))
            print('offset y ; {}'.format(offset_y))

            self.point = (self.point[0] - offset_x, self.point[1] - offset_y)

            self.wheeledbase.goto(*self.point, theta=pi/2, direction = 'forward')
            
            self.arm1.move(PREPARE_TAKING_POS_ROAD)
            self.arm2.move(PREPARE_TAKING_POS_ROAD)
            while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
                time.sleep(0.1)

        self.arm1.start_pump()
        self.arm2.start_pump()
        self.arm1.move(PREPARE_TAKING_POS_STATIC)
        self.arm2.move(PREPARE_TAKING_POS_STATIC)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

        self.arm1.move(TAKE_PUCK_STATIC)
        self.arm2.move(TAKE_PUCK_STATIC)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

        while self.arm1.get_atmosphere_pressure():
            time.sleep(0.2)

        while self.arm2.get_atmosphere_pressure():
            time.sleep(0.2)

        # handle puck by sucker
        self.arm1.sucker.put_puck(self.puck1)
        self.arm2.sucker.put_puck(self.puck2)
    
        self.arm1.move(TAKE_PUCK_INTER_AFTER_STATIC)
        self.arm2.move(TAKE_PUCK_INTER_AFTER_STATIC)
        
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

    def before(self):
        if self.distrib_pos == 1:
            pass
        else:
            self.arm1.move(PREPARE_TAKING_POS_ROAD)
            self.arm2.move(PREPARE_TAKING_POS_ROAD)
            while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
                time.sleep(0.1)

    def after(self):
        if not self.takeError:
            if(self.arm1.tank.index() > 1):
                self.arm1.move(GLOBAL_POS_INTER)
            else:
                self.arm1.move(TANK_POS_INTER)

            self.arm2.move(TANK_POS_INTER)
            while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
                time.sleep(0.1)
            
            if(self.arm1.tank.index() < 2):
                self.arm1.move(self.TankPos[self.arm1.tank.index()])
            self.arm2.move(self.TankPos[self.arm2.tank.index()])

            while not (self.arm1.is_arrived()):
                time.sleep(0.1)
            while not (self.arm2.is_arrived()):
                time.sleep(0.1)

            if(self.arm1.tank.index() < 2):
                self.arm1.stop_pump()
            self.arm2.stop_pump()

            # store puck on tank
            if(self.arm1.tank.index() < 2):
                self.arm1.tank.put_puck(self.arm1.sucker.get_puck())
            self.arm2.tank.put_puck(self.arm2.sucker.get_puck())

            time.sleep(0.5)

            if(self.arm1.tank.index() < 2):
                self.arm1.move(PUT_TANK_AFTER)
            self.arm2.move(PUT_TANK_AFTER)

            while not (self.arm1.is_arrived()):
                time.sleep(0.1)
            while not (self.arm2.is_arrived()):
                time.sleep(0.1)

        else:
            self.arm1.go_home()
            self.arm2.go_home()
            while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
                time.sleep(0.1)

    #override
    def getAction(self):
        return Action(self.actionPoint, lambda : self.realize(), self.before, self.after, 'TakePuck_{}'.format(self.distrib_pos))

class TakePuckSingle(Actionnable):
    YELLOW  = 0
    PURPLE  = 1

    def __init__(self, geogebra, daughter_cards, side, distrib_pos, puck, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side
        self.distrib_pos    = distrib_pos

        if self.side == self.YELLOW:
            self.arm        = daughter_cards['armFront']
        else:
            self.arm        = daughter_cards['armBack']

        # action Points
        self.point          = self.geogebra.get('Distrib{}_{}'.format(self.side,self.distrib_pos))
        self.actionPoint    = ActPoint(self.point, pi/2)

        self.TankPos        = [TAKE_TANK_PUCK1, TAKE_TANK_PUCK2, TAKE_TANK_PUCK3]

        self.handeledPuck   = None
        self.takeError      = False

        self.puck = puck

    def realize(self):
        self.arm.move(PREPARE_TAKING_POS_ROAD)
        while not (self.arm.is_arrived()):
            time.sleep(0.1)
        
        self.arm.start_pump()
        self.arm.move(PREPARE_TAKING_POS_STATIC)
        while not (self.arm.is_arrived()):
            time.sleep(0.1)

        self.arm.move(TAKE_PUCK_STATIC)
        while not (self.arm.is_arrived()):
            time.sleep(0.1)

        while self.arm.get_atmosphere_pressure():
            time.sleep(0.2)

        # handle puck by sucker
        self.arm.sucker.put_puck(self.puck)
    
        self.arm.move(TAKE_PUCK_INTER_AFTER_STATIC)
        
        while not (self.arm.is_arrived()):
            time.sleep(0.1)

    
    def before(self):
        pass
        

    def after(self):
        if not self.takeError:
            self.arm.move(TANK_POS_INTER)
            while not (self.arm.is_arrived()):
                time.sleep(0.1)

            self.arm.move(self.TankPos[self.arm.tank.index()])
            while not (self.arm.is_arrived()):
                time.sleep(0.1)

            self.arm.tank.put_puck(self.arm.sucker.get_puck())

            self.arm.stop_pump()

            time.sleep(0.5)
            self.arm.move(PUT_TANK_AFTER)
            self.arm.move(PUT_TANK_AFTER)

            while not (self.arm.is_arrived()):
                time.sleep(0.1)

        else:
            self.arm.go_home()
            while not (self.arm.is_arrived()):
                time.sleep(0.1)
        

    #override
    def getAction(self):
        return Action(self.actionPoint, lambda : self.realize(), self.before, self.after, 'TakePuck')


class TakePuckSyncMaintain(Actionnable):
    YELLOW  = 0
    PURPLE  = 1

    def __init__(self, geogebra, daughter_cards, side, distrib_pos, puckFront, puckBack, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side
        self.distrib_pos    = distrib_pos

        if self.side == self.YELLOW:
            self.arm1       = daughter_cards['armFront']
            self.arm2       = daughter_cards['armBack']
        else:
            self.arm1       = daughter_cards['armBack']
            self.arm2       = daughter_cards['armFront']

        # action Points
        self.point          = self.geogebra.get('Distrib{}_{}'.format(self.side,self.distrib_pos))
        self.actionPoint    = ActPoint(self.point, pi/2)

        self.TankPos        = [TAKE_TANK_PUCK1, TAKE_TANK_PUCK2, TAKE_TANK_PUCK3]

        self.handeledPuck   = None
        self.takeError      = False

        self.puck1      = puckFront
        self.puck2      = puckBack

    def realize(self):
        self.arm1.start_pump()
        self.arm2.start_pump()
        self.arm1.move(PREPARE_TAKING_POS_STATIC)
        self.arm2.move(PREPARE_TAKING_POS_STATIC)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

        self.arm1.move(TAKE_PUCK_STATIC)
        self.arm2.move(TAKE_PUCK_STATIC)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

        while self.arm1.get_atmosphere_pressure():
            time.sleep(0.2)

        while self.arm2.get_atmosphere_pressure():
            time.sleep(0.2)

        # handle puck by sucker
        self.arm1.sucker.put_puck(self.puck1)
        self.arm2.sucker.put_puck(self.puck2)
    
        self.arm1.move(TAKE_PUCK_INTER_AFTER_STATIC)
        self.arm2.move(TAKE_PUCK_INTER_AFTER_STATIC)
        
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

    def before(self):
        self.arm1.move(PREPARE_TAKING_POS_ROAD)
        self.arm2.move(PREPARE_TAKING_POS_ROAD)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

    def after(self):
        if not self.takeError:
            self.arm1.move(HOME)
            self.arm2.move(HOME)
            while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
                time.sleep(0.1)

        else:
            self.arm1.go_home()
            self.arm2.go_home()
            while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
                time.sleep(0.1)

    #override
    def getAction(self):
        return Action(self.actionPoint, lambda : self.realize(), self.before, self.after, 'TakePuck_{}'.format(self.distrib_pos))