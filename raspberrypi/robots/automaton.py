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

    def positioning(self):
        pass

    def start_preparation(self):
        from robots.buttons_manager import ButtonsManager
        ButtonsManager(self).begin()

