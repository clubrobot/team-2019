#!/usr/bin/python3
#-*- coding: utf-8 -*-

import time
from math import pi
from common.actions.action import *
from common.funcutils      import *
from daughter_cards.wheeledbase import *
from daughter_cards.arm.ArmPosition import *
from daughter_cards.arm.puckUtils import *
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
            
        self.wheeledbase    = daughter_cards['wheeledbase']
        self.sensor_manager = daughter_cards['sensor_manager']
        # action Points
        self.point          = self.geogebra.get('Distrib{}_{}'.format(self.side,self.distrib_pos))
        self.actionPoint    = ActPoint(self.point, pi/2)

        self.TankPos        = [TAKE_TANK_PUCK1, TAKE_TANK_PUCK2, TAKE_TANK_PUCK3]

        self.handeledPuck   = None
        self.takeError      = False

        self.puck1      = puckFront
        self.puck2      = puckBack

        if self.side == self.YELLOW:
            self.sensor_manager.disable_front()
            self.sensor_manager.enable_back()
        else:
            self.sensor_manager.enable_front()
            self.sensor_manager.disable_back()

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
            self.arm1.move(TANK_POS_INTER)
            self.arm2.move(TANK_POS_INTER)
            while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
                time.sleep(0.1)

            self.arm1.move(self.TankPos[self.arm1.tank.index()])
            self.arm2.move(self.TankPos[self.arm2.tank.index()])
            while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
                time.sleep(0.1)

            self.arm1.tank.put_puck(self.puck1)
            self.arm2.tank.put_puck(self.puck2)

            self.arm1.stop_pump()
            self.arm2.stop_pump()

            time.sleep(0.5)
            self.arm1.move(PUT_TANK_AFTER)
            self.arm2.move(PUT_TANK_AFTER)

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
        
        self.wheeledbase    = daughter_cards['wheeledbase']
        # action Points
        self.point          = self.geogebra.get('Distrib{}_{}'.format(self.side,self.distrib_pos))
        self.actionPoint    = ActPoint(self.point, pi/2)

        self.TankPos        = [TAKE_TANK_PUCK1, TAKE_TANK_PUCK2, TAKE_TANK_PUCK3]

        self.handeledPuck   = None
        self.takeError      = False

        self.puck = puck

    def realize(self):
        self.arm.start_pump()
        self.arm.move(PREPARE_TAKING_POS_STATIC)
        while not (self.arm.is_arrived()):
            time.sleep(0.1)

        self.arm.move(TAKE_PUCK_STATIC)
        while not (self.arm.is_arrived()):
            time.sleep(0.1)

        time.sleep(0.5)
    
        self.arm.move(TAKE_PUCK_INTER_AFTER_STATIC)
        
        while not (self.arm.is_arrived()):
            time.sleep(0.1)
    
    def before(self):
        self.arm.move(PREPARE_TAKING_POS_ROAD)
        while not (self.arm.is_arrived()):
            time.sleep(0.1)

    def after(self):
        if not self.takeError:
            self.arm.move(TANK_POS_INTER)
            while not (self.arm.is_arrived()):
                time.sleep(0.1)

            self.arm.move(self.TankPos[self.arm.tank.index()])
            while not (self.arm.is_arrived()):
                time.sleep(0.1)

            self.arm.tank.put_puck(self.puck)

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

    def __init__(self, geogebra, daughter_cards, side, distrib_pos, log):
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
            
        self.wheeledbase    = daughter_cards['wheeledbase']
        # action Points
        self.point          = self.geogebra.get('Distrib{}_{}'.format(self.side,self.distrib_pos))
        self.actionPoint    = ActPoint(self.point, pi/2)

        self.TankPos        = [TAKE_TANK_PUCK1, TAKE_TANK_PUCK2, TAKE_TANK_PUCK3]

        self.handeledPuck   = None
        self.takeError      = False

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

        time.sleep(0.5)
    
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
            self.arm1.move(TANK_POS_INTER)
            self.arm2.move(TANK_POS_INTER)
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