from robots.setup_bornibus import *
from gpiozero import Button

#GPIO: BTN_1=13 marche pas, BTN_2=12, BTN_3=6, BTN_4=5, TIR=26

class ButtonsManager:
    def odometry(self):
        self.state = "position selected"
        self.ready()
    def set_team_orange(self):
        self.team = "O"
        self.green_switch.when_pressed = self.odometry
        self.state = "team selected"
    def set_team_purple(self):
        self.team = "M"
        self.green_switch.when_pressed = self.odometry
        self.state = "team selected"
    def run_match(self):
        tirette.close()
        self.state = "running"
    def ready(self):
        self.orange_switch.close()
        self.blue_switch.close()
        self.red_switch.close()
        if(tirette_switch.is_pressed()):
            self.green_switch.close()
            self.tirette_switch.when_released = self.run_match
    def __init__(self):
        self.state = None
        self.red = 13
        self.green = 12
        self.blue = 6
        self.orange = 5
        self.tirette = 26

        self.red_switch = Button(self.red, pull_up=True)
        self.green_switch = Button(self.green, pull_up=True)
        self.blue_switch = Button(self.blue, pull_up=True)
        self.orange_switch = Button(self.orange, pull_up=True)
        self.tirette_switch = Button(self.tirette, pull_up=True)

        self.blue_switch.when_pressed = self.set_team_purple
        self.orange_switch.when_pressed = self.set_team_orange
