from robots.setup_wheeledbase import *
from daughter_cards.RobotArm import *
import json

wheeledbase.set_velocities(0, 0)

arm = RobotArm(manager)
arm.setup_tank_size(3)
arm.stop_pump()
arm.go_home()