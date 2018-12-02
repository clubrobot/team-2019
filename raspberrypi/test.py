from common.serialtalks import *
from arduino.newArm import *
from math import pi
import time

a = arm()
a.connect()

x 	= 10
y	= 10
phi = 0

trajectory_time = a.goto_directly(x, y, phi)
print(trajectory_time)

print("wait")
time.sleep(trajectory_time)
print("next pos")

x 	= 0
y	= 10
phi = 0

trajectory_time = a.goto_directly(x, y, phi)
print(trajectory_time)

print("wait")
time.sleep(trajectory_time)
print("next pos")



a.disconnect();