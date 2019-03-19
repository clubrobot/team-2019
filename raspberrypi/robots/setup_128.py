from robots.setup_wheeledbase import *
from daughter_cards.arm.RobotArm import *
from daughter_cards.arm.ArmController import *
from common.geogebra import Geogebra
from common.logger      import Logger
# Configure wheeledbase
wheeledbase.set_velocities(0, 0)
log = Logger(Logger.SHOW)
# Configure arm
robArm = RobotArm(manager)

# Configure arm controller
armController = ArmController(robArm, "ARM FRONT")
armController.set_logger(log)
armController.setup_tank_size(3)

# Start arm
armController.stop_pump()
armController.go_home()