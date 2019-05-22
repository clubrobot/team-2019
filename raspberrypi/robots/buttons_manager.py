from robots.setup_display import *
from robots.automaton import Automaton
from common.components import LightButtonProxy, SwitchProxy
from threading import Semaphore

class ButtonsManager:
    RED_PIN = 18  # 1
    RED_LIGHT = 23
    BLUE_PIN = 12  # 2
    BLUE_LIGHT = 4
    GREEN_PIN = 6  # 3
    GREEN_LIGHT = 21
    ORANGE_PIN = 5  # 4
    ORANGE_LIGHT = 16
    TIRETTE_PIN = 26
    URGENCY_PIN = 20

    def begin(self):
        print("debut Button Manager")
        ssd.clear_messages()
        ssd.set_message("set team")
        self.blue.set_function(Thread(target=self.set_team_purple, daemon=True).start)
        self.orange.set_function(Thread(target=self.set_team_orange, daemon=True).start)
        self.red.set_function(Thread(target=self.begin, daemon=True).start)
        self.p.acquire()

    def set_team_orange(self):
        print("team jaune")
        self.side = Automaton.YELLOW
        ssd.clear_messages()
        ssd.set_message("team : o")
        self.green.set_function(Thread(target=self.odometry_stage, daemon=True).start)

    def set_team_purple(self):
        print("team mauve")
        self.side = Automaton.PURPLE
        ssd.clear_messages()
        ssd.set_message("team : m")
        self.green.set_function(Thread(target=self.odometry_stage, daemon=True).start)

    def odometry_stage(self):
        print("validation team")
        self.auto.set_side(self.side)
        ssd.clear_messages()
        ssd.set_message("set pos")
        self.blue.set_function(None)
        self.orange.set_function(None)
        self.green.set_function(Thread(target=self.tirette_stage, daemon=True).start)

    def tirette_stage(self):
        print("validation odometry")
        self.auto.set_position()

        ssd.clear_messages()
        ssd.set_message("tirette")
        self.tirette.set_function(Thread(target=self.urgency_stage, daemon=True).start)
        self.green.set_function(None)

    def urgency_stage(self):
        print("validation tirette")
        ssd.clear_messages()
        ssd.set_message("urgency")
        self.urgency.set_function(Thread(target=self.positioning_stage, daemon=True).start)

    def positioning_stage(self):
        print("validation urgency")
        print("positionnement")
        self.auto.positioning()
        self.ready_stage()

    def ready_stage(self):
        print("ready")
        ssd.clear_messages()
        ssd.set_message("ready")
        self.tirette.set_function(Thread(target=self.run_match, daemon=True).start)
        self.tirette.set_active_high(False)


    def run_match(self):
        print("lancement match")
        self.tirette.close()
        self.urgency.close()
        self.red.close()
        self.blue.close()
        self.orange.close()
        self.green.close()

        Thread(target=self.auto.run(), daemon=True).start()
        self.p.release()

    def __init__(self, auto):
        self.auto = auto
        self.side = None

        self.red = LightButtonProxy(manager, self.RED_PIN, self.RED_LIGHT)
        self.green = LightButtonProxy(manager, self.GREEN_PIN, self.GREEN_LIGHT)
        self.blue = LightButtonProxy(manager, self.BLUE_PIN, self.BLUE_LIGHT)
        self.orange = LightButtonProxy(manager, self.ORANGE_PIN, self.ORANGE_LIGHT)
        self.tirette = SwitchProxy(manager, self.TIRETTE_PIN)
        self.urgency = SwitchProxy(manager, self.URGENCY_PIN, active_high=False)

        self.p = Semaphore(0)
