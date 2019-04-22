from robots.setup_wheeledbase import *
from robots.setup_serialtalks import *
from daughter_cards.arm.RobotArm import *
from robots.get_robot_name import *
from daughter_cards.arm.ArmController import *
from daughter_cards.Sensor_IR import *
from robots.sensors_manager import *
from robots.setup_display import *
from common.funcutils import *
from common.geogebra import *
from common.logger import *
from robots.electron import *
import time
import os

log = Logger(Logger.SHOW)

armF = RobotArm(manager, uuid='arm_front')
armB = RobotArm(manager, uuid='arm_back')

electron = Electron(manager)

armFront = ArmController(armF, 'ARM FRONT', log)
armBack  = ArmController(armB, 'ARM BACK' , log)

sensor_front = Sensors(manager, uuid='sensors_avant')
sensor_back  = Sensors(manager, uuid='sensors_arriere')

sens_manager = SensorsManager(wheeledbase, sensor_front, sensor_back)

import os
if ROBOT_ID == R128_ID:
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
