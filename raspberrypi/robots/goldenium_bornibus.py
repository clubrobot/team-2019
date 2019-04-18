from math import pi
from robots.setup_bornibus import *
from common.geogebra import Geogebra
from robots.electron import *

MAX_TIME_FOR_GOLDENIUM = 2


def def_pos(points, couleur):
    if couleur == "M":
        wheeledbase.set_position(*points["Ini"], -pi/2)
    if couleur == "O":
        wheeledbase.set_position(*points["Ini"], pi/2)


def map_loading(couleur):
    import os
    roadmap = None
    for root, dirs, files in os.walk("."):
        for file in files:
            if file == "roadmap_bornibus.ggb":
                roadmap = os.path.join(root, file)
    test_map = Geogebra(roadmap)

    points = dict()
    points["Ini"] = test_map.get("Ini"+couleur)
    # Gold1 = test_map.get("Gold1"+couleur)
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
    return points


def start(points, couleur):
    wheeledbase.reset_parameters()
    wheeledbase.max_linvel.set(700)
    wheeledbase.max_angvel.set(10)
    wheeledbase.lookahead.set(150.0)
    wheeledbase.max_linacc.set(500.0)
    wheeledbase.max_lindec.set(700.0)

    print("max lin vel : ", wheeledbase.max_linvel.get())
    print("max ang vel : ", wheeledbase.max_angvel.get())

    print("disable back")
    sens_manager.disable_back()
    sens_manager.set_thresold(150)
    print("sens_manager start")
    #sens_manager.start()
    # time.sleep(2)
    print("max ang vel : ", wheeledbase.max_angvel.get())
    print("max lin vel : ", wheeledbase.max_linvel.get())

    disp.start()
    disp.points = 0

    pushers.up()
    gripper.open()

    # Vers l'accélérateur
    print("Vers l'accélérateur")
    wheeledbase.purepursuit([wheeledbase.get_position()[:2], points["Dep1"], points["Dep2"], points["Dep3"], points["Gold2"]],direction = "forward", lookahead = 150, lookaheadbis = 3)
    wheeledbase.wait()

    wheeledbase.lookaheadbis.set(150)

    # Prépare le bras
    print("Prépare le bras")
    sens_manager.disable_front()
    if couleur == "O":
        wheeledbase.turnonthespot(-pi/4)
        while not wheeledbase.isarrived():
            print(wheeledbase.get_position())
            time.sleep(0.1)
    if couleur == "M":
        wheeledbase.turnonthespot(-(3*pi)/4)
        while not wheeledbase.isarrived():
            print(wheeledbase.get_position())
            time.sleep(0.1)

    time.sleep(0.5)
    arm.deploy()

    # Pousse
    print("Pousse")
    wheeledbase.turnonthespot(-pi/2)
    while not wheeledbase.isarrived():
        print(wheeledbase.get_position())
        time.sleep(0.1)

    arm.up()
    disp.addPoints(20)
    gripper.open()

    # Vers préparation Goldenium
    print("Vers préparation Goldenium")
    sens_manager.enable_front()
    wheeledbase.goto(*points["Gold3"], theta=pi )
    while not wheeledbase.isarrived():
        print(wheeledbase.get_position())
        time.sleep(0.1)

    sens_manager.disable_front()

    # Vers Goldenium
    print("Vers Goldenium")
    gold = False
    wheeledbase.right_wheel_maxPWM.set(0.2)
    wheeledbase.left_wheel_maxPWM.set(0.2)

    try:
        wheeledbase.goto(*points["Gold4"], theta=pi)
        wheeledbase.wait()
    except:
        pass

    # Prise goldenium
    print("Prise Goldenium")
    gripper.close()
    time.sleep(1.5)

    if gripper.get_goldsensor_state():
        gold = True
        disp.addPoints(20)


    wheeledbase.right_wheel_maxPWM.set(1)
    wheeledbase.left_wheel_maxPWM.set(1)

    # Vers balance
    print("Vers Balance")
    sens_manager.enable_back()
    wheeledbase.purepursuit([wheeledbase.get_position()[:2], points["Gold3"], points["Gold5"]], direction= "backward", lookahead=150, lookaheadbis=100)
    wheeledbase.wait()
    wheeledbase.max_linacc.set(300.0)
    wheeledbase.max_lindec.set(300.0)
    wheeledbase.turnonthespot(0)
    wheeledbase.wait()

    # Dépose balance
    print("Dépose Balance")
    sens_manager.disable_back()

    try:
        wheeledbase.goto(*points["Gold6"], theta=0)
        wheeledbase.wait()
    except:
        gripper.open()

    wheeledbase.reset_parameters()
    wheeledbase.max_linacc.set(500.0)
    wheeledbase.max_lindec.set(500.0)

    # Depose
    print("Dépose")
    time.sleep(0.5)

    if gripper.get_goldsensor_state():
        disp.addPoints(24)
    gripper.open()

    time.sleep(0.5)


    # Vers palets

    print("Vers Palets")
    sens_manager.enable_back()
    if couleur == "O":
        wheeledbase.purepursuit([wheeledbase.get_position()[:2], points["Gold5"], points["Pal1"], points["Pal4"]], direction="backward", finalangle=-pi/4)
    if couleur == "M":
        wheeledbase.purepursuit([wheeledbase.get_position()[:2], points["Gold5"], points["Pal1"], points["Pal4"]], direction="backward", finalangle=pi/4)

    wheeledbase.wait()

    if couleur == "O":
        wheeledbase.turnonthespot(-5*pi/6)
    if couleur == "M":
        wheeledbase.turnonthespot(5 * pi / 6)
    wheeledbase.wait()


    # Vers zone
    print("Vers Zone")
    sens_manager.back_disable()
    sens_manager.front_enable()
    pushers.down()

    wheeledbase.purepursuit([wheeledbase.get_position()[:2], points["Pal5"], points["Pal6"]], direction="forward")
    wheeledbase.wait()

    disp.addPoints(13)

    # fin
    pushers.up()
    wheeledbase.stop()
    print("fin")


if __name__ == "__main__":
    couleur = "O"
    init_robot()
    points = map_loading(couleur)
    def_pos(points, couleur)
    print("ready")
    input()
    start(points, couleur)
    disp.stop()
