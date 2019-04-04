from robots.setup_bornibus import *
from gpiozero import Button

#GPIO: BTN_1=13 marche pas, BTN_2=12, BTN_3=6, BTN_4=5, TIR=26

class ButtonsManager:
    def odometry(self):
        self.state = "position selected"
        self.ready()
    def set_team_orange(self):
        self.team = "O"
        ssd.set_message("orange")
        self.green_switch.when_pressed = odometry
        self.state = "team selected"
    def set_team_purple(self):
        self.team = "M"
        ssd.set_message("purple")
        self.green_switch.when_pressed = odometry
        self.state = "team selected"
    def run_match(self):
        tirette.close()
        self.state = "running"
    def ready(self):
        self.orange_switch.close()
        self.purple_switch.close()
        self.red_switch.close()
        if(tirette_switch.is_pressed()):
            self.green_switch.close()
            self.tirette_switch.when_released = run_match
            ssd.set_message("ready")
        else:
            ssd.set_message("tirette")
    def __init__(self):
        self.state = None
        self.red = 13
        self.green = 12
        self.blue = 5
        self.orange = 6
        self.tirette = 26

        self.red_switch = Button(self.red, pull_up=True)
        self.green_switch = Button(self.green, pull_up=True)
        self.blue_switch = Button(self.blue, pull_up=True)
        self.orange_switch = Button(self.orange, pull_up=True)
        self.tirette_switch = Button(self.tirette, pull_up=True)

        self.blue_switch.when_pressed = set_team_purple
        self.orange_switch.when_pressed = set_team_orange
