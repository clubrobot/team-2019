#!/usr/bin/env python3
#-*- coding: utf-8 -*-

from robots.setup_serialtalks import *
from daughter_cards.bornibus_actuators import *

gripper = Gripper(manager)
pushers = Pushers(manager)
arm =     Arm(manager)