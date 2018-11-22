from common.serialtalks import *
from math import pi
import time

s = SerialTalks("/tmp/arduino/esp32")
s.connect();

MOVE_DIRECTLY_OPCODE    = 0X10
MOVE_PATH_OPCODE        = 0X11
IS_ARRIVED_OPCODE       = 0X12


x 	= 10
y	= 10
phi = 0


output = s.execute(MOVE_DIRECTLY_OPCODE,FLOAT(5.0), FLOAT(5.0), FLOAT(0.0))
trajectory_time = output.read(FLOAT)
print(trajectory_time)

# Wait for 5 seconds
time.sleep(trajectory_time)

output = s.execute(IS_ARRIVED_OPCODE)
is_arrived = output.read(INT)

while (is_arrived is not 0X00):
    output = s.execute(IS_ARRIVED_OPCODE)
    is_arrived = output.read(INT)
    time.sleep(0.100)


output = s.execute(MOVE_PATH_OPCODE,FLOAT(9.0), FLOAT(5.0), FLOAT(0.0))
trajectory_time = output.read(FLOAT)
print(trajectory_time)

# Wait for 5 seconds
time.sleep(trajectory_time)

output = s.execute(IS_ARRIVED_OPCODE)
is_arrived = output.read(INT)

while (is_arrived is not 0X00):
    output = s.execute(IS_ARRIVED_OPCODE)
    is_arrived = output.read(INT)
    time.sleep(0.100)





s.disconnect();