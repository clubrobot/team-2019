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

color = "YELLOW"
#color = "PURPLE"

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
        color = "P" if side == self.PURPLE else "Y"

        self.points["Ini"] = geo.get("Ini"+color)

        self.points["Gold2"] = geo.get("Gold2"+color)
        self.points["Gold3"] = geo.get("Gold3"+color)
        self.points["Gold4"] = geo.get("Gold4"+color)
        self.points["Gold5"] = geo.get("Gold5"+color)
        self.points["Gold6"] = geo.get("Gold6"+color)

        self.points["Pal1"] = geo.get("Pal1"+color)
        self.points["Pal2"] = geo.get("Pal2"+color)
        self.points["Pal3"] = geo.get("Pal3"+color)
        self.points["Pal4"] = geo.get("Pal4"+color)
        self.points["Pal5"] = geo.get("Pal5"+color)
        self.points["Pal6"] = geo.get("Pal6"+color)

        # Specific Actions initialisation
        self.detectorAct    = detector(self.geogebra, self.daughter_cards, self.side, self.log).getAction()
        self.goldeniumAct   = goldenium(self.geogebra, self.daughter_cards, self.side, self.log).getAction()

        self.action_list = [
            self.detectorAct,
            self.goldeniumAct
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

        sens_manager.start()
        wheeledbase.reset_parameters()
        disp.points = 0
        disp.start()
        pushers.up()
        gripper.open()    

        print("INTO RUN ...")
        time.sleep(3)
        input()

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

        """
        

        # Tempo
        temp = 65 - (time.time() - begin)
        if temp > 0:
            time.sleep(temp)

        wheeledbase.right_wheel_maxPWM.set(1)
        wheeledbase.left_wheel_maxPWM.set(1)

        # Vers balance
        print("Vers Balance")
        # sens_manager.enable_back()
        wheeledbase.purepursuit([wheeledbase.get_position()[:2], self.points["Gold3"], self.points["Gold5"]],
                                direction="backward", finalangle=0, lookahead=150, lookaheadbis=10)
        wheeledbase.wait()
        wheeledbase.max_linacc.set(300.0)
        wheeledbase.max_lindec.set(300.0)
        wheeledbase.max_linvel.set(400)
        wheeledbase.turnonthespot(0)
        wheeledbase.wait()

        # Dépose balance
        print("Dépose Balance")
        # sens_manager.disable_back()
        wheeledbase.goto(*self.points["Gold6"], theta=0)

        wheeledbase.right_wheel_maxPWM.set(0.5)
        wheeledbase.left_wheel_maxPWM.set(0.5)

        wheeledbase.set_velocities(200, 0)
        time.sleep(2)
        wheeledbase.set_velocities(0, 0)

        #try:
        #    wheeledbase.goto(*self.points["Gold6"], theta=0, lookaheadbis=150)
        #    wheeledbase.wait()
        #except:
        #    gripper.open()
        #    print("error")

        wheeledbase.reset_parameters()
        wheeledbase.max_linacc.set(500.0)
        wheeledbase.max_lindec.set(500.0)

        # Depose
        print("Dépose")
        time.sleep(0.5)

        if endstops.get_ES3():
            disp.addPoints(24)
        gripper.open()

        time.sleep(0.5)

        # Premier palet
        if self.side == Bornibus.YELLOW:
            wheeledbase.purepursuit([wheeledbase.get_position()[:2], self.points["Gold5"], self.points["Pal1"],
                                 self.points["Pal6"]], direction="backward", finalangle=-pi / 4, lookahead=150)
        if self.side == Bornibus.PURPLE:
            wheeledbase.purepursuit([wheeledbase.get_position()[:2], self.points["Gold5"], self.points["Pal1"],
                                     self.points["Pal6"]], direction="backward", finalangle=pi / 4, lookahead=150)
        wheeledbase.wait()
        # Vers palets
        print("Vers Palets")
        wheeledbase.right_wheel_maxPWM.set(1)
        wheeledbase.left_wheel_maxPWM.set(1)
        # sens_manager.enable_back()
        if self.side == Bornibus.YELLOW:
            wheeledbase.purepursuit([wheeledbase.get_position()[:2], self.points["Pal3"],
                                     self.points["Pal4"]], direction="forward", finalangle=-pi/4, lookahead=150, lookaheadbis=150)
        if self.side == Bornibus.PURPLE:
            wheeledbase.purepursuit([wheeledbase.get_position()[:2], self.points["Pal3"],
                                     self.points["Pal4"]], direction="forward", finalangle=pi/4, lookahead=150)

        wheeledbase.wait()

        if self.side == Bornibus.YELLOW:
            wheeledbase.turnonthespot(-3*pi/4)
        else:
            wheeledbase.turnonthespot(3 * pi / 4)
        wheeledbase.wait()

        # Vers zone
        print("Vers Zone")
        # sens_manager.back_disable()
        # sens_manager.front_enable()
        pushers.down()

        if self.side == Bornibus.YELLOW:
            wheeledbase.purepursuit([wheeledbase.get_position()[:2], self.points["Pal5"], self.points["Pal6"]],
                                    direction="forward", lookahead=50, lookaheadbis=10, finalangle=-pi/2)
        if self.side == Bornibus.PURPLE:
            wheeledbase.purepursuit([wheeledbase.get_position()[:2], self.points["Pal5"], self.points["Pal6"]],
                                direction="forward", lookahead=50, lookaheadbis=10, finalangle=pi/2)
        wheeledbase.wait()

        disp.addPoints(13)

        # fin
        pushers.up()
        wheeledbase.stop()
        print("fin")
        disp.stop()
        """

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