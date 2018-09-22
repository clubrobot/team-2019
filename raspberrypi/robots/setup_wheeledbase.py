#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from common.roadmap import RoadMap
from common.geogebra import Geogebra
# Check for the Rapsberry Pi address
# It looks for a file in the current directory, whose name is
# a valid IP address


# Import robot stuff

from common.components import *
from arduino.wheeledbase import *

hostname = ""
if hostname == "":
    print("IP adress :  ")
    hostname = input()
    if (len(hostname) == 0):
        hostname = "127.0.0.1"
    elif (len(hostname.split(".")) == 1):
        hostname = "192.168.1." + hostname
    print("Try reaching raspberry at IP " + hostname + "...")

# Connect to the Raspberry Pi and the different modules
MAX_VEL = 500
MAX_ROT = 6


manager = Manager(hostname)
manager.connect(10)

wheeledbase = WheeledBase(manager)


