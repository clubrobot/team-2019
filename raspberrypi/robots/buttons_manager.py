from common.gpiodevices import Switch
from gpiozero import Button

#GPIO: BTN_1=13 marche pas, BTN_2=12, BTN_3=6, BTN_4=5, TIR=26

class ButtonsManager{
    def __init__(self):
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

        self.red_switch.when_pressed = odometry;
        self.blue_switch.when_pressed = set_team_purple
        self.orange_switch.when_pressed = set_team_orange

    def set_team_orange(self):
        print("orange team selected")
        self.green_switch.when_pressed = ready
    
    def set_team_purple(self):
        print("purple team selected")
        self.green_switch.when_pressed = ready
    
    def run_match(self):
        tirette.close()
        print("starting...")
    
    def ready(self):
        print("ready to start")
        self.orange_switch.close()
        self.purple_switch.close()
        self.red_switch.close()

        if(tirette_switch.is_pressed()):
            self.green_switch.close()
            self.tirette_switch.when_released = run_match
        else:
            print("tirette")
    
    def odometry(self):
        print("odomery")
}