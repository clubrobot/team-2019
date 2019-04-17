def start_preparation():
    from common.components import Manager
    manager = Manager()
    manager.connect()
    import robots.goldenium_bornibus as Bornibus
    from robots.buttons_manager import ButtonsManager
    import os
    os.chdir("/home/pi/git/clubrobot/team-2019")
    Bornibus.init_robot()
    ButtonsManager(Bornibus.map_loading, Bornibus.def_pos, start_robot).begin()


def start_robot(points, couleur):
    import robots.goldenium_bornibus as Bornibus
    try:
        Bornibus.start(points, couleur)
    except RuntimeError as e:
        print("error : ", e)
    finally:
        Bornibus.disp.stop()
