from robots.goldenium_bornibus import *
from robots.buttons_manager import ButtonsManager

def start_match():
    init_robot()
    buttons = ButtonsManager()
    # couleur M ou O
    print("wait for team")
    ssd.set_message("set team")
    while(buttons.state!="team selected"):
        time.sleep(0.1)
    print(buttons.team)
    wheeledbase.set_velocities(0, 0)
    couleur = buttons.team

    points = map_loading(couleur)
    ssd.clear_messages()
    ssd.set_message("set pos")

    while(buttons.state!="position selected"):
        time.sleep(0.1)

    if couleur == "O" :
        wheeledbase.set_position(*points["IniO"], pi/2)
        print("robot placé : ", wheeledbase.get_position())

    if couleur == "M" :
        wheeledbase.set_position(*points["IniO"], (3*pi)/2)
        print("robot placé : ", wheeledbase.get_position())

    ssd.clear_messages()
    ssd.set_message("Ready")
    while (buttons.state != "running"):
        time.sleep(0.1)

    start(points)

