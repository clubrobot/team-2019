#!/usr/bin/env python3
#-*- coding: utf-8 -*-

from robots.setup_serialtalks import *
from robots.setup_wheeledbase import *
from daughter_cards.bornibus_actuators import *
from daughter_cards.display import *
from daughter_cards.Sensor_IR import *
from robots.sensors_manager import *
from robots.display_manager import *
from math import pi
from common.logger import *
from common.geogebra import Geogebra
from robots.get_robot_name import *

log = Logger(Logger.SHOW)

led1 = LEDMatrix(manager, 1)
led2 = LEDMatrix(manager, 2)
ssd = SevenSegments(manager)

disp = DisplayPoints(ssd, led1, led2)

pushers = Pushers(manager)
arm = Arm(manager)
endstops = EndStops(manager)
gripper = Gripper(manager, endstops)

sensorsA = Sensors(manager, uuid="sensorsA")
sensorsB = Sensors(manager, uuid="sensorsB")
sensorsC = Sensors(manager, uuid="sensorsC")
sensorsFront = [Sensor("Avant droit",   sensorsA.get_range1, (50, -100), -pi/4),
                Sensor("Avant      ",   sensorsB.get_range2, (60, 0), 0),
                Sensor("Avant gauche",  sensorsC.get_range1, (50, 100), pi/4)]
sensorsBack  = [Sensor("Arrière droit", sensorsA.get_range2, (-50, -100), -3*pi/4),
                Sensor("Arrière",       sensorsB.get_range1, (-60, 0), pi),
                Sensor("Arrière gauche",sensorsC.get_range2, (-50, 100), 3*pi/4)]
sens_manager = SensorsManager(wheeledbase, sensorsFront, sensorsBack, None)

import os
if ROBOT_ID == BORNIBUS_ID:
    print("Bornibus")
    os.chdir("/home/pi/git/clubrobot/team-2019")

roadmap = None
for root, dirs, files in os.walk("."):
    for file in files:
        if file == "Bornibus.ggb":
            roadmap = os.path.join(root, file)
if roadmap:
    geo = Geogebra(roadmap)


def init_robot():
    gripper.open()
    pushers.up()
    arm.up()
    wheeledbase.stop()


if __name__ == "__main__":
    init_robot()
