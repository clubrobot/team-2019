from math import pi
import time
import json

from robots.actions_128 import TakeSyncronized, PutBalance, TakeFront
from common.logger      import Logger

YELLOW = 0
PURPLE = 1

if __name__ == '__main__':
    from robots.setup_128 import *

    side = PURPLE

    geo = Geogebra("128.ggb")
    print("Map chargée")

    print("load points :")
    start       = geo.get("Start"+str(side))
    puck1       = geo.get("Puck"+str(side)+"_1")
    puck2       = geo.get("Puck"+str(side)+"_2")
    puck3       = geo.get("Puck"+str(side)+"_3")
    puck4       = geo.get("Puck"+str(side)+"_4")
    puckLittle  = geo.get("PuckLittle"+str(side))
    balance     = geo.get("Balance"+str(side))

    PathDistrib = geo.getall("PathDistrib"+str(side)+"_*")
    print(puck1)
    print(puck2)
    print(puck3)
    print(balance)
    print(PathDistrib)
    # 128 actions
    print("Creating actions :")
    takeSync    = TakeSyncronized(0, armController, Logger(Logger.SHOW))
    takeFront   = TakeFront      (0, armController, Logger(Logger.SHOW))
    putBalance  = PutBalance     (0, armController, Logger(Logger.SHOW))
    print("End")
    
    wheeledbase.set_position(*start, -pi)
    print("robot placé : ", wheeledbase.get_position())


    wheeledbase.goto(*puck1, theta=pi/2)
    print("robot placé : ", wheeledbase.get_position())
    time.sleep(1)

    wheeledbase.goto(*puck2, theta=pi/2)
    time.sleep(1)

    takeSync.realize()

    wheeledbase.goto(*puck3, theta=pi/2)
    time.sleep(1)

    takeSync.realize()

    wheeledbase.goto(*puck4, theta=pi/2)
    time.sleep(1)

    takeSync.realize()

    wheeledbase.goto(*balance, theta=pi/2)
    time.sleep(1)

    putBalance.realize()

    armController.go_home()

    wheeledbase.purepursuit(PathDistrib, finalangle=pi/2)
    wheeledbase.wait()

    time.sleep(1)

    takeFront.realize()
    
    wheeledbase.goto(*puckLittle, theta=pi/2)
    time.sleep(1)

    takeSync.realize()
    del PathDistrib[-1]
    PathDistrib.reverse()
    PathDistrib.append(balance)
    print(PathDistrib)
    wheeledbase.purepursuit(PathDistrib, finalangle=pi/2)
    wheeledbase.wait()

    putBalance.realize()

    wheeledbase.goto(*start, theta=-pi)
    time.sleep(1)

    # input()