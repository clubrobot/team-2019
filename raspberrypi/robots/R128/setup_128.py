from robots.setup_wheeledbase import *
from robots.setup_serialtalks import *
from daughter_cards.arm.RobotArm import *
from daughter_cards.arm.ArmController import *
from daughter_cards.display import *
from robots.display_manager import *
from common.funcutils import *
from common.logger import *
import time

log = Logger(Logger.SHOW)

armF = RobotArm(manager, uuid='arm_front')
armB = RobotArm(manager, uuid='arm_back')

armFront = ArmController(armF, 'ARM FRONT', log)
armBack  = ArmController(armB, 'ARM BACK' , log)

# led1 = LEDMatrix(manager, 1)
# led2 = LEDMatrix(manager, 2)
# ssd = SevenSegments(manager)

# disp = DisplayPoints(ssd, led1, led2)

# buttons = ButtonsManager()

def init_robot():
        armFront.stop_pump()
        armBack.stop_pump()

        armFront.go_home()
        armBack.go_home()
        while not (armFront.is_arrived() and armBack.is_arrived):
                time.sleep(0.1)

if __name__ == "__main__":
        init_robot()
