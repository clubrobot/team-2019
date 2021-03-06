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
from robots.sensors_manager import *
import traceback
from robots.wheeledbase_manager import PositionUnreachable
COLOR = Automaton.YELLOW
COLOR = Automaton.PURPLE
PREPARATION = False

class Bornibus(Automaton):

    def __init__(self):
        Automaton.__init__(self)

        # Save daughter_cards
        self.daughter_cards = dict( wheeledbase     = wheeledbase,
                                    pushers         = pushers,
                                    arm             = arm,
                                    endstops        = endstops,
                                    gripper         = gripper,
                                    display         = disp,
                                    master          = beacons)

        # Save annexes inf
        self.side           = Automaton.UNDEFINED
        self.geogebra       = geo
        self.log            = log
        self.mover = Mover(self.daughter_cards, log, sensorsFront, sensorsBack)
        
        # action List
        self.action_list    = []

        # Wheeledbase
        self.wheeledbase    = self.daughter_cards['wheeledbase']

        # Display screen
        self.display        = self.daughter_cards['display']

        # Daughter cards 
        self.arm            = self.daughter_cards['arm']

        # Action thread manager
        self.tam = ThreadActionManager()

        self.points = dict()

        # Global sync client
        self.master = beacons

    def set_side(self, side):
        self.side = side
        self.log("SIDE CONFIG : ", "Set Side : {}".format(self.side))
        if self.side == self.YELLOW:
            color = "Y"
        else:
            color = "P"
        self.points["Ini"] = geo.get("Ini"+color)
        self.points["End"] = geo.get("End"+color)
        self.points["Ini2"] = geo.get("Ini2"+color)

        # Specific Actions initialisation
        self.detectorAct    = detector(self.geogebra, self.daughter_cards, self.mover, self.side, self.log).getAction()
        self.goldeniumAct   = Goldenium(self.geogebra, self.daughter_cards, self.mover, self.side, self.log).getAction()
        self.balanceGAct    = balance(self.geogebra, self.daughter_cards, self.mover, self.side, self.log).getAction()
        self.tabAtomsAct    = tabAtoms(self.geogebra, self.daughter_cards, self.mover, self.side, self.log).getAction()
        self.chaosAct       = chaos(self.geogebra, self.daughter_cards, self.mover, self.side, self.log).getAction()
        self.detectorAct_bis    = detector(self.geogebra, self.daughter_cards, self.mover, self.side, self.log, safe_mode=False).getAction()
        self.goldeniumAct_bis   = Goldenium(self.geogebra, self.daughter_cards, self.mover, self.side, self.log, safe_mode=False).getAction()
        self.balanceGAct_bis    = balance(self.geogebra, self.daughter_cards, self.mover, self.side, self.log, way="backward").getAction()

        self.action_list = [
            self.detectorAct,
            self.goldeniumAct,
            self.chaosAct,
            self.tabAtomsAct,
            self.balanceGAct,
            self.detectorAct_bis,
            self.goldeniumAct_bis,
            self.balanceGAct_bis,
        ]

        for sensor in sensorsBack+sensorsFront:
            sensor.set_side(self.side)

        wheeledbase.reset_parameters()

    def set_position(self):
        try:
            if self.side == Bornibus.PURPLE:
                wheeledbase.set_position(*self.points["Ini"], -pi/2)
                print(*self.points["Ini"])
            if self.side == Bornibus.YELLOW:
                wheeledbase.set_position(*self.points["Ini"], pi/2)
                print(*self.points["Ini"])
        except:
            pass    

    def positioning(self):
        init_robot()
        wheeledbase.goto(*self.points["Ini2"],
                         theta=(pi / 2 + pi / 6) if self.side == Automaton.YELLOW else -pi / 2 - pi / 6)

    def stop_match(self):
        import time
        time.sleep(100)
        self.tam.stop()
        wheeledbase.stop()
        disp.stop()
        gripper.open()
        pushers.up()
        arm.up()
        disp.love(duration=1000)
        manager.end_game()

    def run(self):
        self.log("MAIN : ", "RUN...")
        self.log.reset_time()
        Thread(target=self.stop_match).start()
        self.display.start()
        disp.points = 0
        disp.start()
        gold_open = False
        get_gold = False

        self.tam.start()

        for act in self.action_list:
            arm.up()
            #####  VERIFICATION DE L ACTION ###
            if gold_open and act.name=="detectorAction":
                self.log("MAIN : ", "Goldenium already open, pass action.")
                continue

            if (not get_gold) and act.name=="balanceGAction":
                self.log("MAIN : ", "Don't get gold skip balance")
                continue

            if (not gold_open) and act.name=="goldeniumAction":
                self.log("MAIN : ", "Pass gold action due to fail on previous act.")
                continue

            if get_gold and act.name=="goldeniumAction":
                self.log("MAIN : ", "Don't go to gold action because we already did it!")
                continue

            # add before action to the parralel action queue
            self.log("MAIN : ", "Launch Before Action")
            self.tam.putAction(act.getBefore())

            # moving to action point
            self.log("MAIN : ", "Moving ! ...")
            try:
                act.moving()
            except PositionUnreachable:
                self.log("MAIN : ", "Position Unreacheable !")
                self.log(str(traceback.format_exc()))
                continue
            # wait for parralels action end
            while not self.tam.end():
                time.sleep(0.1)

            # execute the current action
            self.log("MAIN ; ", "Arrived on action point ! Go execute {} =)".format(act.name))
            try:
                act()
                ### MISE A JOUR DE L ETAT
                if act.name=="detectorAction":
                    gold_open = True
                    self.log("MAIN : ", "Goldenium is open !")
                
                if act.name=="goldeniumAction":
                    get_gold = True
                    self.log("MAIN : ", "Bornibus get Goldenium !")
                
            except PositionUnreachable:
                self.log("MAIN : ", "Position Unreacheable !")
                self.log(str(traceback.format_exc()))
                continue
            act.done.set()
            self.log("MAIN ; ", "Action End !")

            # add after action to the parrel action queue
            self.log("MAIN : ", "Launch After Action")
            self.tam.putAction(act.getAfter())

            self.log("MAIN : ", "Let's go to the next action !")

        #stop thread action manager
        self.mover.goto(*self.points["End"],safe_mode=True, all_try=True, nb_try=0)
        self.tam.stop()
        self.display.stop()
        self.wheeledbase.stop()


if __name__ == '__main__':
    if PREPARATION:
        Bornibus().start_preparation()
    else:
        auto = Bornibus()
        auto.set_side(COLOR)
        init_robot()
        auto.set_position()
        print("ready")
        input()
        auto.run()
