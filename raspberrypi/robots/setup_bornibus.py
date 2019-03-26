#!/usr/bin/env python3
#-*- coding: utf-8 -*-

from robots.setup_serialtalks import *
from robots.setup_wheeledbase import *
from daughter_cards.bornibus_actuators import *
from daughter_cards.display import *
#from daughter_cards.buttons import *
#from robots.bornibus_buttons import *
from robots.display_manager import *
#from robots.buttons_manager import *
from math import pi
from common.geogebra import Geogebra

gripper = Gripper(manager)
pushers = Pushers(manager)
arm = Arm(manager)
ss = SevenSegments(manager)
left_eye = LEDMatrix(manager, 1)
right_eye = LEDMatrix(manager, 2)
disp_man = DisplayPoints(ss, right_eye, left_eye)

def init_robot():
    gripper.open()
    pushers.up()
    arm.up()

if __name__ == "__main__":
    init_robot()
