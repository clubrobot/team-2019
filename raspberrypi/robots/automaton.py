from robots.get_robot_name import *

class Automaton:
    YELLOW = 0
    PURPLE = 1
    UNDEFINED = -1

    def __init__(self):
        pass

    def set_side(self, side):
        pass

    def set_position(self):
        pass

    def run(self):
        pass

    @staticmethod
    def start_preparation():
        from common.components import Manager
        manager = Manager()
        manager.connect()
        import os
    
        if ROBOT_ID == BORNIBUS_ID:
            print("Launch match Bornibus")
            from robots.Bornibus.Bornibus import Bornibus
            auto = Bornibus()
            print("Button manager Bornibus")
        else:
            print("Launch match 128")
            from robots.R128.R128 import R128
            print("Init 128")
            auto = R128()
        print("Button manager")
        from robots.buttons_manager import ButtonsManager
        ButtonsManager(auto).begin()

