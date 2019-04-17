#!/usr/bin/python3
#-*- coding: utf-8 -*-

from robots.R128.balanceAction import *
from robots.R128.takePuckActions import *
from robots.R128.PutRedZoneAction import *
from robots.R128.movingAction import *
from common.actions.action import ThreadActionManager
from common.geogebra import Geogebra
from robots.R128.setup_128 import *


class R128:
    YELLOW  = 0
    PURPLE  = 1

    DISTRIB6_1 = 1
    DISTRIB6_2 = 2
    DISTRIB6_3 = 3
    DISTRIB3_1 = 4
    DISTRIB3_2 = 5

    def __init__(self, side, geogebra, wheeledbase, arm1, arm2, display, log):
        # Save daughter_cards
        self.daughter_cards = dict(wheeledbase = wheeledbase, armFront = arm1, armBack = arm2, display = display, ssd = display.ssd)

        # Save annexes inf
        self.side       = side
        self.geogebra   = geogebra
        self.log        = log

        # action List
        self.action_list = []

        # Wheeledbase
        self.wheeledbase = self.daughter_cards['wheeledbase']
        self.ssd         = self.daughter_cards['ssd']
        self.display     = self.daughter_cards['display']
        # Action thread manager
        self.tam = ThreadActionManager()

    def approval(self):
        self.log("APPROVAL","wait for team")
        # self.ssd.set_message("set team")
        # while(buttons.state!="team selected"):
        #     time.sleep(0.1)

        # self.log("APPROVAL","Selected team is : {}".format(buttons.team))
        # self.ssd.set_message("T= {}, set pos".format(buttons.team))
        # self.ssd.clear_messages()

        # while(buttons.state!="position selected"):
        #     time.sleep(0.1)

        # if(buttons.team == 'O'):
        #     self.set_side(R128.YELLOW)
        #     self.wheeledbase.set_position(755, 322, pi)
        # else:
        #     self.set_side(R128.PURPLE)
        #     self.wheeledbase.set_position(755, 3000-322, -pi)

        # self.log("APPROVAL","robot placé : {}".format(self.wheeledbase.get_position()))

        # self.daughter_cards['armFront'].go_home()
        # self.daughter_cards['armBack'].go_home()
        # while not (self.daughter_cards['armFront'].is_arrived() and self.daughter_cards['armBack'].is_arrived):
        #         time.sleep(0.1)

        # ssd.set_message("ready")

        # self.log("APPROVAL","Ready For the match")

        # while(buttons.state!="running"):
        #     time.sleep(0.1)
        # ssd.clear_messages()
        #self.daughter_cards['display'].start()

    def set_side(self, side):
        self.side = side
        # Apply cube obstacle
        self.log("SIDE CONFIG : ", "Set Side : {}".format(self.side))

        # Specific Actions initialisation
        self.balanceAct         = BalanceAfter6(self.geogebra, self.daughter_cards, self.side, self.log).getAction()

        self.takeSyncPos1Act    = TakePuckSync(self.geogebra, self.daughter_cards, self.side, self.DISTRIB6_1, GreenPuck, RedPuck, self.log).getAction()

        self.takeSyncPos2Act    = TakePuckSync(self.geogebra, self.daughter_cards, self.side, self.DISTRIB6_2, BluePuck, RedPuck, self.log).getAction()

        self.takeSyncPos3Act    = TakePuckSync(self.geogebra, self.daughter_cards, self.side, self.DISTRIB6_3, GreenPuck, RedPuck, self.log).getAction()
    
        self.takeSyncPos4Act    = TakePuckSyncMaintain(self.geogebra, self.daughter_cards, self.side, self.DISTRIB3_2, self.log).getAction()

        self.putRedZoneAct      = PutRedZone(self.geogebra, self.daughter_cards, self.side, self.log).getAction()

        self.movingAct          = MovingToLittle(self.geogebra, self.daughter_cards, self.side, self.log).getAction()

        self.takeSingleAct      = TakePuckSingle(self.geogebra, self.daughter_cards, self.side, self.DISTRIB3_1, RedPuck, self.log).getAction()

        self.action_list = [
            self.takeSyncPos1Act,
            self.takeSyncPos2Act,
            self.takeSyncPos3Act,
            self.balanceAct,
            self.putRedZoneAct
        ]

    def def_pos(self, point, side):
        if side == "O":
            self.set_side(R128.YELLOW)
            self.wheeledbase.set_position(755, 322, 0)
        else:
            self.set_side(R128.PURPLE)
            self.wheeledbase.set_position(755, 3000-322, -pi)

    def run(self):
        self.log("MAIN : ", "RUN...")
        self.log.reset_time()

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

if __name__ == '__main__':
    from robots.R128.setup_128 import *
    init_robot()
    log("MAIN : ", "DEBUT CHARGEMENT ROADMAP")

    geo = Geogebra('128.ggb')

    auto = R128(R128.PURPLE,geo, wheeledbase, armFront,  armBack, log)
    auto.set_side(R128.PURPLE)
    wheeledbase.set_position(755, 3000-322, -pi)
    auto.run()