from common.serialtalks import *
from daughter_cards.RobotArm import *
import time
from math import pi
import json

with open('daughter_cards/ArmPosition.json') as f:
    data = json.load(f)


s = SerialTalks("/dev/tty.SLAB_USBtoUART")

r = RobotArm(s)
r.connect()

r.start_pump()
r.move(data["TAKE_PUCK_INTER"])
r.move(data["TAKE_PUCK"])
r.run_batch()
time.sleep(1)

r.move(data["TANK_POS_INTER"])
r.move(data["TANK_POS_1"])
r.run_batch()
time.sleep(2)
r.stop_pump()
time.sleep(2)

r.start_pump()
r.move(data["TAKE_PUCK_INTER"])
r.move(data["TAKE_PUCK"])
r.run_batch()
time.sleep(1)

r.move(data["TANK_POS_INTER"])
r.move(data["TANK_POS_2"])
r.run_batch()

time.sleep(2)

r.stop_pump()
time.sleep(2)

r.start_pump()
r.move(data["TAKE_PUCK_INTER"])
r.move(data["TAKE_PUCK"])
r.run_batch()
time.sleep(1)

r.move(data["TANK_POS_INTER"])
r.move(data["TANK_POS_3"])
r.run_batch()
time.sleep(2)
r.stop_pump()
