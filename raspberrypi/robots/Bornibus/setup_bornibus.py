#!/usr/bin/env python3
#-*- coding: utf-8 -*-

from robots.setup_serialtalks import *
from robots.setup_wheeledbase import *
from daughter_cards.bornibus_actuators import *
from daughter_cards.display import *
from daughter_cards.sensors_IR import *
from robots.sensors_manager import *
from robots.display_manager import *
from math import pi
from common.logger import *
from common.geogebra import Geogebra
from robots.get_robot_name import *
from robots.wheeledbase_manager import *
from beacons.global_sync import ClientGS
from robots.wheeledbase_manager import * 

if ROBOT_ID == BORNIBUS_ID:
    log = Logger(Logger.BOTH, file_name="/home/pi/logs/start.log")
else:
    log = Logger(Logger.SHOW)

led1 = LEDMatrix(manager, 1)
led2 = LEDMatrix(manager, 2)
ssd = SevenSegments(manager)

disp = DisplayPoints(ssd, led1, led2)

pushers = Pushers(manager)
arm = Arm(manager)
endstops = EndStops(manager)
gripper = Gripper(manager, endstops)

sensorsA = SensorsIR(manager, uuid="sensorsA")
if not sensorsA.is_ready():
    sensorsA = FakeSensorsIR()
sensorsB = SensorsIR(manager, uuid="sensorsB")
if not sensorsB.is_ready():
    sensorsB = FakeSensorsIR()
sensorsC = SensorsIR(manager, uuid="sensorsC")
if not sensorsC.is_ready():
    sensorsC = FakeSensorsIR()

sensorsFront = [Sensor(wheeledbase, "Avant droit",   sensorsA.get_range1, (50, -100), -pi/4, sensorsA.is_ready()),
                Sensor(wheeledbase, "Avant      ",   sensorsB.get_range2, (60, 0), 0, sensorsB.is_ready()),
                Sensor(wheeledbase, "Avant gauche",  sensorsC.get_range1, (50, 100), pi/4, sensorsC.is_ready())]
sensorsBack  = [Sensor(wheeledbase, "Arrière droit", sensorsA.get_range2, (-50, -100), -3*pi/4, sensorsA.is_ready()),
                Sensor(wheeledbase, "Arrière",       sensorsB.get_range1, (-60, 0), pi, sensorsB.is_ready()),
                Sensor(wheeledbase, "Arrière gauche",sensorsC.get_range2, (-50, 100), 3*pi/4, sensorsC.is_ready())]
# sens_manager = SensorsManager(wheeledbase, sensorsFront, sensorsBack, None)

try:
    beacons = ClientGS(1)
    beacons.connect()
    beacons.reset_ressources()
except TimeoutError:
    pass
    
import os
#if ROBOT_ID == BORNIBUS_ID:
#    print("Bornibus")
#    os.chdir("/home/pi/git/clubrobot/team-2019")

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


