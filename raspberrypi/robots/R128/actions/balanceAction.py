#!/usr/bin/python3
#-*- coding: utf-8 -*-

import time
from math import pi
from common.actions.action import *
from common.funcutils      import *
from daughter_cards.wheeledbase import *
from daughter_cards.arm.ArmPosition import *

class BalanceAfter6(Actionnable):
    YELLOW  = 0
    PURPLE  = 1

    def __init__(self, geogebra, daughter_cards, side, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side

        if self.side == self.YELLOW:
            self.arm        = daughter_cards['armFront']
            self.arm2        = daughter_cards['armBack']
        else:
            self.arm        = daughter_cards['armBack']
            self.arm2        = daughter_cards['armFront']

        self.display        = daughter_cards['display']
        self.wheeledbase    = daughter_cards['wheeledbase']
        
        self.master         = daughter_cards["master"]

        # action Points
        self.point          = self.geogebra.get('Balance{}'.format(self.side))
        self.actionPoint    = ActPoint(self.point, pi/2)

        #armPos
        self.beforeTankPos  = [BEFORE_TAKE_TANK_PUCK1, BEFORE_TAKE_TANK_PUCK2, BEFORE_TAKE_TANK_PUCK3]
        self.TankPos        = [TAKE_TANK_PUCK1, TAKE_TANK_PUCK2, TAKE_TANK_PUCK3]
        self.afterTankPos   = [AFTER_TAKE_TANK_PUCK1, AFTER_TAKE_TANK_PUCK2, AFTER_TAKE_TANK_PUCK3]

        self.handeledPuck   = None

        # Taking error event
        self.takeErrorPuck  = False

        # taking state event
        self.armTakingState    = Event()

    def moving(self):
        if self.master.is_active():
            while self.master.get_ressource("balance"):
                time.sleep(0.4)


        self.wheeledbase.goto(*self.actionPoint.point, theta=self.actionPoint.theta)

    def realize(self):

        if self.side == self.YELLOW:
            self.wheeledbase.set_velocities(50,0)
            time.sleep(1)
            self.wheeledbase.set_velocities(0,0)
        else:
            self.wheeledbase.set_velocities(-50,0)
            time.sleep(1)
            self.wheeledbase.set_velocities(0,0)

        # put the first handled puck
        self.arm.move(BALANCE)
        
        while not self.arm.is_arrived():
            time.sleep(0.1)
        
        if not self.arm.get_atmosphere_pressure():
            self.display.addPoints(self.handeledPuck.getPoints().Balance)
            self.log("BALANCE6", "Add {} points".format(self.handeledPuck.getPoints().Balance))
        else:
            self.log("BALANCE6", "Loose PUCK")

        time.sleep(0.2)
        self.arm.stop_pump()
        time.sleep(0.5)

        self.arm.move(TANK_POS_INTER)
        while not self.arm.is_arrived():
            time.sleep(0.1)

        self.remainingPuck = self.arm.tank.index()

        for i in range(0,self.remainingPuck):
            self.arm.start_pump()

            self.arm.move(self.beforeTankPos[self.arm.tank.index()-1])
            while not self.arm.is_arrived():
                time.sleep(0.1)

            self.arm.move(self.TankPos[self.arm.tank.index()-1])
            while not self.arm.is_arrived():
                time.sleep(0.1)
                
            time.sleep(0.2)
            # get the current time
            self.init_time = time.time()

            # while not the taking sequence is end
            while not self.armTakingState.is_set():
                if not self.arm.get_atmosphere_pressure():
                    # move after take
                    self.log("Balance6", "Take Puck {}... OK".format(self.arm.tank.index()-1))
                    self.arm.move(self.afterTankPos[self.arm.tank.index()-1])
                    self.armTakingState.set()
                else:
                    # retry with new pos
                    self.log("Balance6", "Take Puck... Retry")
                    self.new_arm_pos = ArmPos(self.TankPos[self.arm.tank.index()-1].x - 1 ,self.TankPos[self.arm.tank.index()-1].y, self.TankPos[self.arm.tank.index()-1].phi)
                    self.arm.move(self.new_arm_pos)

                    while not (self.arm.is_arrived()):
                        time.sleep(0.1)
                    # check presure
                    if not self.arm.get_atmosphere_pressure():
                        # wait for taking secure
                        time.sleep(0.5)
                        self.log("Balance6", "Take Puck {}... OK".format(self.arm.tank.index()-1))
                        self.arm.move(self.afterTankPos[self.arm.tank.index()-1])
                        self.armTakingState.set()
                    else:
                        # error stop pump
                        self.takeErrorPuck = True
                        self.armTakingState.set()
                        self.log("Balance6", "Take Puck... ERROR")

            while not (self.arm.is_arrived()):
                time.sleep(0.1)
    
            # reset state
            self.armTakingState.clear()

            self.handeledPuck = self.arm.tank.get_puck()

            # put the remaning pucks
            self.arm.move(BALANCE)
            while not self.arm.is_arrived():
                time.sleep(0.1)

            if not self.arm.get_atmosphere_pressure():
                self.display.addPoints(self.handeledPuck.getPoints().Balance)
                self.log("BALANCE6", "Add {} points".format(self.handeledPuck.getPoints().Balance))

            time.sleep(0.5)
            self.arm.stop_pump()
            time.sleep(0.5)

            if(self.arm.tank.index() > 0):
                self.arm.move(TANK_POS_INTER)
                while not self.arm.is_arrived():
                    time.sleep(0.1)
            else:
                self.arm.move(GLOBAL_POS_INTER_AFTER_BALANCE)
                while not self.arm.is_arrived():
                    time.sleep(0.1)

    def before(self):
        self.arm.start_pump()
        if(self.arm.sucker.index() > 0):
            self.arm.move(GLOBAL_POS_INTER)

            while not self.arm.is_arrived():
                time.sleep(0.1)
            
            self.handeledPuck = self.arm.sucker.get_puck()
            
            
        elif (self.arm.tank.index() > 0):
            self.arm.start_pump()

            self.arm.move(self.beforeTankPos[self.arm.tank.index()-1])
            while not self.arm.is_arrived():
                time.sleep(0.1)

            self.arm.move(self.TankPos[self.arm.tank.index()-1])
            while not self.arm.is_arrived():
                time.sleep(0.1)
                
            time.sleep(0.2)
            # get the current time
            self.init_time = time.time()

            # while not the taking sequence is end
            while not self.armTakingState.is_set():
                if not self.arm.get_atmosphere_pressure():
                    # move after take
                    self.log("Balance6", "Take Puck {}... OK".format(self.arm.tank.index()-1))
                    self.arm.move(self.afterTankPos[self.arm.tank.index()-1])
                    self.armTakingState.set()
                else:
                    # retry with new pos
                    self.log("Balance6", "Take Puck... Retry")
                    self.new_arm_pos = ArmPos(self.TankPos[self.arm.tank.index()-1].x - 1 ,self.TankPos[self.arm.tank.index()-1].y, self.TankPos[self.arm.tank.index()-1].phi)
                    self.arm.move(self.new_arm_pos)

                    while not (self.arm.is_arrived()):
                        time.sleep(0.1)
                    # check presure
                    if not self.arm.get_atmosphere_pressure():
                        # wait for taking secure
                        time.sleep(0.5)
                        self.log("Balance6", "Take Puck {}... OK".format(self.arm.tank.index()-1))
                        self.arm.move(self.afterTankPos[self.arm.tank.index()-1])
                        self.armTakingState.set()
                    else:
                        # error stop pump
                        self.takeErrorPuck = True
                        self.armTakingState.set()
                        self.log("Balance6", "Take Puck... ERROR")

            while not (self.arm.is_arrived()):
                time.sleep(0.1)
    
            # reset state
            self.armTakingState.clear()
            self.handeledPuck = self.arm.tank.get_puck()
        
    def after(self):
        self.master.release_ressource("balance")

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'PutBalance6')

class BalanceAfter3(Actionnable):
    YELLOW  = 0
    PURPLE  = 1

    def __init__(self, geogebra, daughter_cards, side, log):
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side

        if self.side == self.YELLOW:
            self.arm1       = daughter_cards['armFront']
            self.arm2       = daughter_cards['armBack']
        else:
            self.arm1       = daughter_cards['armBack']
            self.arm2       = daughter_cards['armFront']
        
        self.wheeledbase    = daughter_cards['wheeledbase']
        self.display        = daughter_cards['display']

        # action Points
        self.point          = self.geogebra.get('Balance{}'.format(self.side))
        self.actionPoint    = ActPoint(self.point, pi/2)
        
        # path Points
        self.path           = self.geogebra.getall('PathDistrib{}_*'.format(self.side))
        self.path.reverse()
        self.path.append(self.point)

        #armPos
        self.beforeTankPos  = [BEFORE_TAKE_TANK_PUCK1, BEFORE_TAKE_TANK_PUCK2, BEFORE_TAKE_TANK_PUCK3]
        self.TankPos        = [TAKE_TANK_PUCK1, TAKE_TANK_PUCK2, TAKE_TANK_PUCK3]
        self.afterTankPos   = [AFTER_TAKE_TANK_PUCK1, AFTER_TAKE_TANK_PUCK2, AFTER_TAKE_TANK_PUCK3]

        self.handeledPuck1   = None
        self.handeledPuck2   = None

    def moving(self):
        if self.side == self.YELLOW:
            self.wheeledbase.turnonthespot(-pi)
        else:
            self.wheeledbase.turnonthespot(0)
        while not self.wheeledbase.isarrived():
            time.sleep(0.1)

        if self.side == self.YELLOW:
            self.wheeledbase.purepursuit(self.path, direction='forward')
            while not self.wheeledbase.isarrived():
                time.sleep(0.1)
        else:
            self.wheeledbase.purepursuit(self.path, direction='backward')
            while not self.wheeledbase.isarrived():
                time.sleep(0.1)

        self.wheeledbase.turnonthespot(pi/2)
        while not self.wheeledbase.isarrived():
            time.sleep(0.1)

        if self.side == self.YELLOW:
            self.wheeledbase.set_velocities(50,0)
            time.sleep(1)
            self.wheeledbase.set_velocities(0,0)
        else:
            self.wheeledbase.set_velocities(-50,0)
            time.sleep(1)
            self.wheeledbase.set_velocities(0,0)

    def realize(self):
        
        # put the first handled puck
        self.arm1.move(BALANCE)
        self.arm2.move(BALANCE)
        while not (self.arm1.is_arrived()and self.arm2.is_arrived()):
            time.sleep(0.1)

        if not self.arm1.get_atmosphere_pressure():
            self.display.addPoints(self.handeledPuck1.getPoints().Balance)
            self.log("BALANCE3", "Add {} points".format(self.handeledPuck1.getPoints().Balance))
        
        if not self.arm2.get_atmosphere_pressure():
            self.display.addPoints(self.handeledPuck2.getPoints().Balance)
            self.log("BALANCE3", "Add {} points".format(self.handeledPuck2.getPoints().Balance))

        time.sleep(0.5)
        self.arm1.stop_pump()
        self.arm2.stop_pump()
        time.sleep(0.5)

        self.arm1.move(TANK_POS_INTER)
        self.arm2.move(TANK_POS_INTER)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)
    
    def before(self):
        time.sleep(1)
        if(self.arm1.sucker.index() > 0):
            self.arm1.move(GLOBAL_POS_INTER)

            while not self.arm1.is_arrived():
                time.sleep(0.1)
            
            self.handeledPuck1 = self.arm1.sucker.get_puck()

        if(self.arm2.sucker.index() > 0):
            self.arm2.move(BALANCE3_POS_INTER)

            while not self.arm2.is_arrived():
                time.sleep(0.1)
            
            self.handeledPuck2 = self.arm2.sucker.get_puck()
        
    def after(self):
        pass

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'PutBalance3')