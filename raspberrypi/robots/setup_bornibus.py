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

#bmap = Geogebra("roadmap.ggb")
points = dict()


led1 = LEDMatrix(manager, 1)
led2 = LEDMatrix(manager, 2)
ssd = SevenSegments(manager)

disp = DisplayPoints(ssd, led1, led2)

gripper = Gripper(manager)
gripper.open()
pushers = Pushers(manager)
pushers.up()
arm = Arm(manager)
arm.up()

"""
button_manager = BornibusButtonsManager(ButtonCard(manager))
while button_manager.team is none:
    if button_manager.team==1:
        points[Ini]=bmap.get("IniM")
        points[Gold1]=bmap.get("Gold1M")
        points[Gold2]=bmap.get("Gold2M")
        points[Gold3]=bmap.get("Gold3M")
        points[Gold4]=bmap.get("Gold4M")
    elif button_manager.team==2:
        points[Ini]=bmap.get("IniO")
        points[Gold1]=bmap.get("Gold1O")
        points[Gold2]=bmap.get("Gold2O")
        points[Gold3]=bmap.get("Gold3O")
        points[Gold4]=bmap.get("Gold4O")
ss = SevenSegments(manager)
left_eye = LEDMatrix(manager, 1)
right_eye = LEDMatrix(manager, 2)
disp_man = DisplayPoints(ss, right_eye, left_eye)
"""