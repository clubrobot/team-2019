from robots.get_robot_name import *


def start_preparation():
    from common.components import Manager
    manager = Manager()
    manager.connect()
    import os
    os.chdir("/home/pi/git/clubrobot/team-2019")
    from robots.buttons_manager import ButtonsManager

    if ROBOT_ID == BORNIBUS_ID:
        print("Launch match Bornibus")
        import robots.goldenium_bornibus as robot
        print("Init Bornibus")
        robot.init_robot()
        print("Button manager Bornibus")
        ButtonsManager(robot.map_loading, robot.def_pos, start_robot).begin()

    else:
        print("Launch match 128")
        import robots.R128.R128 as robot
        print("Init 128")
        R128 = robot.R128(robot.R128.PURPLE, robot.geo, robot.wheeledbase, robot.armFront, robot.armBack, robot.disp, robot.log)
        print("Button manager 128")
        ButtonsManager(R128.set_side, R128.def_pos, lambda x, y: R128.run()).begin()


def start_robot(points, couleur):
    import robots.goldenium_bornibus as Bornibus
    from threading import Thread
    Thread(target=stop_match).start()
    try:
        Bornibus.start(points, couleur)
    except RuntimeError as e:
        print("error : ", e)
    finally:
        Bornibus.disp.stop()


def stop_match():
    import robots.goldenium_bornibus as Bornibus
    import time
    time.sleep(100)
    Bornibus.wheeledbase.stop()
    Bornibus.gripper.open()
    Bornibus.pushers.up()
    Bornibus.arm.up()
    Bornibus.manager.disconnect()

