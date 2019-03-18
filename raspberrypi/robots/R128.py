from common.logger                      import Logger
from robots.action                      import Action
from daughter_cards.arm.ArmController   import ArmController
import time

class R128:
    take    = "take"
    store   = "store"
    put     = "put"

    YELLOW  = 0
    PURPLE  = 1

    def __init__(self, side, geogebra, wheeledbase, arm_front, arm_back):
        # Save arduinos
        self.arduinos = dict(wheeledbase = wheeledbase,
                             arm_front   = arm_front,
                             arm_back    = arm_back,
                             )

        # Save annexes inf
        self.side     = side
        self.geogebra = geogebra
        self.logger   = Logger(Logger.SHOW)

        arm_front.set_logger(self.logger)
        arm_back.set_logger(self.logger)

        self.data = dict()

    def set_side(self, side):
        self.side = side

        self.action_list = []

        self.action_list = [
          
        ]

        if self.side == R128.YELLOW:
            self.arduinos["wheeledbase"].set_position(755, 2678, -pi) #change with correct pos
        else:
            self.arduinos["wheeledbase"].set_position(755, 2678, -pi)

    def run(self):
        self.logger.reset_time()
        #self.mover.reset()

        # act = self.heuristics.get_best()
        time.sleep(2)
        #print(act)
        # while act is not None:
        #     try:
        #         act.before_action()
        #         self.logger("MAIN : ", "Let's go to the next action : {}".format(act.name))
        #         self.mover.goto(*act.actionPoint)
        #         self.logger("MAIN ; ", "Arrived on action point ! Go execute it =)")
        #         act()
        #         act.done.set()
        #     except PositionUnreachable:
        #         act.temp_disable(5)

        #     act = self.heuristics.get_best()
        #     self.mover.reset()


if __name__ == '__main__':
    from robots.setup_128 import *

    print("DEBUT CHARGEMENT ROADMAP")
    geo = Geogebra('TakePuck.ggb')
    print("Fin Chargement")

    auto = R128(0, geo, wheeledbase, armController, None)
    auto.set_side(0)
    auto.run()