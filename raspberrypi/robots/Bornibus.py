from robots.goldenium_bornibus import *
from robots.buttons_manager import ButtonsManager

def start_match():
    buttons = ButtonsManager()
    # couleur M ou O
    print("wait for team")
    ssd.set_message("set team")
    while(buttons.state!="team selected"):
        time.sleep(0.1)
    print(buttons.team)
    wheeledbase.set_velocities(0, 0)
    couleur = buttons.team

    points = map_loading()

    while(buttons.state!="position selected"):
        time.sleep(0.1)

    if couleur == "O" :
        wheeledbase.set_position(*points["IniO"], pi/2)
        print("robot placé : ", wheeledbase.get_position())

    if couleur == "M" :
        wheeledbase.set_position(*points["IniO"], (3*pi)/2)
        print("robot placé : ", wheeledbase.get_position())

    ssd.clear_messages()
    ssd.set_message("T=" + couleur + ", set pos")

    ssd.clear_messages()
    while (buttons.state != "running"):
        time.sleep(0.1)

    start(points)

