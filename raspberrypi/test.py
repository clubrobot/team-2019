from common.serialtalks import *
from math import pi
import time

s = SerialTalks("/dev/tty.SLAB_USBtoUART")
s.connect();

MOVE_DIRECTLY_OPCODE    = 0X10
MOVE_PATH_OPCODE        = 0X11
IS_ARRIVED_OPCODE       = 0X12

x 	= 10
y	= 10
phi = 0


output = s.execute(MOVE_DIRECTLY_OPCODE,FLOAT(15.0), FLOAT(10.0), FLOAT(0.0))
trajectory_time = output.read(FLOAT)
print(trajectory_time)

print("wait")
time.sleep(trajectory_time)
print("next pos")



s.disconnect();