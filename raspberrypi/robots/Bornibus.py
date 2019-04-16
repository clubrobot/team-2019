from robots.goldenium_bornibus import *
from robots.buttons_manager import ButtonsManager


def start_preparation():
    init_robot()
    ButtonsManager(map_loading, def_pos, start_robot).begin()

def start_robot(points, couleur):
    try:
        start(points, couleur)
    except RuntimeError:
        print("error")
    finally:
        disp.stop()
