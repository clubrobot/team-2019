from robots.setup_display import *
from gpiozero import Button

# GPIO: BTN_1=13 marche pas, BTN_2=12, BTN_3=6, BTN_4=5, TIR=26


class ButtonsManager:
    def set_team_orange(self):
        print("team jaune")
        self.color = "O"
        ssd.clear_messages()
        ssd.set_message("team : o")
        self.green_switch.when_released = self.odometry

    def set_team_purple(self):
        print("team mauve")
        self.color = "M"
        ssd.clear_messages()
        ssd.set_message("team : m")
        self.green_switch.when_released = self.odometry

    def odometry(self):
        print("validation team")
        self.orange_switch.close()
        self.blue_switch.close()

        self.points = self.map_load_funct(self.color)
        self.green_switch.when_released = self.tirret
        ssd.clear_messages()
        ssd.set_message("set pos")

    def ready(self):
        print("validation tirette")
        self.red_switch.close()
        self.green_switch.close()
        ssd.clear_messages()
        ssd.set_message("ready")
        self.tirette_switch.when_released = self.run_match

    def run_match(self):
        print("lancement match")
        self.start_funct(self.points, self.color)
        self.tirette_switch.close()

    def tirret(self):
        print("validation odometry")
        self.pos_funct(self.points, self.color)
        ssd.clear_messages()
        ssd.set_message("tirette")
        self.tirette_switch.when_pressed = self.ready

    def begin(self):
        print("debut Button Manager")
        self.blue_switch.when_pressed = self.set_team_purple
        self.orange_switch.when_pressed = self.set_team_orange
        ssd.clear_messages()
        ssd.set_message("set team")

    def __init__(self, map_load_funct, pos_funct, start_funct):
        self.state = None
        self.red = 18  # 1
        self.blue = 12  # 2
        self.green = 6  # 3
        self.orange = 5  # 4
        self.tirette = 26

        self.map_load_funct = map_load_funct
        self.start_funct = start_funct
        self.pos_funct = pos_funct
        self.points = None
        self.color = None

        self.red_switch = Button(self.red)
        self.green_switch = Button(self.green)
        self.blue_switch = Button(self.blue)
        self.orange_switch = Button(self.orange)
        self.tirette_switch = Button(self.tirette)
