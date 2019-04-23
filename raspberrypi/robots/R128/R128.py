#!/usr/bin/python3
#-*- coding: utf-8 -*-

from robots.R128.actions.balanceAction import *
from robots.R128.actions.takePuckActions import *
from robots.R128.actions.PutRedZoneAction import *
from robots.R128.actions.movingAction import *
from common.actions.action import ThreadActionManager
from common.geogebra import Geogebra
from robots.R128.setup_128 import *
from robots.automaton import Automaton


class R128(Automaton):
    DISTRIB6_1 = 1
    DISTRIB6_2 = 2
    DISTRIB6_3 = 3
    DISTRIB3_1 = 4
    DISTRIB3_2 = 5


    def __init__(self):
        Automaton.__init__(self)
        # Save daughter_cards
        self.daughter_cards = dict( wheeledbase     = wheeledbase, 
                                    armFront        = armFront,
                                    armBack         = armBack,
                                    display         = disp,
                                    sensor_manager  = sens_manager)

        # Save annexes inf
        self.side           = Automaton.UNDEFINED
        self.geogebra       = geo
        self.log            = log

        # action List
        self.action_list    = []

        #electron control
        self.electron       = electron
        # Wheeledbase
        self.wheeledbase    = self.daughter_cards['wheeledbase']
        # Display screen
        self.display        = self.daughter_cards['display']

        # Action thread manager
        self.tam = ThreadActionManager()

        init_robot()
        
    def stop_match():
        import time
        time.sleep(100)
        wheeledbase.stop()
        armFront.stop_pump()
        armBack.stop_pump()
        armF.stop()
        armB.stop()
        manager.disconnect()


    def set_side(self, side):
        self.side = side

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

    def set_position(self, point, side):
        if side == self.YELLOW:
            self.wheeledbase.set_position(785, 312, 0)
        else:
            self.wheeledbase.set_position(785, 3000-312, -pi)

    def run(self):
        self.log("MAIN : ", "RUN...")
        self.log.reset_time()
        Thread(target=self.stop_match).start()
        self.display.start()

        self.log("MAIN : ", "Launch Electron")
        self.electron.start()
        self.display.addPoints(40)

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
    auto = R128()
    auto.set_side(R128.PURPLE)
    init_robot()
    auto.set_position()
    print("ready")
    input()
    auto.run()
    pass