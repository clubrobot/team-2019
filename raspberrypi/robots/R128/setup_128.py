from robots.setup_wheeledbase import *
from robots.setup_serialtalks import *
from daughter_cards.arm.RobotArm import *
from daughter_cards.arm.ArmController import *
from robots.setup_display import *
from common.funcutils import *
from common.geogebra import *
from common.logger import *
import time
import os

log = Logger(Logger.SHOW)

armF = RobotArm(manager, uuid='arm_front')
armB = RobotArm(manager, uuid='arm_back')

armFront = ArmController(armF, 'ARM FRONT', log)
armBack  = ArmController(armB, 'ARM BACK' , log)

os.chdir("/home/pi/git/clubrobot/team-2019")
roadmap = None
for root, dirs, files in os.walk("."):
        for file in files:
                if file == "128.ggb":
                        roadmap = os.path.join(root, file)
geo = Geogebra(roadmap)


def init_robot():
        armFront.stop_pump()
        armBack.stop_pump()

        armFront.go_home()
        armBack.go_home()
        while not (armFront.is_arrived() and armBack.is_arrived()):
                time.sleep(0.1)

if __name__ == "__main__":
        init_robot()
        pass
