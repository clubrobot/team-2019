from robots.setup_display import *
from gpiozero import Button
from robots.automaton import Automaton

# GPIO: BTN_1=13 marche pas, BTN_2=12, BTN_3=6, BTN_4=5, TIR=26


class ButtonsManager:
    def begin(self):
        print("debut Button Manager")
        ssd.clear_messages()
        ssd.set_message("set team")
        self.blue.when_pressed = self.set_team_purple
        self.orange.when_pressed = self.set_team_orange

    def set_team_orange(self):
        print("team jaune")
        self.side = Automaton.YELLOW
        ssd.clear_messages()
        ssd.set_message("team : o")
        self.green.when_released = self.odometry_stage

    def set_team_purple(self):
        print("team mauve")
        self.side = Automaton.PURPLE
        ssd.clear_messages()
        ssd.set_message("team : m")
        self.green.when_released = self.odometry_stage

    def odometry_stage(self):
        print("validation team")
        self.auto.set_side(self.side)
        ssd.clear_messages()
        ssd.set_message("set pos")
        self.green.when_released = self.tirette_stage

    def tirette_stage(self):
        print("validation odometry")
        self.auto.set_position()
        ssd.clear_messages()
        ssd.set_message("tirette")
        self.tirette.when_activated = self.urgency_stage

    def urgency_stage(self):
        print("validation urgency")
        ssd.clear_messages()
        ssd.set_message("urgency")
        self.urgency.when_deactivated = self.positioning_stage

    def positioning_stage(self):
        print("positionnement")
        self.auto.positioning()
        self.ready_stage()

    def ready_stage(self):
        print("validation tirette")
        ssd.clear_messages()
        ssd.set_message("ready")
        self.tirette.when_released = self.run_match

    def run_match(self):
        print("lancement match")
        self.red.close()
        self.green.close()
        self.orange.close()
        self.blue.close()
        self.tirette.close()
        self.urgency.close()
        self.auto.run()
        self.tirette.close()

    def __init__(self, auto):
        self.red_pin = 18  # 1
        self.blue_pin = 12  # 2
        self.green_pin = 6  # 3
        self.orange_pin = 5  # 4
        self.tirette_pin = 26
        self.urgency_pin = 20

        self.auto = auto
        self.side = None

        self.red = Button(self.red_pin)
        self.green = Button(self.green_pin)
        self.blue = Button(self.blue_pin)
        self.orange = Button(self.orange_pin)
        self.tirette = Button(self.tirette_pin)
        self.urgency = Button(self.urgency_pin)
