from math import pi
from robots.setup_bornibus import *
from common.geogebra import Geogebra

# from common.components import *
# from daughter_cards.wheeledbase import *
# manager = Manager("127.0.0.1")
# manager.connect(10)
# wheeledbase = WheeledBase(manager)

MAX_TIME_FOR_GOLDENIUM = 2

wheeledbase.set_velocities(0, 0)

Path = test_map.get("Path")

wheeledbase.set_position(*points[Ini] , pi/2)
print("robot placé : ", wheeledbase.get_position())

wheeledbase.max_linvel.set(300)

wheeledbase.goto(*path, theta=-pi/2)

while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

wheeledbase.goto(*points[Ini], theta=pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

input()

wheeledbase.goto(*points[Gold1], theta=-pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

arm.deploy()

wheeledbase.goto(*points[Gold2], theta=-pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

arm.up()
gripper.open()

wheeledbase.goto(*points[Gold3], theta=pi)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

wheeledbase.goto(*points[Gold4], theta=pi)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)



gripper.close()
time.sleep(1)

wheeledbase.goto(*points[Gold3], theta=pi)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

input()

""" 
wheeledbase.goto_delta(100, 0)
while not wheeledbase.isarrived():
    time.sleep(0.1)
print("robot arrivé")

input()

wheeledbase.max_linvel.set(200)
wheeledbase.goto_delta(-100, 0)
while not wheeledbase.isarrived():
    time.sleep(0.1)
print("robot arrivé")

input()

wheeledbase.max_angvel.set(3)
wheeledbase.turnonthespot(pi/2)
while not wheeledbase.isarrived():
    time.sleep(0.1)
print("robot arrivé")

input()

wheeledbase.max_angvel.set(10)
wheeledbase.turnonthespot(-pi/2)
while not wheeledbase.isarrived():
    time.sleep(0.1)
print("robot arrivé")

input()

path = test_map.getall("Path_*")
wheeledbase.max_linvel.set(700)
wheeledbase.max_angvel.set(7)
wheeledbase.purepursuit(path)

while not wheeledbase.isarrived():
    time.sleep(0.1)
print("robot arrivé")

input()

try:
    wheeledbase.set_velocities(300, 0)
    while True:
        time.sleep(0.1)


except Exception as e:
    print("Patinage : ", e)
"""

