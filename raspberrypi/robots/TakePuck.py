from math import pi
import time
import json

from common.geogebra import Geogebra
from setup_128 import *


test_map = Geogebra("TakePuck.ggb")
print("Map chargée")

print("load points :")
puck1   = test_map.get("Puck1")
puck2   = test_map.get("Puck2")
puck3   = test_map.get("Puck3")
balance = test_map.get("Balance")

print(puck1)
print(puck2)
print(puck3)
print(balance)

wheeledbase.set_position(755, 2681, -pi)
print("robot placé : ", wheeledbase.get_position())

input()

wheeledbase.goto(*puck1, theta=pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

print("robot arrivé sur puck1")

input()

arm.take_puck_in_distributor()
arm.put_in_tank()
arm.go_home()
input()

wheeledbase.goto(*puck2, theta=pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

print("robot arrivé sur puck2")

input()

arm.take_puck_in_distributor()
arm.put_in_tank()
arm.go_home()
input()

wheeledbase.goto(*puck3, theta=pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

print("robot arrivé sur puck3")

input()

arm.take_puck_in_distributor()
arm.put_in_tank()
arm.go_home()
input()

wheeledbase.goto(*balance, theta=pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

print("robot arrivé sur Balance")

input()

arm.take_in_tank()
arm.put_in_balance()

input()
arm.take_in_tank()
arm.put_in_balance()

input()
arm.take_in_tank()
arm.put_in_balance()

input()
print("end")