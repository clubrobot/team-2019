#!/usr/bin/env python3
#-*- coding: utf-8 -*-

# Common import

import time
from math import pi

# Add the robot directory to the import path

import os, sys, glob

# Import robot stuff
from common.components         import *
#from common.serialtalks        import *
from daughter_cards.bornibus_actuators           import *
# Define temporary modules

# Connect to the Raspberry Pi and the different modules

print('Connecting')
try:
	m = Manager()
	m.connect(10)
except :
	print("Error while connecting -> Check if 'Robot Server' is running !!")

g = Gripper(m)

p = Pushers(m)

a = Arm(m)

e = EndStops(m)

#Initialisation 