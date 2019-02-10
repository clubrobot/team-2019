from robots.setup_wheeledbase import *
from daughter_cards.RobotArm import *
import json

arm = RobotArm(manager)
arm.go_home()