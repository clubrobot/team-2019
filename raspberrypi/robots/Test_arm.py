from setup_128 import *
import time
import json

with open('../daughter_cards/ArmPosition.json') as f:
    data = json.load(f)

wheeledbase.set_velocities(0,0)
arm.move(data["TANK_POS_INTER"])
time.sleep(1)
arm.start_pump()
arm.move(data["TANK_POS_3"])
time.sleep(1)
arm.move(data["BALANCE"])
time.sleep(2)
arm.stop_pump()
time.sleep(2)
arm.move(data["TANK_POS_INTER"])
time.sleep(10)


# arm.move(data["TAKE_PUCK_INTER"])
# time.sleep(1)
# arm.start_pump()
# time.sleep(1)
# arm.move(data["TAKE_PUCK"])
# time.sleep(1)
# arm.move(data["TANK_POS_INTER"])
# time.sleep(1)
# arm.move(data["TANK_POS_2"])

# time.sleep(1)
# arm.move(data["TAKE_PUCK_INTER"])
# time.sleep(1)
# arm.move(data["TAKE_PUCK"])
# time.sleep(1)
# arm.move(data["TANK_POS_INTER"])
# time.sleep(1)
# arm.move(data["TANK_POS_1"])
# time.sleep(1)
# arm.move(data["TANK_POS_INTER"])
# time.sleep(5)