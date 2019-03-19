from math import pi
import time
import json

from robots.actions_128 import TakeSyncronized, PutBalance, TakeFront

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
    puckLittle1 = geo.get("PuckLittle1")
    puckLittle2 = geo.get("PuckLittle2")
    balance     = geo.get("Balance"+str(side))

    PathDistrib = geo.getall("PathDistrib"+str(side)+"_*")
    print(puck1)
    print(puck2)
    print(puck3)
    print(balance)
    print(PathDistrib)
    # 128 actions
    print("Creating actions :")
    takeSync    = TakeSyncronized(0, armController, log)
    takeFront   = TakeFront      (0, armController, log)
    putBalance  = PutBalance     (0, armController, log)
    print("End")
    
    wheeledbase.set_position(*start, -pi)
    print("robot placé : ", wheeledbase.get_position())

    starttime = time.time()
    wheeledbase.goto(*puck1, theta=pi/2)
    print("robot placé : ", wheeledbase.get_position())
   

    wheeledbase.goto(*puck2, theta=pi/2)
    print("robot placé : ", wheeledbase.get_position())

    takeSync.realize()

    wheeledbase.goto(*puck3, theta=pi/2)
    

    takeSync.realize()

    wheeledbase.goto(*puck4, theta=pi/2)
    

    takeSync.realize()

    wheeledbase.goto(*balance, theta=pi/2)


    putBalance.realize()

   
    wheeledbase.purepursuit(PathDistrib, finalangle=pi)
    wheeledbase.wait()


    
    wheeledbase.goto(*puckLittle1, theta=pi/2)


    takeSync.realize()

    wheeledbase.goto(*puckLittle2, theta=pi/2)


    takeSync.realize()

    wheeledbase.turnonthespot(pi)
    wheeledbase.wait()

    PathDistrib.reverse()
    wheeledbase.purepursuit(PathDistrib, finalangle=pi/2)
    wheeledbase.wait()



    wheeledbase.goto(*balance, theta=pi/2)


    putBalance.realize()
    print("Match Time : ")
    print(time.time() - starttime)

    wheeledbase.goto(*start, theta=-pi)
    
    log("MAIN","END !")
    input()