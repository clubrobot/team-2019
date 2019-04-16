from math import pi
from robots.setup_bornibus import *
from common.geogebra import Geogebra

MAX_TIME_FOR_GOLDENIUM = 2
COULEUR = "M"

def map_loading(couleur):
    import os
    for root, dirs, files in os.walk("."):
        for file in files:
            if file == "roadmap_bornibus.ggb":
                roadmap = os.path.join(root, file)
    test_map = Geogebra(roadmap)
    points = dict()
    points["IniO"] = test_map.get("Ini"+couleur)
    #Gold1 = test_map.get("Gold1"+couleur)
    points["Gold2"] = test_map.get("Gold2"+couleur)
    points["Gold3"] = test_map.get("Gold3"+couleur)
    points["Gold4"] = test_map.get("Gold4"+couleur)
    points["Gold5"] = test_map.get("Gold5"+couleur)
    points["Gold6"] = test_map.get("Gold6"+couleur)

    points["Dep1"] = test_map.get("Dep1"+couleur)
    points["Dep2"] = test_map.get("Dep2"+couleur)
    points["Dep3"] = test_map.get("Dep3"+couleur)

    points["Pal1"] = test_map.get("Pal1"+couleur)
    points["Pal2"] = test_map.get("Pal2"+couleur)
    points["Pal3"] = test_map.get("Pal3"+couleur)
    points["Pal4"] = test_map.get("Pal4"+couleur)
    points["Pal5"] = test_map.get("Pal5"+couleur)
    points["Pal6"] = test_map.get("Pal6"+couleur)

    points["tmp"] = test_map.get("tmp"+couleur)
    points["tmp2"] = test_map.get("tmp2"+couleur)
    points["tmp3"] = test_map.get("tmp3"+couleur)
    print("Map chargée")
    return points


def start(points):
    ssd.set_message("ready")
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

    print("Get position : ", wheeledbase.get_position())
    print("Dep1 : ", points["Dep1"])
    disp.start()

    wheeledbase.purepursuit([wheeledbase.get_position()[:2], points["Dep1"], points["Dep2"], points["Dep3"], points["Gold2"]],direction = "forward", lookahead = 150, lookaheadbis = 3)
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

    wheeledbase.goto(*points["Gold3"], theta=pi)
    while not wheeledbase.isarrived():
        print(wheeledbase.get_position())
        time.sleep(0.1)

    gold = False
    wheeledbase.right_wheel_maxPWM.set(0.2)
    wheeledbase.left_wheel_maxPWM.set(0.2)

    try:
        wheeledbase.goto(*points["Gold4"], theta=pi)
        wheeledbase.wait()
    except:
        pass

    if gripper.get_goldsensor_state():
        gold = True
        disp.addPoints(20)

    gripper.close()
    time.sleep(0.8)

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

    wheeledbase.purepursuit([wheeledbase.get_position()[:2], points["Gold3"], points["Gold5"]], direction= "backward", lookahead=150, lookaheadbis=100)
    wheeledbase.wait()
    wheeledbase.max_linacc.set(300.0)
    wheeledbase.max_lindec.set(300.0)
    wheeledbase.turnonthespot(0)
    wheeledbase.wait()

    try:
        wheeledbase.goto(*points["Gold6"], theta=0)
        wheeledbase.wait()
    except:
        gripper.open()

    wheeledbase.reset_parameters()
    wheeledbase.max_linacc.set(500.0)
    wheeledbase.max_lindec.set(500.0)

    time.sleep(0.5)
    gripper.open()
    if gold:
        disp.addPoints(24)

    wheeledbase.purepursuit([wheeledbase.get_position()[:2], points["Gold5"], points["Pal1"], points["Pal2"], points["tmp"]], direction="backward")
    wheeledbase.wait()

    if couleur=="O":
        wheeledbase.purepursuit([wheeledbase.get_position()[:2], points["tmp2"], points["Pal3"], points["Pal4"]], direction="forward", finalangle=-0.7936277389526367)
    if couleur == "M":
        wheeledbase.purepursuit([wheeledbase.get_position()[:2], points["tmp2"], points["Pal3"], points["Pal4"]], direction="forward", finalangle=0.7936277389526367)

    wheeledbase.wait()
    pushers.down()

    if couleur=="O":
        wheeledbase.turnonthespot(-5*pi/6)
    if couleur == "M":
        wheeledbase.turnonthespot(5 * pi / 6)
    wheeledbase.wait()

    wheeledbase.purepursuit([wheeledbase.get_position()[:2], points["Pal5"], points["Pal6"]], direction="forward")
    wheeledbase.wait()

    disp.addPoints(13)
    pushers.up()


if __name__ == "__main__":
    couleur = COULEUR
    init_robot()
    points = map_loading()
    if couleur == "O":
        wheeledbase.set_position(*points["IniO"], pi / 2)
        print("robot placé : ", wheeledbase.get_position())

    if couleur == "M":
        wheeledbase.set_position(*points["IniO"], (3 * pi) / 2)
        print("robot placé : ", wheeledbase.get_position())

    input()
    start(points)