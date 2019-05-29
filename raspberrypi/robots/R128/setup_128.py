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
from beacons.global_sync import ClientGS
from daughter_cards.sensors_IR import *
from robots.sensors_manager import *
from robots.wheeledbase_manager import * 
import time
import warnings
warnings.filterwarnings('error')

if ROBOT_ID == R128_ID:
    log = Logger(Logger.BOTH, file_name="/home/pi/logs/start.log")
else:
    log = Logger(Logger.SHOW)

armF = RobotArm(manager, uuid='arm_front')
armB = RobotArm(manager, uuid='arm_back')

electron = Electron(manager)

armFront = ArmController(armF, 'ARM FRONT', log)
armBack  = ArmController(armB, 'ARM BACK' , log)

sensorsA = SensorsIR(manager, uuid="sensorsA")
if not sensorsA.is_ready():
    sensorsA = FakeSensorsIR()
sensorsB = SensorsIR(manager, uuid="sensorsB")
if not sensorsB.is_ready():
    sensorsB = FakeSensorsIR()
sensorsC = SensorsIR(manager, uuid="sensorsC")
if not sensorsC.is_ready():
    sensorsC = FakeSensorsIR()
sensorsD = SensorsIR(manager, uuid="sensorsD")
if not sensorsD.is_ready():
    sensorsD = FakeSensorsIR()



sensorsFront = [Sensor(wheeledbase, "Avant gauche",   sensorsD.get_range2, (110, 110), pi/4, sensorsD.is_ready()),
                Sensor(wheeledbase, "Avant      ",    sensorsD.get_range1, (140, -50), 0, sensorsD.is_ready()),
                Sensor(wheeledbase, "Avant droit",    sensorsC.get_range2, (110, -110), -pi/4, sensorsC.is_ready())]

sensorsLat =   [Sensor(wheeledbase, "Lateral avant",  sensorsC.get_range1, (60, -130), -pi/2, sensorsC.is_ready()),
                Sensor(wheeledbase, "Lateral arriere",sensorsB.get_range1, (-60, -130), -pi/2, sensorsB.is_ready())]

sensorsBack  = [Sensor(wheeledbase, "Arrière droit",  sensorsB.get_range2, (-110, -110), -3*pi/4, sensorsB.is_ready()),
                Sensor(wheeledbase, "Arrière",        sensorsA.get_range1, (-110, -50), pi, sensorsA.is_ready()),
                Sensor(wheeledbase, "Arrière gauche", sensorsA.get_range2, (-110, 110), 3*pi/4, sensorsA.is_ready())]
sens_manager = SensorsManager(wheeledbase, sensorsFront, sensorsBack, sensorsLat)

import os
if ROBOT_ID == R128_ID:
        print("128")
        os.chdir("/home/pi/git/clubrobot/team-2019")

roadmap = None
for root, dirs, files in os.walk("."):
        for file in files:
                if file == "128.ggb":
                        roadmap = os.path.join(root, file)
if roadmap:
        geo = Geogebra(roadmap)
else:
        geo = None

try:
    beacons = ClientGS(2)
    beacons.connect()
    beacons.reset_ressources()
except TimeoutError:
    pass

def init_robot():
        armFront.stop_pump()
        armBack.stop_pump()

        armFront.go_home()
        armBack.go_home()

if __name__ == "__main__":
        from robots.arm_recalibration import *  
        #a = ArmRecalibration(sensorsLat,1,log)
        init_robot()
        #daughter_cards = dict( wheeledbase     = wheeledbase, 
        #                            armFront        = armFront,
        #                            armBack         = armBack,
        #                            display         = disp,
        #                            master          = beacons)
        #mover = Mover(daughter_cards, log, sensorsFront, sensorsBack)
        #wheeledbase.set_position(780, 2595, -pi/2)
        #while True:
        #        mover.goto(780,1000,safe_mode=True)
        #        mover.goto(780,2595,safe_mode=True)
