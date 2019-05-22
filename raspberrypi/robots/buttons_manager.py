from robots.setup_display import *
from robots.automaton import Automaton
from common.components import LightButtonProxy, SwitchProxy

class ButtonsManager:
    def begin(self):
        print("debut Button Manager")
        ssd.clear_messages()
        ssd.set_message("set team")
        self.blue.set_function(self.set_team_purple)
        self.orange.set_function(self.set_team_orange)
        self.red.set_function(self.begin)

    def set_team_orange(self):
        print("team jaune")
        self.side = Automaton.YELLOW
        ssd.clear_messages()
        ssd.set_message("team : o")
        self.green.set_function(self.odometry_stage)

    def set_team_purple(self):
        print("team mauve")
        self.side = Automaton.PURPLE
        ssd.clear_messages()
        ssd.set_message("team : m")
        self.green.set_function(self.odometry_stage)

    def odometry_stage(self):
        print("validation team")
        self.blue.close()
        self.orange.close()

        self.auto.set_side(self.side)
        ssd.clear_messages()
        ssd.set_message("set pos")
        self.green.set_function(self.tirette_stage)

    def tirette_stage(self):
        print("validation odometry")
        self.auto.set_position()
        self.green.close()

        ssd.clear_messages()
        ssd.set_message("tirette")
        self.tirette.set_function(self.urgency_stage)

    def urgency_stage(self):
        print("validation tirette")
        ssd.clear_messages()
        ssd.set_message("urgency")
        self.urgency.set_function(self.positioning_stage)

    def positioning_stage(self):
        print("positionnement")
        self.auto.positioning()
        self.ready_stage()

    def ready_stage(self):
        print("validation urgency")
        self.urgency.close()
        ssd.clear_messages()
        ssd.set_message("ready")
        self.tirette.set_function(self.run_match)

    def run_match(self):
        print("lancement match")
        self.tirette.close()
        self.red.close()
        self.auto.run()

    def __init__(self, auto):
        self.red_pin = 18  # 1
        self.red_light = 23
        self.blue_pin = 12  # 2
        self.blue_light = 4
        self.green_pin = 6  # 3
        self.green_light = 21
        self.orange_pin = 5  # 4
        self.orange_light = 16

        self.tirette_pin = 26
        self.urgency_pin = 20

        self.auto = auto
        self.side = None

        self.red = LightButtonProxy(manager, self.red_pin, self.red_light)
        self.green = LightButtonProxy(manager, self.green_pin, self.green_light)
        self.blue = LightButtonProxy(manager, self.blue_pin, self.blue_light)
        self.orange = LightButtonProxy(manager, self.orange_pin, self.orange_light)
        self.tirette = SwitchProxy(manager, self.tirette_pin)
        self.urgency = SwitchProxy(manager, self.urgency_pin, active_high=False)
