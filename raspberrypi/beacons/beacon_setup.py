#!/usr/bin/env python3
#-*- coding: utf-8 -*-

# Common import

import time
from math import pi

# Add the robot directory to the import path

import os, sys, glob
home = os.path.expanduser("~")
for directory in glob.iglob(os.path.join(home, '**/team-2018/raspberrypi'), recursive=True):
	sys.path.append(directory)
	sys.path.append(os.path.join(directory,'robots'))
	sys.path.append(os.path.join(directory,'common'))
	print(directory)
	print(os.path.join(directory,'robots'))
	break

# Check for the Rapsberry Pi address
# It looks for a file in the current directory, whose name is
# a valid IP address

hostname = ""
if hostname=="":
	print ("IP adress :  ")
	hostname=input()	
	if(len(hostname.split("."))==1 and len(hostname)>0):
		hostname="192.168.1."+hostname
	print ("Try reaching raspberry at IP "+hostname+"...")


# Import robot stuff
from common.components         import *
from common.serialtalks        import *
from arduino.wheeledbase        import *
from beacons.anchor		import *
from arduino.tag			import *

# Define temporary modules

# Connect to the Raspberry Pi and the different modules

try:
	m = Manager(hostname)
	m.connect(10)
except:
	raise
	m = Manager()
	m.connect(10)

try:
	b = WheeledBase(m)
except :
	print('\'wheeledbase\' not connected')


try:
	t = Tag(m)
except:
	print('\'tag\' not connected')

try:
	a = Anchor()
except:
	print('\'Anchor\' not connected')


