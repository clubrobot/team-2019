from robots.setup_wheeledbase import *
from robots.setup_serialtalks import *
from daughter_cards.arm.RobotArm import *
from robots.get_robot_name import *
from daughter_cards.arm.ArmController import *
from robots.setup_display import *
from common.funcutils import *
from common.geogebra import *
from common.logger import *
from daughter_cards.electron import *
from daughter_cards.Sensor_IR import *
from robots.sensors_manager import *
import time


log = Logger(Logger.WRITE)

armF = RobotArm(manager, uuid='arm_front')
armB = RobotArm(manager, uuid='arm_back')

electron = Electron(manager)

armFront = ArmController(armF, 'ARM FRONT', log)
armBack  = ArmController(armB, 'ARM BACK' , log)

sensorsA = Sensors(manager, uuid="sensorsA")
sensorsB = Sensors(manager, uuid="sensorsB")
sensorsC = Sensors(manager, uuid="sensorsC")
sensorsD = Sensors(manager, uuid="sensorsD")
sensorsFront = [Sensor("Avant droit",   sensorsB.get_range1, (110, -110), -pi/4),
                Sensor("Avant      ",   sensorsB.get_range2, (140, -50), 0),
                Sensor("Avant gauche",  sensorsC.get_range1, (110, 110), pi/4)]
sensorsBack  = [Sensor("Arrière droit", sensorsA.get_range1, (-110, -110), -3*pi/4),
                Sensor("Arrière",       sensorsA.get_range2, (-110, -50), pi),
                Sensor("Arrière gauche",sensorsD.get_range2, (-110, 110), 3*pi/4)]

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
