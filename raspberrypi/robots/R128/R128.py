#!/usr/bin/python3
#-*- coding: utf-8 -*-

from robots.R128.R128Actions import TakeSyncDistrib, PutBalance, PutRed
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

        # Wheeledbase
        self.wheeledbase = self.daughter_cards['wheeledbase']

    def approval(self):
        self.log("APPROVAL","wait for team")
        ssd.set_message("set team")
        while(buttons.state!="team selected"):
            time.sleep(0.1)

        self.log("APPROVAL","Selected team is : {}".format(buttons.team))

        if(buttons.team == 'O')
            self.side = R128.YELLOW
        else:
            self.side = R128.PURPLE

        ssd.clear_messages()

        while(buttons.state!="position selected"):
            time.sleep(0.1)

         if self.side == R128.YELLOW:
            self.set_side(R128.YELLOW)
            self.wheeledbase.set_position(755, 322, pi)
        else:
            self.set_side(R128.PURPLE)
            self.wheeledbase.set_position(755, 3000-322, -pi)

        self.log("APPROVAL","robot placé : {}", .format(self.wheeledbase.get_position()))
        ssd.set_message("ready")

        self.daughter_cards['armFront'].go_home()
        self.daughter_cards['armBack'].go_home()
        while not (self.daughter_cards['armFront'].is_arrived() and self.daughter_cards['armBack'].is_arrived):
                time.sleep(0.1)

        self.log("APPROVAL","Ready For the match")
        ssd.clear_messages()
        
        while(buttons.state!="running"):
            time.sleep(0.1)

    def set_side(self, side):
        self.side = side
        # Apply cube obstacle
        self.log("SIDE CONFIG : ", "Set Side : {}".format(self.side))

        self.TakeSync1  = TakeSyncDistrib(self.geogebra, self.daughter_cards, self.side, R128.DISTRIB6_1, self.log)
        self.TakeSync2  = TakeSyncDistrib(self.geogebra, self.daughter_cards, self.side, R128.DISTRIB6_2, self.log)
        self.TakeSync3  = TakeSyncDistrib(self.geogebra, self.daughter_cards, self.side, R128.DISTRIB6_3, self.log)

        self.TakeSync1Act = self.TakeSync1.getAction()[0]
        self.TakeSync2Act = self.TakeSync2.getAction()[0]
        self.TakeSync3Act = self.TakeSync3.getAction()[0]

        self.Balance    = PutBalance(self.geogebra, self.daughter_cards, self.side, self.log)
        self.BalanceAct = self.Balance.getAction()[0]

        self.Red    = PutRed(self.geogebra, self.daughter_cards, self.side, self.log)
        self.RedAct = self.Red.getAction()[0]

        self.action_list = 
        [
            self.TakeSync1Act,
            self.TakeSync2Act,
            self.TakeSync3Act,
            self.BalanceAct,
            self.RedAct
        ]

    def run(self):
        self.log("MAIN : ", "RUN...")
        self.log.reset_time()
        
        for act in self.action_list:
                self.log("MAIN : ", "Let's go to the next action : {}".format(act.name))
                act.prepare()

                self.wheeledbase.goto(*act.actionPoint.point, theta=act.actionPoint.theta)

                while not act.prepareEnd():
                    time.sleep(0.1)
                
                self.log("MAIN ; ", "Arrived on action point ! Go execute it =)")

                act()
                act.done.set()
                act.complete()

                while not act.completeEnd():
                    time.sleep(0.1)


if __name__ == '__main__':
    from robots.R128.setup_128 import *
    init_robot()
    log("MAIN : ", "DEBUT CHARGEMENT ROADMAP")

    geo = Geogebra('128.ggb')

    auto = R128(R128.PURPLE,geo, wheeledbase, armFront,  armBack, log)
    auto.set_side(R128.PURPLE)
    auto.run()
