#!/usr/bin/python3
#-*- coding: utf-8 -*-

from robots.R128.actions.balanceAction import *
from robots.R128.actions.takePuckActions import *
from robots.R128.actions.PutRedZoneAction import *
from robots.R128.actions.movingAction import *
from robots.R128.actions.acceleratorAction import *
from common.actions.action import ThreadActionManager
from common.geogebra import Geogebra
from robots.R128.setup_128 import *
from robots.automaton import Automaton
from robots.sensors_manager import *


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
                                    display         = disp)

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

    def set_side(self, side):
        self.side = side

        # Apply cube obstacle
        self.log("SIDE CONFIG : ", "Set Side : {}".format(self.side))

        # Specific Actions initialisation
        self.balanceAct6        = BalanceAfter6(self.geogebra, self.daughter_cards, self.side, self.log).getAction()

        self.balanceAct3        = BalanceAfter3(self.geogebra, self.daughter_cards, self.side, self.log).getAction()

        self.takeSyncPos1Act    = TakePuckSync(self.geogebra, self.daughter_cards, self.side, self.DISTRIB6_1, GreenPuck, RedPuck, self.log).getAction()

        self.takeSyncPos2Act    = TakePuckSync(self.geogebra, self.daughter_cards, self.side, self.DISTRIB6_2, BluePuck, RedPuck, self.log).getAction()

        self.takeSyncPos3Act    = TakePuckSync(self.geogebra, self.daughter_cards, self.side, self.DISTRIB6_3, GreenPuck, RedPuck, self.log).getAction()

        self.putRedZoneAct      = PutRedZone(self.geogebra, self.daughter_cards, self.side, self.log).getAction()

        self.takesingle         = TakePuckSingle(self.geogebra, self.daughter_cards, self.side, self.DISTRIB3_1, RedPuck, self.log).getAction()

        self.takemaintain       = TakePuckSyncMaintain(self.geogebra, self.daughter_cards, self.side, self.DISTRIB3_2, GreenPuck, BluePuck, self.log).getAction()

        self.movingAfterlittle  = MovingAfterLittle(self.geogebra, self.daughter_cards, self.side, self.log).getAction()

        #self.accel              = PutAccelerator(self.geogebra, self.daughter_cards, self.side, self.log).getAction()

        self.action_list = [
            self.takeSyncPos1Act,
            self.takeSyncPos2Act,
            self.takeSyncPos3Act,
            self.balanceAct6,
            self.takesingle,
            self.takemaintain,
            self.movingAfterlittle,
            self.balanceAct3,
            self.putRedZoneAct,
        ]

    def set_position(self):
        if self.side == R128.YELLOW:
            self.wheeledbase.set_position(755, 322, 0)
        else:
            self.wheeledbase.set_position(755, 3000-322, -pi)

    def stop_match(self):
        time.sleep(100)
        wheeledbase.stop()
        armFront.stop_pump()
        armBack.stop_pump()
        armF.stop()
        armB.stop()
        manager.disconnect()

    def run(self):
        self.log("MAIN : ", "RUN...")
        self.log.reset_time()
        Thread(target=self.stop_match).start()
        self.display.start()

        self.display.addPoints(5) #pose electron

        self.electron.start()
        self.display.addPoints(35)

        # s = SensorsManager(wheeledbase, sensorsFront, sensorsBack)
        # s.start()
        # starting thread action manager
        self.tam.start()
        
        for act in self.action_list:
            
            # add before action to the parralel action queue
            self.log("MAIN : ", "Launch Before Action")
            self.tam.putAction(act.getBefore())

            # moving to action point
            self.log("MAIN : ", "Moving ! ...")
            act.moving()

            # wait for parralels action end
            while not self.tam.end():
                time.sleep(0.1)
            
            # execute the current action
            self.log("MAIN ; ", "Arrived on action point ! Go execute {} =)".format(act.name))
            act()
            act.done.set()
            self.log("MAIN ; ", "Action End !")

            # add after action to the parrel action queue
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
