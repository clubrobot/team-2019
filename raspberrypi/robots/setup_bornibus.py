#!/usr/bin/env python3
#-*- coding: utf-8 -*-

from robots.setup_serialtalks import *
from robots.setup_wheeledbase import *
from daughter_cards.bornibus_actuators import *
from daughter_cards.display import *
from robots.display_manager import *

gripper = Gripper(manager)
pushers = Pushers(manager)
arm =     Arm(manager)

ss = SevenSegments(manager)
right_eye = LEDMatrix(manager, 1)
left_eye = LEDMatrix(manager, 2)
disp_man = DisplayPoints(ss, right_eye, left_eye)