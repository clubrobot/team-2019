from robots.setup_wheeledbase import *
from daughter_cards.RobotArm import *
import json

arm = RobotArm(manager)
arm.setup_tank_size(3)
arm.go_home()