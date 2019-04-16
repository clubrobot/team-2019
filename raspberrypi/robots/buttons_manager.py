from robots.setup_bornibus import *
from gpiozero import Button

# GPIO: BTN_1=13 marche pas, BTN_2=12, BTN_3=6, BTN_4=5, TIR=26


class ButtonsManager:
    def set_team_orange(self):
        self.color = "O"
        ssd.clear_messages()
        ssd.set_message("team : o")
        self.green_switch.when_released = self.odometry

    def set_team_purple(self):
        self.color = "M"
        ssd.clear_messages()
        ssd.set_message("team : m")
        self.green_switch.when_released = self.odometry

    def odometry(self):
        self.orange_switch.close()
        self.blue_switch.close()

        self.points = self.map_load_funct(self.color)
        self.green_switch.when_released = self.tirret
        ssd.clear_messages()
        ssd.set_message("set pos")

    def ready(self):
        self.red_switch.close()
        self.green_switch.close()
        ssd.clear_messages()
        ssd.set_message("ready")
        self.tirette_switch.when_released = self.run_match

    def run_match(self):
        self.tirette_switch.close()
        if self.red_switch.is_active():
            print("red")
        if self.green_switch.is_active():
            print("green")
        if self.blue_switch.is_active():
            print("blue")
        if self.orange_switch.is_active():
            print("orange")
        if self.tirette_switch.is_active():
            print("tirette")
        self.start_funct(self.points, self.color)

    def tirret(self):
        self.pos_funct(self.points, self.color)
        ssd.clear_messages()
        ssd.set_message("tirette")
        self.tirette_switch.when_pressed = self.ready

    def begin(self):
        self.blue_switch.when_pressed = self.set_team_purple
        self.orange_switch.when_pressed = self.set_team_orange
        ssd.clear_messages()
        ssd.set_message("set team")

    def __init__(self, map_load_funct, pos_funct, start_funct):
        self.state = None
        self.red = 13
        self.green = 6
        self.blue = 12
        self.orange = 5
        self.tirette = 26

        self.map_load_funct = map_load_funct
        self.start_funct = start_funct
        self.pos_funct = pos_funct
        self.points = None
        self.color = None

        self.red_switch = Button(self.red, pull_up=True)
        self.green_switch = Button(self.green, pull_up=True)
        self.blue_switch = Button(self.blue, pull_up=True)
        self.orange_switch = Button(self.orange, pull_up=True)
        self.tirette_switch = Button(self.tirette, pull_up=True)
