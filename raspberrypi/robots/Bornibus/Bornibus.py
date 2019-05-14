#!/usr/bin/python3
#-*- coding: utf-8 -*-

from robots.Bornibus.setup_bornibus import *
from robots.automaton import Automaton
from robots.Bornibus.actions.detectorAction import *
from robots.Bornibus.actions.goldeniumAction import *
from robots.Bornibus.actions.balanceGAction import *
from robots.Bornibus.actions.tabAtomsAction import *
from robots.Bornibus.actions.chaosAction import *
from common.actions.action import ThreadActionManager
from common.geogebra import Geogebra
from robots.sensors_manager import *

#color = "YELLOW"
color = "PURPLE"

class Bornibus(Automaton):

    def __init__(self):
        Automaton.__init__(self)

        # Save daughter_cards
        self.daughter_cards = dict( wheeledbase     = wheeledbase,
                                    pushers         = pushers,
                                    arm             = arm,
                                    endstops        = endstops,
                                    gripper         = gripper,
                                    display         = disp )

        # Save annexes inf
        self.side           = Automaton.UNDEFINED
        self.geogebra       = geo
        self.log            = log

        # action List
        self.action_list    = []

        # Wheeledbase
        self.wheeledbase    = self.daughter_cards['wheeledbase']

        # Display screen
        self.display        = self.daughter_cards['display']

        # Action thread manager
        self.tam = ThreadActionManager()

        self.points = dict()

    def set_side(self, side):
        self.side = side
        self.log("SIDE CONFIG : ", "Set Side : {}".format(self.side))
        if self.side == self.YELLOW:
            color = "Y"
        else:
            color = "P"
        self.points["Ini"] = geo.get("Ini"+color)

        # Specific Actions initialisation
        self.detectorAct    = detector(self.geogebra, self.daughter_cards, self.side, self.log).getAction()
        self.goldeniumAct   = goldenium(self.geogebra, self.daughter_cards, self.side, self.log).getAction()
        self.balanceGAct    = balance(self.geogebra, self.daughter_cards, self.side, self.log).getAction()
        self.tabAtomsAct    = tabAtoms(self.geogebra, self.daughter_cards, self.side, self.log).getAction()
        self.chaosAct       = chaos(self.geogebra, self.daughter_cards, self.side, self.log).getAction()

        self.action_list = [
            self.detectorAct,
            self.goldeniumAct,
            self.chaosAct,
            self.tabAtomsAct,
            self.balanceGAct
        ]

    def set_position(self):
        if self.side == Bornibus.PURPLE:
            wheeledbase.set_position(*self.points["Ini"], -pi/2)
            print(*self.points["Ini"])
        if self.side == Bornibus.YELLOW:
            wheeledbase.set_position(*self.points["Ini"], pi/2)
            print(*self.points["Ini"])

    def stop_match(self):
        import time
        time.sleep(100)
        wheeledbase.stop()
        disp.stop()
        gripper.open()
        pushers.up()
        arm.up()
        manager.disconnect()

    def run(self):

        self.log("MAIN : ", "RUN...")
        self.log.reset_time()
        Thread(target=self.stop_match).start()
        self.display.start()
        begin = time.time()

        ##sens_manager.start()
        wheeledbase.reset_parameters()
        disp.points = 0
        disp.start()
        pushers.up()
        gripper.open()    

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

if __name__ == "__main__":
    auto = Bornibus()
    if color == "YELLOW" :
        auto.set_side(Bornibus.YELLOW)
    if color == "PURPLE" :
        auto.set_side(Bornibus.PURPLE)
    init_robot()
    auto.set_position()
    print("ready")
    input()
    auto.run()
    pass