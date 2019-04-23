#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
from daughter_cards.arm.RobotArm import *
from daughter_cards.arm.ArmPosition import *
from daughter_cards.arm.puckUtils import *
from daughter_cards.arm.puckUtils import *



class ArmController():
    TANK_SIZE = 3
    def __init__(self, arm, name, log):
        self.name           = name
        self.log            = log
        self.arm            = arm
        # Puck Management
        self.tank           = PuckTank('TANK '+self.name, self.TANK_SIZE, log)

    # private func
    def move(self, PosID):
        self.log(self.name, "Sending Move command: {}".format(PosID))
        self.arm.move(PosID.x, PosID.y, PosID.phi)
        self.log(self.name, "Start")
        self.arm.run()

    def movePath(self, Path):
        for PosID in Path:
            self.log(self.name, "Sending Move command: {}".format(PosID))
            self.arm.move(PosID.x, PosID.y, PosID.phi)
        self.log(self.name, "Start")
        self.arm.run()

    def is_arrived(self):
        return self.arm.is_arrived()

    def wait(self, timestep = 0.1, timeout = 200):
        self.log(self.name, "WAIT...")
        init_time = time.time()
        while not self.is_arrived():
            time.sleep(timestep)
            if (time.time()-init_time > timeout):
                raise RuntimeError('{} Timeout'.format(self.name))

    def check_pressure(self, timestep = 0.1, timeout = 100, max_attempt = 5):
        self.log(self.name, "WAIT FOR SUFFISANT PRESSURE")
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
        self.log(self.name, "Start Pump")
        self.arm.start_pump()

    def stop_pump(self):
        self.log(self.name, "Stop Pump")
        self.arm.stop_pump()

    def get_pressure_kpa(self):
        return self.arm.get_pressure_kpa()

    def get_atmosphere_pressure(self):
        return self.arm.atmosphere_pressure()

    def go_home(self):
        self.log(self.name, "Go Home")
        self.movePath(HomePath)

    # def prepare_to_take_road(self):
    #     self.log(self.name, "Prepare to take")
    #     self.move(PREPARE_TAKING_POS_ROAD)

    # def prepare_to_take(self):
    #     self.log(self.name, "Prepare to take")
    #     self.move(PREPARE_TAKING_POS_STATIC)

    # def take(self, puck):
    #     self.log(self.name, "Take")
    #     self.move(TAKE_PUCK_STATIC)

    # def after_take(self):
    #     self.log(self.name, "after Take")
    #     self.move(TAKE_PUCK_INTER_AFTER_STATIC)