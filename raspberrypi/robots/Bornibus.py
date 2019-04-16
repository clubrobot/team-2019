from robots.goldenium_bornibus import *
from robots.buttons_manager import ButtonsManager

points = dict()

def start_preparation():
    init_robot()
    buttons = ButtonsManager(map_loading, def_pos, start_robot)
    # couleur M ou O
    while buttons.state is not "OK":
        time.sleep(0.1)


def start_robot(points, couleur):
    try:
        start(points, couleur)
    except RuntimeError:
        pass
    finally:
        disp.stop()


