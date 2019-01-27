from common.serialtalks import *
from arduino.newArm import *
from math import pi
import time

a = SerialTalks("/dev/tty.SLAB_USBtoUART")
a.connect()

output = a.execute(0X14)

print(output.read(FLOAT,FLOAT,FLOAT))

a.send(0X10, FLOAT(10.0), FLOAT(10.0), FLOAT(0))

a.send(0X10, FLOAT(11.0), FLOAT(10.0), FLOAT(0))

a.send(0X10, FLOAT(7.0), FLOAT(0.0), FLOAT(-(pi/2)))


output = a.execute(0X14)

print(output.read(FLOAT,FLOAT,FLOAT))

output = a.execute(0X14)

print(output.read(FLOAT,FLOAT,FLOAT))

output = a.execute(0X14)

print(output.read(FLOAT,FLOAT,FLOAT))







a.disconnect()