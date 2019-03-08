from robots.setup_wheeledbase import *
from daughter_cards.arm.RobotArm import *
from daughter_cards.arm.ArmController import *

# Configure wheeledbase
wheeledbase.set_velocities(0, 0)

# Configure arm
robArm = RobotArm(manager)

# Configure arm controller
armController = ArmController(robArm)
armController.setup_tank_size(3)

# Start arm
armController.stop_pump()
armController.go_home()
