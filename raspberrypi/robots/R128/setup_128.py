from robots.setup_wheeledbase import *
from robots.setup_serialtalks import *
from daughter_cards.arm.RobotArm import *
from daughter_cards.arm.ArmController import *
from common.funcutils import *
from common.logger import *
import time

log = Logger(Logger.SHOW)

armF = RobotArm(manager, uuid='arm_front')
armB = RobotArm(manager, uuid='arm_back')

armFront = ArmController(armF, 'ARM FRONT', log)
armBack  = ArmController(armB, 'ARM BACK' , log)

def init_robot():
        wheeledbase.set_velocities(0,0)

        armFront.go_home()
        armBack.go_home()
        while not (armFront.is_arrived() and armBack.is_arrived):
                time.sleep(0.1)

if __name__ == "__main__":
        init_robot()