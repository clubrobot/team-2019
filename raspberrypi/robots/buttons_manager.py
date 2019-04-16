from robots.setup_bornibus import *
from gpiozero import Button

# GPIO: BTN_1=13 marche pas, BTN_2=12, BTN_3=6, BTN_4=5, TIR=26


class ButtonsManager:
    def set_team_orange(self):
        self.color = "O"
        self.color_return = self.color_funct("O")
        ssd.clear_messages()
        ssd.set_message("team : o")
        self.green_switch.when_released = self.odometry

    def set_team_purple(self):
        self.color_return = self.color_funct("M")
        self.color = "M"
        ssd.clear_messages()
        ssd.set_message("team : m")
        self.green_switch.when_released = self.odometry

    def odometry(self):
        self.orange_switch.close()
        self.blue_switch.close()
        self.green_switch.when_released = self.tirret
        self.pos_funct(self.color)
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
        self.start_funct(self.color_return, self.color)

    def tirret(self):
        ssd.clear_messages()
        ssd.set_message("tirret")
        self.tirette_switch.when_pressed = self.ready

    def __init__(self, color_funct, pos_funct, start_funct):
        self.state = None
        self.red = 13
        self.green = 6
        self.blue = 12
        self.orange = 5
        self.tirette = 26

        self.color_funct = color_funct
        self.start_funct = start_funct
        self.pos_funct = pos_funct
        self.color_return = None
        self.team = None

        self.red_switch = Button(self.red, pull_up=True)
        self.green_switch = Button(self.green, pull_up=True)
        self.blue_switch = Button(self.blue, pull_up=True)
        self.orange_switch = Button(self.orange, pull_up=True)
        self.tirette_switch = Button(self.tirette, pull_up=True)

        self.blue_switch.when_pressed = self.set_team_purple
        self.orange_switch.when_pressed = self.set_team_orange

        ssd.clear_messages()
        ssd.set_message("set team")
