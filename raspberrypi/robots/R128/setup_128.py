from robots.setup_wheeledbase import *
from robots.setup_serialtalks import *
from daughter_cards.arm.RobotArm import *
from daughter_cards.arm.newArmController import *
from common.funcutils import *
from common.logger import *
import time

log = Logger(Logger.SHOW)

armF = RobotArm(manager, uuid='arm_front')
armB = RobotArm(manager, uuid='arm_back')

armFront = ArmController(armF, 'ARM FRONT', log)
armBack  = ArmController(armB, 'ARM BACK' , log)

armFront.go_home = ThreadMethod(armFront.go_home)

def init_robot():
        wheeledbase.set_velocities(0,0)

        armFront.go_home()
        armBack.go_home()
        while not (armFront.go_home.end() and armBack.go_home.end()):
                time.sleep(0.1)



if __name__ == "__main__":
        init_robot()
