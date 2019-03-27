from math import pi
from robots.setup_bornibus import *
from common.geogebra import Geogebra

# from common.components import *
# from daughter_cards.wheeledbase import *
# manager = Manager("127.0.0.1")
# manager.connect(10)
# wheeledbase = WheeledBase(manager)

wheeledbase.set_velocities(0, 0)

couleur="O"
# couleur M ou O

test_map = Geogebra("roadmap_bornibus.ggb")
print("Map chargée")

IniO = test_map.get("Ini"+couleur+"p")
Dep1 = test_map.get("Dep1"+couleur)
Dep2 = test_map.get("Dep2"+couleur)
Dep3 = test_map.get("Dep3"+couleur)
Dep4 = test_map.get("Dep4"+couleur)
Dep5 = test_map.get("Dep5"+couleur)
Dep6 = test_map.get("Dep6"+couleur)
Dep7 = test_map.get("Dep7"+couleur)
Dep8 = test_map.get("Dep8"+couleur)

if couleur=="O" :
    wheeledbase.set_position(*IniO , pi/2)
    print("robot placé : ", wheeledbase.get_position())

if couleur=="M" :
    wheeledbase.set_position(*IniO , (3*pi)/2)
    print("robot placé : ", wheeledbase.get_position())


wheeledbase.max_linvel.set(700)
wheeledbase.max_linacc.set(500.0)
wheeledbase.max_lindec.set(700.0)
#avant 300

pushers.up()

wheeledbase.goto(*Dep1, theta=pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

wheeledbase.goto(*Dep2, theta=pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

wheeledbase.goto(*Dep3, theta=3.5)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

wheeledbase.max_linvel.set(300)
pushers.down()

wheeledbase.goto(*Dep3, theta=3.5)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)


wheeledbase.goto(*Dep4, theta=3.5)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

wheeledbase.goto(*Dep5, theta=3.5)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

wheeledbase.goto(*Dep6, theta=3.5)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

wheeledbase.goto(*Dep5, theta=3.5)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

wheeledbase.goto(*Dep7, theta=-pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

wheeledbase.goto(*Dep8, theta=-pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)


"""
wheeledbase.goto(*Dep4, theta=pi)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

wheeledbase.goto(*Dep5, theta=pi)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

wheeledbase.goto(*Dep4, theta=pi)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

wheeledbase.goto(*Dep6, theta=-pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

wheeledbase.goto(*Dep5, theta=-pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

wheeledbase.goto(*Dep6, theta=-pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)
"""