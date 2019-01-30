#!/usr/bin/env python3
#-*- coding: utf-8 -*-

from robots.setup_serialtalks import *
from daughter_cards.bornibus_actuators import *
from robots.setup_wheeledbase import *

gripper = Gripper(manager)
gripper.open()
pushers = Pushers(manager)
pushers.up()
arm =     Arm(manager)
arm.up()