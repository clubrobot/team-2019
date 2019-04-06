#!/usr/bin/python3
#-*- coding: utf-8 -*-

from robots.R128.R128Actions import TakeSyncDistrib, PutBalance, PutRed
from common.actions.action import ThreadActionManager
from common.geogebra import Geogebra

class R128:
    YELLOW  = 0
    PURPLE  = 1

    DISTRIB6_1 = 1
    DISTRIB6_2 = 2
    DISTRIB6_3 = 3
    def __init__(self, side, geogebra, wheeledbase, arm1, arm2, log):
        # Save daughter_cards
        self.daughter_cards = dict(wheeledbase = wheeledbase, armFront = arm1,armBack = arm2)

        # Save annexes inf
        self.side       = side
        self.geogebra   = geogebra
        self.log        = log

        # action List
        self.action_list = []

        # Wheeledbase
        self.wheeledbase = self.daughter_cards['wheeledbase']

        # Action thread manager
        self.tam = ThreadActionManager()

    def approval(self):
        self.log("APPROVAL","wait for team")
        ssd.set_message("set team")
        while(buttons.state!="team selected"):
            time.sleep(0.1)

        self.log("APPROVAL","Selected team is : {}".format(buttons.team))
        ssd.set_message("T= {}, set pos".format(buttons.team))
        ssd.clear_messages()

        while(buttons.state!="position selected"):
            time.sleep(0.1)

        if(buttons.team == 'O')
            self.set_side(R128.YELLOW)
            self.wheeledbase.set_position(755, 322, pi)
        else:
            self.set_side(R128.PURPLE)
            self.wheeledbase.set_position(755, 3000-322, -pi)

        self.log("APPROVAL","robot placé : {}", .format(self.wheeledbase.get_position()))

        self.daughter_cards['armFront'].go_home()
        self.daughter_cards['armBack'].go_home()
        while not (self.daughter_cards['armFront'].is_arrived() and self.daughter_cards['armBack'].is_arrived):
                time.sleep(0.1)

        ssd.set_message("ready")

        self.log("APPROVAL","Ready For the match")

        while(buttons.state!="running"):
            time.sleep(0.1)
        ssd.clear_messages()

    def set_side(self, side):
        self.side = side
        # Apply cube obstacle
        self.log("SIDE CONFIG : ", "Set Side : {}".format(self.side))

        # Specific Actions initialisation


    def run(self):
        self.log("MAIN : ", "RUN...")
        self.log.reset_time()

        # starting thread action manager
        self.tam.start()
        
        for act in self.action_list:

            self.log("MAIN : ", "Launch Before Action")
            self.tam.putAction(act.getBefore())

            self.daughter_cards['wheeledbase'].goto(*act.actionPoint.point, theta=act.actionPoint.theta)

            while not self.tam.end():
                time.sleep(0.1)

            self.log("MAIN ; ", "Arrived on action point ! Go execute {} =)".format(action.name))

            act()
            act.done.set()

            self.log("MAIN ; ", "Action End !")
            self.log("MAIN : ", "Launch After Action")

            self.atm.putAction(act.getAfter())
            
            self.log("MAIN : ", "Let's go to the next action !)

        # stop thread action manager
        self.tam.stop()

if __name__ == '__main__':
    from robots.R128.setup_128 import *
    init_robot()
    log("MAIN : ", "DEBUT CHARGEMENT ROADMAP")

    geo = Geogebra('128.ggb')

    auto = R128(R128.PURPLE,geo, wheeledbase, armFront,  armBack, log)
    auto.set_side(R128.PURPLE)
    auto.run()




 # self.TakeSync1  = TakeSyncDistrib(self.geogebra, self.daughter_cards, self.side, R128.DISTRIB6_1, self.log)
        # self.TakeSync2  = TakeSyncDistrib(self.geogebra, self.daughter_cards, self.side, R128.DISTRIB6_2, self.log)
        # self.TakeSync3  = TakeSyncDistrib(self.geogebra, self.daughter_cards, self.side, R128.DISTRIB6_3, self.log)

        # self.TakeSync1Act = self.TakeSync1.getAction()
        # self.TakeSync2Act = self.TakeSync2.getAction()
        # self.TakeSync3Act = self.TakeSync3.getAction()

        # self.Balance    = PutBalance(self.geogebra, self.daughter_cards, self.side, self.log)
        # self.BalanceAct = self.Balance.getAction()[0]

        # self.Red    = PutRed(self.geogebra, self.daughter_cards, self.side, self.log)
        # self.RedAct = self.Red.getAction()[0]

        # self.action_list = 
        # [
        #     self.TakeSync1Act,
        #     self.TakeSync2Act,
        #     self.TakeSync3Act,
        #     self.BalanceAct,
        #     self.RedAct
        # ]