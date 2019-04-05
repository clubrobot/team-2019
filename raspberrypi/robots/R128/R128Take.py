from math import pi
import time
import json
from common.geogebra import Geogebra

color = {'YELLOW':0, 'PURPLE':1}

if __name__ == '__main__':
    from robots.R128.setup_128 import *
    
    print(color)
    side = color['PURPLE']

    geo = Geogebra("128.ggb")
    print("Map chargée")

    print("load points :")
    start       = geo.get("Start{}".format(side))
    puck1       = geo.get("Puck{}_1".format(side))
    puck2       = geo.get("Puck{}_2".format(side))
    puck3       = geo.get("Puck{}_3".format(side))
    puck4       = geo.get("Puck{}_4".format(side))
    puckLittle1 = geo.get("PuckLittle1")
    puckLittle2 = geo.get("PuckLittle2")
    balance     = geo.get("Balance{}".format(side))

    PathDistrib = geo.getall("PathDistrib{}_*".format(side))
    print(puck1)
    print(puck2)
    print(puck3)
    print(balance)
    print(PathDistrib)

    #128 actions
    print("Creating actions :")
    wheeledbase.set_position(*start, -pi)
    print("robot placé : ", wheeledbase.get_position())

    starttime = time.time()

    wheeledbase.goto(*puck1, theta=pi/2)
    while not wheeledbase.goto.end():
        time.sleep(0.1)

    time.sleep(5)

    wheeledbase.goto(*puck2, theta=pi/2)
    armFront.prepare_to_take()
    armBack.prepare_to_take()

    armFront.start_pump()
    armBack.start_pump()

    while not (wheeledbase.goto.end() and armFront.prepare_to_take.end() and armBack.prepare_to_take.end()):
        time.sleep(0.1)

    armFront.take()
    armBack.take()
    
    while not (armFront.take.end() and armBack.take.end()):
        time.sleep(0.1)

    armFront.put_in_tank()
    armBack.put_in_tank()

    while not (armFront.put_in_tank.end() and armBack.put_in_tank.end()):
        time.sleep(0.1)

    
    wheeledbase.goto(*puck3, theta=pi/2)
    armFront.prepare_to_take()
    armBack.prepare_to_take()


    armFront.start_pump()
    armBack.start_pump()

    while not (wheeledbase.goto.end() and armFront.prepare_to_take.end() and armBack.prepare_to_take.end()):
        time.sleep(0.1)

    armFront.take()
    armBack.take()

    while not (armFront.take.end() and armBack.take.end()):
        time.sleep(0.1)

    armFront.put_in_tank()
    armBack.put_in_tank()

    while not (armFront.put_in_tank.end() and armBack.put_in_tank.end()):
        time.sleep(0.1)
    
    wheeledbase.goto(*puck4, theta=pi/2)
    armFront.prepare_to_take()
    armBack.prepare_to_take()

    armFront.start_pump()
    armBack.start_pump()

    while not (wheeledbase.goto.end() and armFront.prepare_to_take.end() and armBack.prepare_to_take.end()):
        time.sleep(0.1)

    armFront.take()
    armBack.take()

    while not (armFront.take.end() and armBack.take.end()):
        time.sleep(0.1)

    armFront.put_in_tank()
    armBack.put_in_tank()

    while not (armFront.put_in_tank.end() and armBack.put_in_tank.end()):
        time.sleep(0.1)
    input()
