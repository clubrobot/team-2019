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

disp.start()

couleur="O"
# couleur M ou O

test_map = Geogebra("roadmap_bornibus.ggb")
print("Map chargée")

IniO = test_map.get("Ini"+couleur+"p")
#Gold1 = test_map.get("Gold1"+couleur)
Gold2 = test_map.get("Gold2"+couleur)
Gold3 = test_map.get("Gold3"+couleur)
Gold4 = test_map.get("Gold4"+couleur)
Gold5 = test_map.get("Gold5"+couleur)
Gold6 = test_map.get("Gold6"+couleur)

Dep1 = test_map.get("Dep1"+couleur)
Dep2 = test_map.get("Dep2"+couleur)
Dep3 = test_map.get("Dep3"+couleur)
Dep4 = test_map.get("Dep4"+couleur)
Dep5 = test_map.get("Dep5"+couleur)
Dep6 = test_map.get("Dep6"+couleur)
Dep7 = test_map.get("Dep7"+couleur)
Dep8 = test_map.get("Dep8"+couleur)

# Pour le gouter :
#Gold5 = test_map.get("Gold5"+"M")#couleur)
#Gold6 = test_map.get("Gold6"+"M")#couleur)

Path = test_map.get("Path1")

print("IniO loaded :", IniO)
print("Gold2 loaded :", Gold2)

if couleur=="O" :
    wheeledbase.set_position(*IniO , pi/2)
    print("robot placé : ", wheeledbase.get_position())

if couleur=="M" :
    wheeledbase.set_position(*IniO , (3*pi)/2)
    print("robot placé : ", wheeledbase.get_position())

wheeledbase.max_linvel.set(700)
wheeledbase.max_angvel.set(10)
wheeledbase.lookahead.set(150.0)
wheeledbase.max_linacc.set(500.0)
wheeledbase.max_lindec.set(700.0)
#avant 300

pushers.up()

wheeledbase.goto(*Dep1)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

print("debut purepursuit")
wheeledbase.purepursuit([wheeledbase.get_position()[:2], Dep2, Gold2],lookahead = 150, lookaheadbis = 1)
wheeledbase.wait()
print("Finis")
#wheeledbase.goto(*Dep2, theta=pi/2)
#while not wheeledbase.isarrived():
#    print(wheeledbase.get_position())
#    time.sleep(0.1)

"""
wheeledbase.goto(*Gold1, theta=-pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

time.sleep(0.5)
arm.deploy()


wheeledbase.goto(*Gold2, theta=-pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)
"""

if couleur=="O" :
    wheeledbase.turnonthespot(-pi/4)
    while not wheeledbase.isarrived():
        print(wheeledbase.get_position())
        time.sleep(0.1)
if couleur=="M" :
    wheeledbase.turnonthespot(-(3*pi)/4)
    while not wheeledbase.isarrived():
        print(wheeledbase.get_position())
        time.sleep(0.1)


time.sleep(0.5)
arm.deploy()

print("actual Gold2 :", wheeledbase.get_position())

wheeledbase.turnonthespot(-pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

arm.up()
disp.addPoints(20)
gripper.open()

wheeledbase.goto(*Gold3, theta=pi)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

goldenium = 0
attempt = 0

while (goldenium!=1) and (attempt <=4) :
    try:
        wheeledbase.goto(*Gold4, theta=pi)
        counter_start_time = time.monotonic()
        while not wheeledbase.isarrived():
            if time.monotonic() - counter_star_time < MAX_TIME_FOR_GOLDENIUM :
                print(wheeledbase.get_position())
                time.sleep(0.1)
            else :
                raise RuntimeError('time-out')
        gripper.close()
        time.sleep(0.5)
        disp.addPoints(20)
        goldenium=1
    except:
        print("Erreur")
        time.sleep(0.5)
        error_theta = wheeledbase.get_position()[2]
        print("error_theta=", error_theta)
        attempt+=1
        if abs(-pi - error_theta) <= 0.1 : # Lol
            print("rentré dans abs")
            gripper.close()
            time.sleep(5)
            goldenium=1
        if pi - error_theta > 0.1:
            wheeledbase.goto(*Gold3, theta=pi)
            while not wheeledbase.isarrived() :
                print(wheeledbase.get_position())
                time.sleep(0.1)
            Gold4 = (Gold4[0],Gold4[1]+2)
        else :
            wheeledbase.goto(*Gold3, theta=pi)
            while not wheeledbase.isarrived() :
                print(wheeledbase.get_position())
                time.sleep(0.1)
            Gold4= (Gold4[0],Gold4[1]-2)

#wheeledbase.get_velocities_wanted(True)

wheeledbase.goto(*Gold3, theta=pi)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

wheeledbase.goto(*Gold5, theta=0)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

time.sleep(0.5)

wheeledbase.goto(*Gold6, theta=0)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

time.sleep(0.5)
gripper.open()
disp.addPoints(24)
time.sleep(0.5)

"""
wheeledbase.goto(*Gold3, theta=pi)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

wheeledbase.goto(*Gold4, theta=pi)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

input()
"""
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

wheeledbase.goto(*Dep3, theta=3.5)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

#wheeledbase.max_linvel.set(300)
pushers.down()

wheeledbase.purepursuit([Dep3, Dep4, Dep5, Dep6])
#wheeledbase.goto(*Dep3, theta=3.5)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

#wheeledbase.goto(*Dep4, theta=3.5)
#while not wheeledbase.isarrived():
#    print(wheeledbase.get_position())
#    time.sleep(0.1)

#wheeledbase.goto(*Dep5, theta=3.5)
#while not wheeledbase.isarrived():
#    print(wheeledbase.get_position())
#    time.sleep(0.1)

#wheeledbase.goto(*Dep6, theta=3.5)
#while not wheeledbase.isarrived():
#    print(wheeledbase.get_position())
#    time.sleep(0.1)

#wheeledbase.purepursuit([Dep5], finalangle=3.5])
#wheeledbase.goto(*Dep5, theta=3.5)
#while not wheeledbase.isarrived():
#    print(wheeledbase.get_position())
#    time.sleep(0.1)

wheeledbase.purepursuit([Dep5, Dep7], finalangle=-pi/2, lookahead=50)
#wheeledbase.goto(*Dep7, theta=-pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

disp.addPoints(13)
pushers.up()

