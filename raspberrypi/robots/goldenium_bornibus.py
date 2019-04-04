from math import pi
from robots.setup_bornibus import *
from common.geogebra import Geogebra
from robots.buttons_manager import ButtonsManager

# from common.components import *
# from daughter_cards.wheeledbase import *
# manager = Manager("127.0.0.1")
# manager.connect(10)
# wheeledbase = WheeledBase(manager)

MAX_TIME_FOR_GOLDENIUM = 2
buttons = ButtonsManager()
# couleur M ou O
print("wait for team")
while(buttons.state!="team selected"):
    time.sleep(0.1)
print(buttons.team)
wheeledbase.set_velocities(0, 0)

test_map = Geogebra("roadmap_bornibus.ggb")
print("Map chargée")

couleur = buttons.team

IniO = test_map.get("Ini"+couleur)
#Gold1 = test_map.get("Gold1"+couleur)
Gold2 = test_map.get("Gold2"+couleur)
Gold3 = test_map.get("Gold3"+couleur)
Gold4 = test_map.get("Gold4"+couleur)
Gold5 = test_map.get("Gold5"+couleur)
Gold6 = test_map.get("Gold6"+couleur)

Dep1 = test_map.get("Dep1"+couleur)
Dep2 = test_map.get("Dep2"+couleur)
Dep3 = test_map.get("Dep3"+couleur)

Pal1 = test_map.get("Pal1"+couleur)
Pal2 = test_map.get("Pal2"+couleur)
Pal3 = test_map.get("Pal3"+couleur)
Pal4 = test_map.get("Pal4"+couleur)
Pal5 = test_map.get("Pal5"+couleur)
Pal6 = test_map.get("Pal6"+couleur)

tmp = test_map.get("tmp"+couleur)
tmp2 = test_map.get("tmp2"+couleur)
tmp3 = test_map.get("tmp3"+couleur)

while(buttons.state!="position selected"):
    time.sleep(0.1)

if couleur=="O" :
    wheeledbase.set_position(*IniO , pi/2)
    print("robot placé : ", wheeledbase.get_position())

if couleur=="M" :
    wheeledbase.set_position(*IniO , (3*pi)/2)
    print("robot placé : ", wheeledbase.get_position())

wheeledbase.reset_parameters()
wheeledbase.max_linvel.set(700)
wheeledbase.max_angvel.set(10)
wheeledbase.lookahead.set(150.0)
wheeledbase.max_linacc.set(500.0)
wheeledbase.max_lindec.set(700.0)
#avant 300

pushers.up()
gripper.open()

wheeledbase.stop()

while(buttons.state!="running"):
    time.sleep(0.1)

print("Get position : ", wheeledbase.get_position())
print("Dep1 : ", Dep1)
disp.start()

wheeledbase.purepursuit([wheeledbase.get_position()[:2], Dep1, Dep2, Dep3, Gold2],direction = "forward", lookahead = 150, lookaheadbis = 3)
wheeledbase.wait()
print("Finis")

wheeledbase.lookaheadbis.set(150)

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
#attempt = 0
wheeledbase.right_wheel_maxPWM.set(0.2)
wheeledbase.left_wheel_maxPWM.set(0.2)

try:
    wheeledbase.goto(*Gold4, theta=pi)
    wheeledbase.wait()
except:
    gripper.close()

gripper.close()
time.sleep(0.5)
goldenium=1
disp.addPoints(20)

wheeledbase.right_wheel_maxPWM.set(1)
wheeledbase.left_wheel_maxPWM.set(1)

#while (goldenium!=1) and (attempt <=4) :
#    try:
#        wheeledbase.goto(*Gold4, theta=pi)
#       counter_start_time = time.monotonic()
#       while not wheeledbase.isarrived():
#           if time.monotonic() - counter_star_time < MAX_TIME_FOR_GOLDENIUM :
#               print(wheeledbase.get_position())
#               time.sleep(0.1)
#           else :
#                raise RuntimeError('time-out')
#       gripper.close()
#       time.sleep(0.5)
#       disp.addPoints(20)
#       goldenium=1
#   except:
#       print("Erreur")
#       time.sleep(0.5)
#       error_theta = wheeledbase.get_position()[2]
#       print("error_theta=", error_theta)
#       attempt+=1
#       if abs(-pi - error_theta) <= 0.1 : # Lol
#           print("rentré dans abs")
#           gripper.close()
#           time.sleep(5)
#           goldenium=1
#       if pi - error_theta > 0.1:
#           wheeledbase.goto(*Gold3, theta=pi)
#           while not wheeledbase.isarrived() :
#               print(wheeledbase.get_position())
#               time.sleep(0.1)
#           Gold4 = (Gold4[0],Gold4[1]+2)
#       else :
#           wheeledbase.goto(*Gold3, theta=pi)
#           while not wheeledbase.isarrived() :
#               print(wheeledbase.get_position())
#               time.sleep(0.1)
#           Gold4= (Gold4[0],Gold4[1]-2)

#wheeledbase.get_velocities_wanted(True)

wheeledbase.purepursuit([wheeledbase.get_position()[:2], Gold3, Gold5], direction= "backward", lookahead=150, lookaheadbis=100)
wheeledbase.wait()
wheeledbase.max_linacc.set(300.0)
wheeledbase.max_lindec.set(300.0)
wheeledbase.turnonthespot(0)
wheeledbase.wait()

try:
    wheeledbase.goto(*Gold6, theta=0)
    wheeledbase.wait()
except:
    gripper.open()
wheeledbase.reset_parameters()
wheeledbase.max_linacc.set(500.0)
wheeledbase.max_lindec.set(500.0)

time.sleep(0.5)
gripper.open()
disp.addPoints(24)

wheeledbase.purepursuit([wheeledbase.get_position()[:2], Gold5, Pal1, Pal2, tmp], direction="backward" )
wheeledbase.wait()

if couleur=="O":
    wheeledbase.purepursuit([wheeledbase.get_position()[:2], tmp2, Pal3, Pal4], direction="forward", finalangle=-0.7936277389526367)
    wheeledbase.wait()
    pushers.down()
    wheeledbase.turnonthespot(-5*pi/6)
    wheeledbase.wait()

if couleur=="M":
    wheeledbase.purepursuit([wheeledbase.get_position()[:2], tmp2, Pal3, Pal4], direction="forward", finalangle=0.7936277389526367)
    wheeledbase.wait()
    pushers.down()
    wheeledbase.turnonthespot(5*pi/6)
    wheeledbase.wait()



wheeledbase.purepursuit([wheeledbase.get_position()[:2], Pal5, Pal6], direction="forward")
wheeledbase.wait()

disp.addPoints(13)
pushers.up()

