from math import pi
from common.geogebra import Geogebra
from robots.setup_wheeledbase import *

# from common.components import *
# from daughter_cards.wheeledbase import *
# manager = Manager("127.0.0.1")
# manager.connect(10)
# wheeledbase = WheeledBase(manager)

test_map = Geogebra("roadmap.ggb")
print("Map chargée")

wheeledbase.set_position(415, 2610, -pi/2)
path = test_map.getall("Path_*")
print(path)
wheeledbase.max_linvel.set(500)
wheeledbase.max_angvel.set(7)
wheeledbase.purepursuit(path)

while not wheeledbase.isarrived():
    time.sleep(0.1)

wheeledbase.goto(415, 2610, theta=-pi/2)
print("robot arrivé")

input()

try:
    wheeledbase.set_velocities(300, 0)
    while True:
        time.sleep(0.1)

except Exception as e:
    print("Patinage : ", e)

point = test_map.get("A")
print(point)

input()

wheeledbase.set_position(300, 300, -pi/4)
print("robot placé : ", wheeledbase.get_position())

wheeledbase.goto(*point, theta=0)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

print("robot arrivé")

input()

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






