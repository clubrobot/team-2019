#!/usr/bin/python3
#-*- coding: utf-8 -*-

from robots.R128.R128Actionss import TakeSyncDistrib, TakeSpecificArm, PutBalance, PutRedZone
from common.geogebra import Geogebra

class R128:
    YELLOW  = 0
    PURPLE  = 1

    DISTRIB6_1 = 1
    DISTRIB6_2 = 2
    DISTRIB6_3 = 3
    def __init__(self, side, geogebra, wheeledbase, arm1, arm2, log):
        # Save daughter_cards
        self.daughter_cards = dict(wheeledbase  = wheeledbase, 
                                    armFront    = arm1,
                                    armBack     = arm2
                                    )

        # Save annexes inf
        self.side       = side
        self.geogebra   = geogebra
        self.log        = log

        self.action_list = []

    def set_side(self, side):
        self.side = side
        # Apply cube obstacle

        self.TakeSync1  = TakeSyncDistrib(self.geogebra, self.daughter_cards, self.side, R128.DISTRIB6_1, self.log)
        self.TakeSync2  = TakeSyncDistrib(self.geogebra, self.daughter_cards, self.side, R128.DISTRIB6_2, self.log)
        self.TakeSync3  = TakeSyncDistrib(self.geogebra, self.daughter_cards, self.side, R128.DISTRIB6_3, self.log)

        self.TakeSync1Act = self.TakeSync1.getAction()[0]
        self.TakeSync2Act = self.TakeSync2.getAction()[0]
        self.TakeSync3Act = self.TakeSync3.getAction()[0]

        self.Balance    = PutBalance(self.geogebra, self.daughter_cards, self.side, self.log)
        self.BalanceAct = self.Balance.getAction()[0]

        self.action_list = [
            self.TakeSync1Act,
            self.TakeSync2Act,
            self.TakeSync3Act,
            self.BalanceAct,
        ]

        if self.side == R128.YELLOW:
            wheeledbase.set_position(755, 322, pi)
        else:
            wheeledbase.set_position(755, 3000-322, -pi)

    def run(self):
        self.log.reset_time()
        
        for act in self.action_list:
                self.log("MAIN : ", "Let's go to the next action : {}".format(act.name))
                self.daughter_cards['wheeledbase'].goto(*act.actionPoint, theta=act.theta)
                self.log("MAIN ; ", "Arrived on action point ! Go execute it =)")
                act()
                act.done.set()


if __name__ == '__main__':
    from robots.R128.setup_128 import *

    print("DEBUT CHARGEMENT ROADMAP")
    geo = Geogebra('128.ggb')

    auto = R128(R128.PURPLE,geo, wheeledbase, armFront,  armBack, log)
    auto.set_side(R128.PURPLE)
    auto.run()
