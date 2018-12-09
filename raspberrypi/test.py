from common.serialtalks import *
from arduino.newArm import *
from math import pi
import time

a = SerialTalks("/dev/tty.SLAB_USBtoUART")
a.connect(timeout = 10)

x 	= 15
y	= 15
phi = 0

output = a.execute(0X17, FLOAT(x), FLOAT(y), FLOAT(phi))
trajectory_time = output.read(INT)
print(trajectory_time)
time.sleep(1)

output = a.execute(0X17, FLOAT(x), FLOAT(y), FLOAT(phi))
trajectory_time = output.read(INT)
print(trajectory_time)
time.sleep(1)

output = a.execute(0X17, FLOAT(x), FLOAT(y), FLOAT(phi))
trajectory_time = output.read(INT)
print(trajectory_time)
time.sleep(1)

# output = a.execute(0X15, FLOAT(x), FLOAT(y), FLOAT(phi))
# trajectory_time = output.read(FLOAT)
# print(trajectory_time)
# print("wait")
# time.sleep(trajectory_time)
# print("next pos")

# output = a.execute(0X15, FLOAT(x), FLOAT(y), FLOAT(phi))
# trajectory_time = output.read(FLOAT)
# print(trajectory_time)
# print("wait")
# time.sleep(trajectory_time)
# print("next pos")

# output = a.execute(0X15, FLOAT(x), FLOAT(y), FLOAT(phi))
# trajectory_time = output.read(FLOAT)
# print(trajectory_time)
# print("wait")
# time.sleep(trajectory_time)
# print("next pos")



a.disconnect()