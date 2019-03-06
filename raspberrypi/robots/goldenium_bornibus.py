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

couleur="M"
# couleur M ou O

test_map = Geogebra("roadmap_bornibus.ggb")
print("Map chargée")

IniO = test_map.get("ini"+couleur+"p")
Gold1 = test_map.get("Gold1"+couleur)
Gold2 = test_map.get("Gold2"+couleur)
Gold3 = test_map.get("Gold3"+couleur)
Gold4 = test_map.get("Gold4"+couleur)
Gold5 = test_map.get("Gold5"+couleur)
Gold6 = test_map.get("Gold6"+couleur)
# Pour le gouter :
#Gold5 = test_map.get("Gold5"+"M")#couleur)
#Gold6 = test_map.get("Gold6"+"M")#couleur)

Path = test_map.get("Path1")

print(IniO)
print(Gold1)
print(Gold2)

if couleur=="O" :
    wheeledbase.set_position(*IniO , pi/2)
    print("robot placé : ", wheeledbase.get_position())

if couleur=="M" :
    wheeledbase.set_position(*IniO , (3*pi)/2)
    print("robot placé : ", wheeledbase.get_position())

# Pour le gouter :
#wheeledbase.set_position(*IniO , -pi/2)

wheeledbase.max_linvel.set(700)
wheeledbase.max_linacc.set(500.0)
wheeledbase.max_lindec.set(700.0)
#avant 300

"""
wheeledbase.goto(*Path, theta=-pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

wheeledbase.goto(*IniO, theta=pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

input()
"""
wheeledbase.goto(*Gold1, theta=-pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

time.sleep(1)
print(wheeledbase.get_position())
arm.deploy()

wheeledbase.goto(*Gold2, theta=-pi/2)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

time.sleep(1)
print(wheeledbase.get_position())
arm.up()
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
        goldenium=1
        
    except:
        print("Erreur")
        time.sleep(0.5)
        error_theta = wheeledbase.get_position()[2]
        print("error_theta=", error_theta)
        wheeledbase.goto(*Gold3, theta=pi)
        while not wheeledbase.isarrived() :
            print(wheeledbase.get_position())
            time.sleep(0.1)
        if pi - error_theta > 0.1:
            Gold4 = (Gold4[0],Gold4[1]+2)
        #if abs(pi - error_theta) >= 0.1 :
        #    Gold4 = (Gold4[0]+2,Gold4[1])
        else :
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

time.sleep(1)

wheeledbase.goto(*Gold6, theta=0)
while not wheeledbase.isarrived():
    print(wheeledbase.get_position())
    time.sleep(0.1)

time.sleep(0.5)
gripper.open()
time.sleep(0.5)

if couleur=="O" :
    wheeledbase.goto(*IniO, theta=pi/2)
    while not wheeledbase.isarrived():
        print(wheeledbase.get_position())
        time.sleep(0.1)

if couleur=="M" :
    wheeledbase.goto(*IniO, theta=-pi/2)
    while not wheeledbase.isarrived():
        print(wheeledbase.get_position())
        time.sleep(0.1)



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

