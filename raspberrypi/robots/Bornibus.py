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
    start(points)


def def_pos(color):
    if couleur == color:
        wheeledbase.set_position(*points["IniO"], pi/2)
        print("robot placé : ", wheeledbase.get_position())

    if couleur == color :
        wheeledbase.set_position(*points["IniM"], (3*pi)/2)
        print("robot placé : ", wheeledbase.get_position())


