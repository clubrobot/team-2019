from math import pi
import time
import json

from setup_128 import *


test_map = Geogebra("TakePuck.ggb")
print("Map chargée")

print("load points :")
puck1   = test_map.get("Puck1")
puck2   = test_map.get("Puck2")
puck3   = test_map.get("Puck3")
balance = test_map.get("Balance")
path    = test_map.getall("Path_*")
print(path)


wheeledbase.set_position(755, 2678, -pi)
print("robot placé : ", wheeledbase.get_position())

# #input()

wheeledbase.goto(1290, 2678, theta=pi/2)
print("robot placé : ", wheeledbase.get_position())
time.sleep(1)

wheeledbase.goto(1290, 2460, theta=pi/2)
time.sleep(1)

armController.take_puck_in_distributor()
armController.put_in_tank()
armController.go_home()

wheeledbase.goto(1300, 2260,theta=pi/2)
time.sleep(1)
armController.take_puck_in_distributor()
armController.put_in_tank()
armController.go_home()

wheeledbase.goto(1300, 2060,theta=pi/2)
time.sleep(1)
armController.take_puck_in_distributor()
armController.put_in_tank()
armController.go_home()

wheeledbase.goto(*balance,theta=pi/2)
time.sleep(1)
armController.take_in_tank()
armController.put_in_balance()
armController.take_in_tank()
armController.put_in_balance()
armController.take_in_tank()
armController.put_in_balance()

armController.go_home()

wheeledbase.goto(755, 2678, theta=-pi)
print("robot placé : ", wheeledbase.get_position())
input()