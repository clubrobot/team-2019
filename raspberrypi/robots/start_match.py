from robots.get_robot_name import *


def start_preparation():
    from common.components import Manager
    manager = Manager()
    manager.connect()
    import os
    os.chdir("/home/pi/git/clubrobot/team-2019")

    from robots.buttons_manager import ButtonsManager
    if ROBOT_ID == BORNIBUS_ID:
        import robots.goldenium_bornibus as robot
        robot.init_robot()
        ButtonsManager(robot.map_loading, robot.def_pos, start_robot).begin()
    else:
        import robots.R128.R128 as robot
        R128 = robot.R128(robot.R128.PURPLE, robot.geo, robot.wheeledbase, robot.armFront, robot.armBack, robot.log)
        ButtonsManager(R128.set_side, R128.def_pos, lambda x, y: R128.run())


def start_robot(points, couleur):
    import robots.goldenium_bornibus as Bornibus
    try:
        Bornibus.start(points, couleur)
    except RuntimeError as e:
        print("error : ", e)
    finally:
        Bornibus.disp.stop()
