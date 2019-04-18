#!/usr/bin/python3
#-*- coding: utf-8 -*-

from robots.R128.balanceAction import *
from robots.R128.takePuckActions import *
from robots.R128.PutRedZoneAction import *
from robots.R128.movingAction import *
from common.actions.action import ThreadActionManager
from common.geogebra import Geogebra
from robots.R128.setup_128 import *


def stop_match():
    time.sleep(100)
    wheeledbase.stop()
    armFront.stop_pump()
    armBack.stop_pump()
    armF.stop()
    armB.stop()
    manager.disconnect()


class R128:
    YELLOW  = 0
    PURPLE  = 1

    DISTRIB6_1 = 1
    DISTRIB6_2 = 2
    DISTRIB6_3 = 3
    DISTRIB3_1 = 4
    DISTRIB3_2 = 5

    def __init__(self, side, geogebra, wheeledbase, arm1, arm2, display, sensor_manager, log):
        # Save daughter_cards
        self.daughter_cards = dict(wheeledbase = wheeledbase, armFront=arm1, armBack = arm2, display = display, ssd = display.display, sensor_manager = sens_manager)

        # Save annexes inf
        self.side       = side
        self.geogebra   = geogebra
        self.log        = log

        # action List
        self.action_list = []

        # Wheeledbase
        self.wheeledbase    = self.daughter_cards['wheeledbase']
        self.ssd            = self.daughter_cards['ssd']
        self.display        = self.daughter_cards['display']
        self.sensor_manager = self.daughter_cards['sensor_manager']
        # Action thread manager
        self.tam = ThreadActionManager()
        init_robot()

    def set_side(self, side):
        if side == "O":
            self.side = R128.YELLOW
        else:
            self.side = R128.PURPLE

        # Apply cube obstacle
        self.log("SIDE CONFIG : ", "Set Side : {}".format(self.side))

        # Specific Actions initialisation
        self.balanceAct         = BalanceAfter6(self.geogebra, self.daughter_cards, self.side, self.log).getAction()

        self.movingAfterStart   = MovingAfterStart(self.geogebra, self.daughter_cards, self.side, self.log).getAction()

        self.takeSyncPos1Act    = TakePuckSync(self.geogebra, self.daughter_cards, self.side, self.DISTRIB6_1, GreenPuck, RedPuck, self.log).getAction()

        self.takeSyncPos2Act    = TakePuckSync(self.geogebra, self.daughter_cards, self.side, self.DISTRIB6_2, BluePuck, RedPuck, self.log).getAction()

        self.takeSyncPos3Act    = TakePuckSync(self.geogebra, self.daughter_cards, self.side, self.DISTRIB6_3, GreenPuck, RedPuck, self.log).getAction()

        self.putRedZoneAct      = PutRedZone(self.geogebra, self.daughter_cards, self.side, self.log).getAction()

        self.moveToRed          = MovingToRed(self.geogebra, self.daughter_cards, self.side, self.log).getAction()

        self.action_list = [
            self.movingAfterStart,
            self.takeSyncPos1Act,
            self.takeSyncPos2Act,
            self.takeSyncPos3Act,
            self.moveToRed,
            self.putRedZoneAct,
            self.movingAfterStart,
            self.balanceAct,
        ]

    def def_pos(self, point, side):
        if side == "O":
            self.wheeledbase.set_position(785, 312, 0)
        else:
            self.wheeledbase.set_position(785, 3000-312, -pi)

    def run(self):
        self.log("MAIN : ", "RUN...")
        self.log.reset_time()
        #self.sensor_manager.start()
        Thread(target=stop_match).start()
        self.display.start()

        # starting thread action manager
        self.tam.start()
        
        for act in self.action_list:

            self.log("MAIN : ", "Launch Before Action")
            self.tam.putAction(act.getBefore())

            if act.actionPoint is not None:
                self.log("MAIN : ", "{}".format(act.actionPoint))
                self.daughter_cards['wheeledbase'].goto(*act.actionPoint.point, theta=act.actionPoint.theta)

            while not self.tam.end():
                time.sleep(0.1)

            self.log("MAIN ; ", "Arrived on action point ! Go execute {} =)".format(act.name))
            act()
            act.done.set()

            self.log("MAIN ; ", "Action End !")

            self.log("MAIN : ", "Launch After Action")
            self.tam.putAction(act.getAfter())
            
            self.log("MAIN : ", "Let's go to the next action !")
        #stop thread action manager
        self.tam.stop()
        self.display.stop()
        self.wheeledbase.stop()

if __name__ == '__main__':
    from robots.R128.setup_128 import *
    init_robot()
    log("MAIN : ", "DEBUT CHARGEMENT ROADMAP")

    geo = Geogebra('128.ggb')

    auto = R128(R128.YELLOW,geo, wheeledbase, armFront,  armBack, log)
    auto.set_side(R128.YELLOW)
    wheeledbase.set_position(755, 322, 0)
    #wheeledbase.set_position(755, 3000-322, -pi)
    auto.run()