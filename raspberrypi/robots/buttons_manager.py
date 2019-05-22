from robots.setup_display import *
from robots.automaton import Automaton
from common.components import LightButtonProxy, SwitchProxy


class ButtonsManager:
    RED_PIN = 18  # 1
    RED_LIGHT = 23
    BLUE_PIN = 12  # 2
    BLUE_LIGHT = 4
    GREEN_PIN = 6  # 3
    GREEN_LIGHT = 21
    ORANGE_PIN = 5  # 4
    ORANGE_LIGHT = 16

    def begin(self):
        print("debut Button Manager")
        ssd.clear_messages()
        ssd.set_message("set team")
        self.blue.set_function(Thread(target=self.set_team_purple).start)
        self.orange.set_function(Thread(target=self.set_team_orange).start)
        self.red.set_function(Thread(target=self.begin).start)

    def set_team_orange(self):
        print("team jaune")
        self.side = Automaton.YELLOW
        ssd.clear_messages()
        ssd.set_message("team : o")
        self.green.set_function(Thread(target=self.odometry_stage).start)

    def set_team_purple(self):
        print("team mauve")
        self.side = Automaton.PURPLE
        ssd.clear_messages()
        ssd.set_message("team : m")
        self.green.set_function(Thread(target=self.odometry_stage).start)

    def odometry_stage(self):
        print("validation team")
        self.blue.close()
        self.orange.close()

        self.auto.set_side(self.side)
        ssd.clear_messages()
        ssd.set_message("set pos")
        self.green.set_function(Thread(target=self.tirette_stage).start)

    def tirette_stage(self):
        print("validation odometry")
        self.auto.set_position()
        self.green.close()

        ssd.clear_messages()
        ssd.set_message("tirette")
        self.tirette.set_function(Thread(target=self.urgency_stage).start)

    def urgency_stage(self):
        print("validation tirette")
        ssd.clear_messages()
        ssd.set_message("urgency")
        self.urgency.set_function(Thread(target=self.positioning_stage).start)

    def positioning_stage(self):
        print("positionnement")
        self.auto.positioning()
        self.ready_stage()

    def ready_stage(self):
        print("validation urgency")
        self.urgency.close()
        ssd.clear_messages()
        ssd.set_message("ready")
        self.tirette.set_function(Thread(target=self.run_match).start)

    def run_match(self):
        print("lancement match")
        self.tirette.close()
        self.red.close()
        self.auto.run()

    def __init__(self, auto):
        self.tirette_pin = 26
        self.urgency_pin = 20

        self.auto = auto
        self.side = None

        self.red = LightButtonProxy(manager, self.RED_PIN, self.RED_LIGHT)
        self.green = LightButtonProxy(manager, self.GREEN_PIN, self.GREEN_LIGHT)
        self.blue = LightButtonProxy(manager, self.BLUE_PIN, self.BLUE_LIGHT)
        self.orange = LightButtonProxy(manager, self.ORANGE_PIN, self.ORANGE_LIGHT)
        self.tirette = SwitchProxy(manager, self.tirette_pin)
        self.urgency = SwitchProxy(manager, self.urgency_pin, active_high=False)
