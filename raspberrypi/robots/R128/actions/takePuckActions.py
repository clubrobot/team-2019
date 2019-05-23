#!/usr/bin/python3
#-*- coding: utf-8 -*-

import time
from math import pi, atan
from common.actions.action import *
from common.funcutils      import *
from daughter_cards.arm.ArmPosition import *
from daughter_cards.arm.puckUtils import *

from threading import Event

RECALAGE_VEL = 300
offset_x = 0

class TakePuckSync(Actionnable):
    YELLOW  = 0
    PURPLE  = 1

    DISTRIB6_1 = 1
    DISTRIB6_2 = 2
    DISTRIB6_3 = 3
    DISTRIB3_1 = 4
    DISTRIB3_2 = 5

    NO = 0
    SENSORS = 1
    WALL = 2
    RECALAGE = WALL

    def __init__(self, geogebra, daughter_cards, side, distrib_pos, puckFront, puckBack, log, sensors):
        # Save map, log and side
        self.geogebra       = geogebra
        self.log            = log
        self.side           = side
        self.distrib_pos    = distrib_pos
        self.sensors        = sensors

        # Bind arm in funtion of desired side
        if self.side == self.YELLOW:
            self.arm1       = daughter_cards['armFront']
            self.arm2       = daughter_cards['armBack']
        else:
            self.arm1       = daughter_cards['armBack']
            self.arm2       = daughter_cards['armFront']

        # wheeledbase
        self.wheeledbase    = daughter_cards['wheeledbase']

        # display
        self.display        = daughter_cards['display']

        # action Points
        self.point          = self.geogebra.get('Distrib{}_{}'.format(self.side,self.distrib_pos))

        # if action correspond to the first distrib pos, prepare starting path
        if self.distrib_pos == self.DISTRIB6_1:
            # start Path
            self.path = self.geogebra.getall('StartPath{}_*'.format(self.side))         

            # recalage point
            self.RecalagePoint  = self.geogebra.get('Recal{}'.format(self.side))
            self.point = (self.point[0] + offset_x, self.point[1])
            self.actionPoint    = ActPoint(self.point, pi)
        else:
            self.point = (self.point[0] + offset_x, self.point[1])
            self.actionPoint    = ActPoint(self.point, pi/2)

        # 3 tank pos list
        self.TankPos        = [TAKE_TANK_PUCK1, TAKE_TANK_PUCK2, TAKE_TANK_PUCK3]

        # Taking error event
        self.takeErrorPuck1     = False
        self.takeErrorPuck2     = False

        # taking state event
        self.arm1TakingState    = Event()
        self.arm2TakingState    = Event()

        # handled puck
        self.puck1              = puckFront
        self.puck2              = puckBack

    def moving(self):
        global offset_x
        self.wheeledbase.reset_parameters()
        # if action correspond to the first distrib pos
        if self.distrib_pos == self.DISTRIB6_1:
            
            # Set velocites to the max
            self.wheeledbase.lookahead.set(200.0)
            self.wheeledbase.lookaheadbis.set(50.0)

            # Start Path
            if self.side == self.YELLOW:
                self.wheeledbase.purepursuit(self.path, direction = 'forward')
            else:
                self.wheeledbase.purepursuit(self.path, direction = 'backward')
            self.wheeledbase.wait()

            sens_left = self.sensors.sensors_lat[0]
            sens_right = self.sensors.sensors_lat[1]

            if self.RECALAGE is self.SENSORS:
                # prepare recalage
                self.wheeledbase.turnonthespot(pi/2)
                self.wheeledbase.wait()
                time.sleep(1)

                left = sens_left.dist()
                right = sens_right.dist()
                self.log("CHECK ALIGNMENT:", left, right)

                dtheta = atan((right-left)/(sens_left.pos[0] - sens_right.pos[0]))
                theta = self.wheeledbase.get_position()[2]

                self.log("CHECK ALIGNMENT angle correction:", dtheta*180/pi, "°")

                self.wheeledbase.turnonthespot(theta + dtheta)
                self.wheeledbase.wait()
                time.sleep(1)

                left = sens_left.dist()
                right = sens_right.dist()
                self.log("CHECK ALIGNMENT after correction:", left, right)

            else:
                self.log("RECALAGE : face au mur en x")
                # prepare recalage
                self.wheeledbase.turnonthespot(pi)
                self.wheeledbase.wait()
                # recalage

                self.log("RECALAGE : fonce dans le mur")
                self.wheeledbase.set_velocities(-RECALAGE_VEL,0)
                try:
                    self.wheeledbase.wait()
                except:
                    pass

                self.correct_pos = self.wheeledbase.get_position()

                offset_x = self.RecalagePoint[0] - self.correct_pos[0]

                self.point = (self.point[0] - offset_x, self.point[1])

                # prepare arm to take
                self.arm1.move(PREPARE_TAKING_POS_ROAD)
                self.arm2.move(PREPARE_TAKING_POS_ROAD)

                self.log("RECALAGE : Se repositionne correctement")
                self.wheeledbase.goto(*self.point, theta=pi/2, direction = 'forward')

            while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
                time.sleep(0.1)

        else:
            # else go to point
            self.wheeledbase.lookaheadbis.set(5)
            self.point = (self.actionPoint.point[0] - offset_x, self.actionPoint.point[1])
            self.wheeledbase.goto(*self.point, theta=self.actionPoint.theta)

    def realize(self):
        # starting two pump
        self.arm1.start_pump()
        self.arm2.start_pump()

        # prepare to taking
        self.arm1.move(PREPARE_TAKING_POS_STATIC)
        self.arm2.move(PREPARE_TAKING_POS_STATIC)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

        # taking
        self.arm1.move(TAKE_PUCK_STATIC)
        self.arm2.move(TAKE_PUCK_STATIC)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

        # get the current time
        self.init_time = time.time()

        # while not the taking sequence is end
        while not (self.arm1TakingState.is_set() and self.arm2TakingState.is_set()):
            if not self.arm1.get_atmosphere_pressure():
                # handle puck by sucker
                self.arm1.sucker.put_puck(self.puck1)
                # move after take
                self.arm1.move(TAKE_PUCK_INTER_AFTER_STATIC)
                self.arm1TakingState.set()
                self.log("TakePuckSync", "Take arm 1... OK")
            else:
                # retry with new pos
                self.log("TakePuckSync", "Take arm 1... Retry")
                self.new_arm_pos = ArmPos(TAKE_PUCK_STATIC.x + 1 , TAKE_PUCK_STATIC.y + 3, TAKE_PUCK_STATIC.phi)
                self.arm1.move(self.new_arm_pos)

                while not (self.arm1.is_arrived()):
                    time.sleep(0.1)
                # check presure
                time.sleep(0.5)
                if not self.arm1.get_atmosphere_pressure():
                    # wait for taking secure
                    self.arm1.sucker.put_puck(self.puck1)
                    self.arm1.move(TAKE_PUCK_INTER_AFTER_STATIC)
                    self.log("TakePuckSync", "Take arm 1... OK")
                    self.arm1TakingState.set()
                else:
                    # error stop pump
                    self.arm1.stop_pump()
                    self.takeErrorPuck1 = True
                    self.arm1TakingState.set()
                    self.log("TakePuckSync", "Take arm 1... ERROR")
                    self.arm1.move(HOME)

            if not self.arm2.get_atmosphere_pressure():
                # handle puck by sucker
                self.arm2.sucker.put_puck(self.puck2)
                # move after take
                self.arm2.move(TAKE_PUCK_INTER_AFTER_STATIC)
                self.arm2TakingState.set()
                self.log("TakePuckSync", "Take arm 2... OK")
            else:
                # retry with new pos
                self.log("TakePuckSync", "Take arm 2... Retry")
                self.new_arm_pos = ArmPos(TAKE_PUCK_STATIC.x + 1 , TAKE_PUCK_STATIC.y + 3, TAKE_PUCK_STATIC.phi)
                self.arm2.move(self.new_arm_pos)
                # check presure
                while not (self.arm2.is_arrived()):
                    time.sleep(0.1)
                # check presure
                time.sleep(0.5)
                if not self.arm2.get_atmosphere_pressure():
                    # wait for taking secure
                    self.arm2.sucker.put_puck(self.puck2)
                    self.arm2.move(TAKE_PUCK_INTER_AFTER_STATIC)
                    self.log("TakePuckSync", "Take arm 2... OK")
                    self.arm2TakingState.set()
                else:
                    # error stop pump
                    self.arm2.stop_pump()
                    self.takeErrorPuck2 = True
                    self.arm2TakingState.set()
                    self.log("TakePuckSync", "Take arm 2... ERROR")
                    self.arm2.move(HOME)
            while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
                time.sleep(0.1)

    def before(self):
        if self.distrib_pos != self.DISTRIB6_1:
            self.arm1.move(PREPARE_TAKING_POS_ROAD)
            self.arm2.move(PREPARE_TAKING_POS_ROAD)
            while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
                time.sleep(0.1)

    def after(self):
        # if puck1 1 error go home else action
        if self.takeErrorPuck1 == False:
            if self.distrib_pos == self.DISTRIB6_3:
                self.arm1.move(GLOBAL_POS_INTER_AFTER_TAKE)
            else:
                self.arm1.move(TANK_POS_INTER)
        else:
            if self.distrib_pos == self.DISTRIB6_3:
                self.arm1.move(TANK_POS_INTER)
            else:
                self.arm1.move(PREPARE_TAKING_POS_ROAD)
        
        # if puck1 2 error go home else action
        if self.takeErrorPuck2 == False:
            self.arm2.move(TANK_POS_INTER)
        else:
            if self.distrib_pos == self.DISTRIB6_3:
                self.arm2.move(HOME)
            else:
                self.arm2.move(PREPARE_TAKING_POS_ROAD)
        
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)
        
        if self.distrib_pos != self.DISTRIB6_3:
            if self.takeErrorPuck1 == False:
                self.arm1.move(self.TankPos[self.arm1.tank.index()])

        if self.takeErrorPuck2 == False:
            self.arm2.move(self.TankPos[self.arm2.tank.index()])

        while not ((self.arm1.is_arrived() or self.takeErrorPuck1) and (self.arm2.is_arrived() or self.takeErrorPuck2)):
            time.sleep(0.1)

        if self.distrib_pos != self.DISTRIB6_3:
            if self.takeErrorPuck1 == False:
                if not self.arm1.get_atmosphere_pressure():
                    self.arm1.tank.put_puck(self.arm1.sucker.get_puck())
                else:
                    # release puck for next take
                    self.arm1.sucker.get_puck()
                time.sleep(0.2)
                self.arm1.stop_pump()


        if self.takeErrorPuck2 == False:
            if not self.arm2.get_atmosphere_pressure():
                self.arm2.tank.put_puck(self.arm2.sucker.get_puck())
            else:
                    # release puck for next take
                    self.arm2.sucker.get_puck()
            time.sleep(0.2)
            self.arm2.stop_pump()

        if self.distrib_pos != self.DISTRIB6_3:
            if self.takeErrorPuck1 == False:
                self.arm1.move(PUT_TANK_AFTER)

        if self.takeErrorPuck2 == False:
            self.arm2.move(PUT_TANK_AFTER)

        while not ((self.arm1.is_arrived() or self.takeErrorPuck1) and (self.arm2.is_arrived() or self.takeErrorPuck2)):
            time.sleep(0.1)

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'TakePuck_{}'.format(self.distrib_pos))


class TakePuckSingle(Actionnable):
    YELLOW  = 0
    PURPLE  = 1

    NO = 0
    SENSORS = 1
    WALL = 2
    RECALAGE = WALL

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
        # path Points
        self.path           = self.geogebra.getall('PathDistrib{}_*'.format(self.side))

        self.TankPos        = [TAKE_TANK_PUCK1, TAKE_TANK_PUCK2, TAKE_TANK_PUCK3]

        # Taking error event
        self.takeErrorPuck  = False

        # taking state event
        self.armTakingState    = Event()

        # handled puck
        self.puck              = puck

        # wait befor deploy arms
        self.deployArm    = Event()

        self.master         = daughter_cards["master"]


    def moving(self):
        # reach little distributor
        if self.side == self.YELLOW:
            self.wheeledbase.purepursuit(self.path, direction = 'backward')
        else:
            self.wheeledbase.purepursuit(self.path, direction = 'forward')
        
        self.wheeledbase.wait()

        # correct robot orientation
        if self.side == self.YELLOW:
            self.wheeledbase.turnonthespot(-pi)
        else:
            self.wheeledbase.turnonthespot(0)
        
        self.wheeledbase.wait()


        if self.RECALAGE == self.WALL:
            self.wheeledbase.turnonthespot(0)
            self.wheeledbase.linpos_threshold.set(10)
            self.wheeledbase.angpos_threshold.set(0.2)

            self.log("RECALAGE : face au mur en x")
            self.wheeledbase.wait()
            self.wheeledbase.set_velocities(RECALAGE_VEL, 0)

            try:
                self.wheeledbase.wait()
            except:
                pass

            self.log("RECALAGE : colle au mur en x")
            pos = self.wheeledbase.get_position()
            self.wheeledbase.set_position(2000 - 280 / 2, pos[1], 0)

            self.log("RECALAGE : retour arrière")
            self.wheeledbase.goto(*self.actionPoint.point)
            self.wheeledbase.turnonthespot(pi/2)

            self.log("RECALAGE : face au mur en y")
            self.wheeledbase.wait()

            if self.side == self.YELLOW:
                self.wheeledbase.set_velocities(-RECALAGE_VEL, 0)
            else:
                self.wheeledbase.set_velocities(RECALAGE_VEL, 0)

            try:
                self.wheeledbase.wait()
            except:
                pass

            self.log("RECALAGE : colle au mur en y")
            pos = self.wheeledbase.get_position()
            if self.side == self.YELLOW:
                self.wheeledbase.set_position(pos[0], 280 / 2, pos[2])
            else:
                self.wheeledbase.set_position(pos[0], 3000 - 280 / 2, pos[2])

            self.wheeledbase.reset_parameters()
            self.wheeledbase.goto(*self.actionPoint.point, theta=pi / 2)

            self.log("RECALAGE : retour au point d'action")

        else:
            # goto action point
            self.wheeledbase.goto(*self.actionPoint.point)

        self.deployArm.set()
        # correct robot orientation
        self.wheeledbase.turnonthespot(self.actionPoint.theta)
        self.wheeledbase.wait()


    def realize(self):
        
        if self.master.is_active():
            self.master.get_ressource("balance")


        self.arm.start_pump()
        self.arm.move(PREPARE_TAKING_POS_STATIC)
        while not (self.arm.is_arrived()):
            time.sleep(0.1)

        self.arm.move(TAKE_PUCK_STATIC)
        while not (self.arm.is_arrived()):
            time.sleep(0.1)

        # get the current time
        self.init_time = time.time()

        # while not the taking sequence is end
        while not self.armTakingState.is_set():
            if not self.arm.get_atmosphere_pressure():
                # handle puck by sucker
                self.arm.sucker.put_puck(self.puck)
                # move after take
                self.arm.move(TAKE_PUCK_INTER_AFTER_STATIC)
                self.armTakingState.set()
                self.log("TakePuckSync", "Take arm 1... OK")
            else:
                # retry with new pos
                self.log("TakePuckSync", "Take arm 1... Retry")
                self.new_arm_pos = ArmPos(TAKE_PUCK_STATIC.x + 1 , TAKE_PUCK_STATIC.y + 3, TAKE_PUCK_STATIC.phi)
                self.arm.move(self.new_arm_pos)

                while not (self.arm.is_arrived()):
                    time.sleep(0.1)
                # check presure
                if not self.arm.get_atmosphere_pressure():
                    # wait for taking secure
                    time.sleep(0.5)
                    self.arm.sucker.put_puck(self.puck)
                    self.arm.move(TAKE_PUCK_INTER_AFTER_STATIC)
                    self.log("TakePuckSync", "Take arm 1... OK")
                    self.armTakingState.set()
                else:
                    # error stop pump
                    self.arm.stop_pump()
                    self.takeErrorPuck = True
                    self.armTakingState.set()
                    self.arm.move(HOME)
                    self.log("TakePuckSync", "Take arm 1... ERROR")

            while not (self.arm.is_arrived()):
                time.sleep(0.1)
    
    def before(self):
        # wait turnonthespot action before deploy arm
        while not self.deployArm.is_set():
            time.sleep(0.1)
        
        # prepare arm to take
        self.arm.move(PREPARE_TAKING_POS_ROAD)
        while not (self.arm.is_arrived()):
                time.sleep(0.1)

    def after(self):

        # if puck1 1 error go home else action
        if self.takeErrorPuck == False:
                self.arm.move(TANK_POS_INTER)
        else:
                self.arm.move(PREPARE_TAKING_POS_ROAD)

        while not self.arm.is_arrived():
            time.sleep(0.1)
        
        if self.takeErrorPuck == False:
            self.arm.move(self.TankPos[self.arm.tank.index()])

        while not (self.arm.is_arrived() or self.takeErrorPuck):
            time.sleep(0.1)

        if self.takeErrorPuck == False:
            if not self.arm.get_atmosphere_pressure():
                self.arm.tank.put_puck(self.arm.sucker.get_puck())
            else:
                # release puck for next take
                self.arm.sucker.get_puck()
            self.arm.stop_pump()
            self.arm.move(PUT_TANK_AFTER)

        while not (self.arm.is_arrived() or self.takeErrorPuck):
            time.sleep(0.1)

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'TakePuck')


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

        self.wheeledbase    = daughter_cards['wheeledbase']

        # action Points
        self.point          = self.geogebra.get('Distrib{}_{}'.format(self.side,self.distrib_pos))
        self.actionPoint    = ActPoint(self.point, pi/2)

        self.TankPos        = [TAKE_TANK_PUCK1, TAKE_TANK_PUCK2, TAKE_TANK_PUCK3]

        # Taking error event
        self.takeErrorPuck1     = False
        self.takeErrorPuck2     = False

        # taking state event
        self.arm1TakingState    = Event()
        self.arm2TakingState    = Event()

        # handled puck
        self.puck1              = puckFront
        self.puck2              = puckBack

    def moving(self):
        self.wheeledbase.goto(*self.actionPoint.point, theta=self.actionPoint.theta)

    def realize(self):
        # starting two pump
        self.arm1.start_pump()
        self.arm2.start_pump()

        # prepare to taking
        self.arm1.move(PREPARE_TAKING_POS_STATIC)
        self.arm2.move(PREPARE_TAKING_POS_STATIC)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

        # taking
        self.arm1.move(TAKE_PUCK_STATIC)
        self.arm2.move(TAKE_PUCK_STATIC)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

        # get the current time
        self.init_time = time.time()

        # while not the taking sequence is end
        while not (self.arm1TakingState.is_set() and self.arm2TakingState.is_set()):
            if not self.arm1.get_atmosphere_pressure():
                # handle puck by sucker
                self.arm1.sucker.put_puck(self.puck1)
                # move after take
                self.arm1.move(TAKE_PUCK_INTER_AFTER_STATIC)
                self.arm1TakingState.set()
                self.log("TakePuckSync", "Take arm 1... OK")
            else:
                # retry with new pos
                self.log("TakePuckSync", "Take arm 1... Retry")
                self.new_arm_pos = ArmPos(TAKE_PUCK_STATIC.x + 1 , TAKE_PUCK_STATIC.y + 3, TAKE_PUCK_STATIC.phi)
                self.arm1.move(self.new_arm_pos)

                while not (self.arm1.is_arrived()):
                    time.sleep(0.1)
                # check presure
                if not self.arm1.get_atmosphere_pressure():
                    # wait for taking secure
                    time.sleep(0.5)
                    self.arm1.sucker.put_puck(self.puck1)
                    self.arm1.move(TAKE_PUCK_INTER_AFTER_STATIC)
                    self.log("TakePuckSync", "Take arm 1... OK")
                    self.arm1TakingState.set()
                else:
                    # error stop pump
                    self.arm1.stop_pump()
                    self.takeErrorPuck1 = True
                    self.arm1TakingState.set()
                    self.arm1.move(HOME)
                    self.log("TakePuckSync", "Take arm 1... ERROR")

            if not self.arm2.get_atmosphere_pressure():
                # handle puck by sucker
                self.arm2.sucker.put_puck(self.puck2)
                # move after take
                self.arm2.move(TAKE_PUCK_INTER_AFTER_STATIC)
                self.arm2TakingState.set()
                self.log("TakePuckSync", "Take arm 2... OK")
            else:
                # retry with new pos
                self.log("TakePuckSync", "Take arm 2... Retry")
                self.new_arm_pos = ArmPos(TAKE_PUCK_STATIC.x + 1 , TAKE_PUCK_STATIC.y + 3, TAKE_PUCK_STATIC.phi)
                self.arm2.move(self.new_arm_pos)
                # check presure
                while not (self.arm2.is_arrived()):
                    time.sleep(0.1)
                # check presure
                if not self.arm2.get_atmosphere_pressure():
                    # wait for taking secure
                    time.sleep(0.5)
                    self.arm2.sucker.put_puck(self.puck2)
                    self.arm2.move(TAKE_PUCK_INTER_AFTER_STATIC)
                    self.log("TakePuckSync", "Take arm 2... OK")
                    self.arm2TakingState.set()
                else:
                    # error stop pump
                    self.arm2.stop_pump()
                    self.takeErrorPuck2 = True
                    self.arm2TakingState.set()
                    self.arm2.move(HOME)
                    self.log("TakePuckSync", "Take arm 2... ERROR")
            while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
                time.sleep(0.1)

    def before(self):
        self.arm1.move(PREPARE_TAKING_POS_ROAD)
        self.arm2.move(PREPARE_TAKING_POS_ROAD)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

    def after(self):
        if ((self.takeErrorPuck1 == True) and (self.takeErrorPuck2 == True)):
            pass    # Find a way to switch balance3 action
        else:
            self.arm1.move(HOME)
            self.arm2.move(HOME)
        while not (self.arm1.is_arrived() and self.arm2.is_arrived()):
            time.sleep(0.1)

    #override
    def getAction(self):
        return Action(lambda : self.moving(), lambda : self.realize(), self.before, self.after, 'TakePuck_{}'.format(self.distrib_pos))