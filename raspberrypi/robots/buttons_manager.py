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
        print("BUTTON MANAGER : Start")
        Thread(target=self.team_stage, daemon=True).start()
        self.red.set_function(Thread(target=self.team_stage, daemon=True).start)
        self.p.acquire()

    def team_stage(self):
        print("BUTTON MANAGER : Team Stage")
        ssd.set_message("set team")
        self.blue.set_function(Thread(target=self.set_team_purple, daemon=True).start)
        self.orange.set_function(Thread(target=self.set_team_yellow, daemon=True).start)

    def set_team_yellow(self):
        print("BUTTON MANAGER : Yellow Team")
        self.side = Automaton.YELLOW
        ssd.set_message("team : o")
        self.green.set_function(Thread(target=self.odometry_stage, daemon=True).start)

    def set_team_purple(self):
        print("BUTTON MANAGER : Purple Team")
        self.side = Automaton.PURPLE
        ssd.set_message("team : m")
        self.green.set_function(Thread(target=self.odometry_stage, daemon=True).start)

    def odometry_stage(self):
        print("BUTTON MANAGER : Team Validation")
        print("BUTTON MANAGER : Odometry Stage")
        self.auto.set_side(self.side)
        ssd.set_message("set pos")
        self.blue.set_function(None)
        self.orange.set_function(None)
        self.green.set_function(Thread(target=self.tirette_stage, daemon=True).start)

    def tirette_stage(self):
        print("BUTTON MANAGER : Odometry Validation")
        self.auto.set_position()

        print("BUTTON MANAGER : Tirret Stage")
        ssd.set_message("tirret")
        self.tirette.set_function(Thread(target=self.urgency_stage, daemon=True).start)
        self.green.set_function(None)

    def urgency_stage(self):
        print("BUTTON MANAGER : Tirret Validation")
        print("BUTTON MANAGER : Urgency Button Stage")
        ssd.set_message("urgency")
        self.urgency.set_function(Thread(target=self.positioning_stage, daemon=True).start)

    def positioning_stage(self):
        print("BUTTON MANAGER : Urgency Button Validation")
        print("BUTTON MANAGER : Robot Positionning")
        self.auto.positioning()
        self.ready_stage()

    def ready_stage(self):
        print("BUTTON MANAGER : Robot Ready !")
        if self.auto.master.is_active():
            ssd.set_message("ready.")
        else:
            ssd.set_message("ready")
        self.tirette.set_function(Thread(target=self.run_match, daemon=True).start)
        self.tirette.set_active_high(False)

    def run_match(self):
        print("BUTTON MANAGER : MATCH LAUNCHED !!!")
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
