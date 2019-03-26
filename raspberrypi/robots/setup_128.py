from robots.setup_wheeledbase import *
from robots.setup_serialtalks import *
from daughter_cards.RobotArm import *

arm = RobotArm(manager)
wheeledbase = WheeledBase(manager)


def init_robot():
    pass


if __name__ == "__main__":
    init_robot()
