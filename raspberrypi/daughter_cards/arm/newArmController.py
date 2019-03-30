#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
from daughter_cards.arm.RobotArm import *
from daughter_cards.arm.ArmPosition import *
from daughter_cards.arm.puckUtils import *

TANK_SIZE = 3


class ArmController():
    def __init__(self, arm, name, logger):
        self.name           = name
        self.logger         = logger
        self.arm            = arm

    def is_arrived(self):
        return self.arm.is_arrived()

    def wait(self, timestep = 0.1, timeout = 200):
        self.logger(self.name, "WAIT...")
        init_time = time.time()
        while not self.is_arrived():
            time.sleep(timestep)
            if (time.time()-init_time > timeout):
                raise RuntimeError('{} Timeout'.format(self.name))

    # private func
    def move(self, PosID):
        self.logger(self.name, "Sending Move command: {}".format(PosID))
        self.arm.move(PosID.x, PosID.y, PosID.phi)
        self.logger(self.name, "Start")
        self.arm.run()

    def movePath(self, Path):
        for PosID in Path:
            self.logger(self.name, "Sending Move command: {}".format(PosID))
            self.arm.move(PosID.x, PosID.y, PosID.phi)
        self.logger(self.name, "Start")
        self.arm.run()

    def check_pressure(self, timestep = 0.1, timeout = 100, max_attempt = 5):
        self.logger(self.name, "WAIT FOR SUFFISANT PRESSURE")
        init_time = time.time()
        attempt   = 0
        while self.get_atmosphere_pressure():
            time.sleep(timestep)
            if (time.time()-init_time > timeout):
                if(attempt >= max_attempt):
                    return False
                attempt += 1
        return True

    def start_pump(self):
        self.logger(self.name, "Start Pump")
        self.arm.start_pump()

    def stop_pump(self):
        self.logger(self.name, "Stop Pump")
        self.arm.stop_pump()

    def get_pressure_kpa(self):
        return self.arm.get_pressure_kpa()

    def get_atmosphere_pressure(self):
        return self.arm.atmosphere_pressure()

    def go_home(self):
        self.movePath(HomePath)
        self.wait()

    def prepare_to_take(self):
        self.logger(self.name, "Prepare to take")
        self.move(PREPARE_TAKING_POS)
        self.wait()

    def take(self):
        self.logger(self.name, "Take")
        self.move(TAKE_PUCK)
        self.wait()
        if(self.check_pressure()):
            self.logger(self.name, "Succes")
            self.move(TAKE_PUCK_INTER_AFTER)
            self.wait()
        else:
            self.go_home()
            raise RuntimeError('{} Take puck Fail'.format(self.name))

    def put_in_tank(self):
        self.logger(self.name, "Put in tank")
        # self.movePath(self.PutPathList[self.tank.get_index()])
        # #self.tank.put_puck(self.currentPuck)
        # self.wait()
        # self.stop_pump()
        # self.move(TANK_POS_INTER)
        # self.wait()
        # self.movePath(HomePath)
        # self.wait()

    def take_in_tank(self):
        self.logger(self.name, "Take in tank")
        # self.currentPuck = self.tank.get_puck()
        # self.movePath(self.TakePathList[self.tank.get_index()])
        # self.wait()

    def put_in_balance(self):
        self.logger(self.name, "Put in balance")
        # self.move(BALANCE)
        # self.currentPuck = None
        # self.wait()