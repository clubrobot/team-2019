from common.serialtalks import *
from arduino.newArm import *
from math import pi
import time

a = SerialTalks("/dev/tty.SLAB_USBtoUART")
a.connect(timeout = 10)

x 	= 15
y	= 15
phi = 0

a.send(0X15, INT(8))



# output = a.execute(0X15, FLOAT(x), FLOAT(y), FLOAT(phi))
# print(output.read(FLOAT))
# print(5)
# print("wait")
# time.sleep(5)
# print("next pos")

# output = a.execute(0X15, FLOAT(x), FLOAT(y), FLOAT(phi))
# print(output.read(FLOAT))
# print(5)
# print("wait")
# time.sleep(5)
# print("next pos")



a.disconnect()