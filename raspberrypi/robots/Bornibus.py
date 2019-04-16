from robots.goldenium_bornibus import *
from robots.buttons_manager import ButtonsManager

points = dict()

def start_preparation():
    init_robot()
    buttons = ButtonsManager(map_loading, def_pos, start_robot)
    # couleur M ou O
    while buttons.state is not "OK":
        time.sleep(0.1)


def start_robot(points):
    try:
        start(points)
    except(...):
        pass
    finally:
        disp.stop()


def def_pos(color):
    if color == "M":
        wheeledbase.set_position(*points["IniM"], pi/2)
        print("robot placé : ", wheeledbase.get_position())

    if color == "O":
        wheeledbase.set_position(*points["IniO"], (3*pi)/2)
        print("robot placé : ", wheeledbase.get_position())


