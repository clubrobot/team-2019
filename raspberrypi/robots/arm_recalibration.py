#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
from daughter_cards.arm.RobotArm import *
from daughter_cards.arm.ArmPosition import *
from daughter_cards.arm.puckUtils import *


class ArmRecalibration():
    YELLOW  = 0
    PURPLE  = 1

    SENSOR_TO_ORIGIN = 30 #mm

    def __init__(self, sensors, side, log):
        self.sensors        = sensors
        self.log            = log
        self.side           = side

        # Bind arm in funtion of desired side
        if self.side == self.YELLOW:
            self.arm1_sensor    = self.sensors[0]
            self.arm2_sensor    = self.sensors[1]
        else:
            self.arm1_sensor    = self.sensors[1]
            self.arm2_sensor    = self.sensors[0]

    def readjust_arm1(self, last_desired_pos, x_offset = 0, y_offset = 0, theta_offset = 0):
        self.log("ArmRecalibration","Start ARM 1 readjusts : {}".format(last_desired_pos))

        self.dist = self.arm1_sensor.dist() + self.SENSOR_TO_ORIGIN
        self.log("ArmRecalibration","ARM 1 dist to wall : {} mm".format(self.dist))

        self.delta = ((last_desired_pos.x * 10)- self.dist) / 10
        self.log("ArmRecalibration","ARM 1 current delta : {} cm".format(self.delta))
        # readjust x
        self.new_pos = ArmPos((last_desired_pos.x - self.delta) + x_offset, last_desired_pos.y + y_offset, last_desired_pos.phi + theta_offset)

        return self.new_pos

    def readjust_arm2(self, last_desired_pos, x_offset = 0, y_offset = 0, theta_offset = 0):
        self.log("ArmRecalibration","Start ARM 2 readjusts : {}".format(last_desired_pos))

        self.dist = self.arm2_sensor.dist() + self.SENSOR_TO_ORIGIN
        self.log("ArmRecalibration","ARM 2 dist to wall : {} cm".format(self.dist))

        self.delta = ((last_desired_pos.x * 10)- self.dist) / 10
        self.log("ArmRecalibration","ARM 2 current delta : {} cm".format(self.delta))
        # readjust x
        self.new_pos = ArmPos((last_desired_pos.x - self.delta) + x_offset, last_desired_pos.y + y_offset, last_desired_pos.phi + theta_offset)

        return self.new_pos