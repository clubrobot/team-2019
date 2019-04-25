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
from common.geogebra import Geogebra
from robots.get_robot_name import *

led1 = LEDMatrix(manager, 1)
led2 = LEDMatrix(manager, 2)
ssd = SevenSegments(manager)

disp = DisplayPoints(ssd, led1, led2)

pushers = Pushers(manager)
arm = Arm(manager)
endstops = EndStops(manager)
gripper = Gripper(manager, endstops)

sensor_front = Sensors(manager, uuid='sensors_avant')
sensor_back  = Sensors(manager, uuid='sensors_arriere')

sens_manager = SensorsManager(wheeledbase, sensor_front, sensor_back)

import os
if ROBOT_ID == BORNIBUS_ID:
    print("Bornibus")
    os.chdir("/home/pi/git/clubrobot/team-2019")

roadmap = None
for root, dirs, files in os.walk("."):
    for file in files:
        if file == "Bornibus.ggb":
            roadmap = os.path.join(root, file)
geo = Geogebra(roadmap)


def init_robot():
    gripper.open()
    pushers.up()
    arm.up()
    wheeledbase.stop()


if __name__ == "__main__":
    init_robot()
