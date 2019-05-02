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

couleur = "YELLOW"

class gripperError(Exception) :
    def __init__(self,*args, **kwargs) :
        Exception.__init__(self,*args, kwargs)

class Bornibus(Automaton):

    def __init__(self):
        Automaton.__init__(self)

        # Save daughter_cards
        self.daughter_cards = dict( wheeledbase     = wheeledbase, 
                                    display         = disp)

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

    def set_position(self):
        if self.side == Bornibus.PURPLE:
            wheeledbase.set_position(*self.points["Ini"], -pi/2)
            print(*self.points["Ini"])
        if self.side == Bornibus.YELLOW:
            wheeledbase.set_position(*self.points["Ini"], pi/2)
            print(*self.points["Ini"])

    def set_side(self, side):
        self.side = side
        color = "M" if side == self.PURPLE else "O"

        self.points["Ini"] = geo.get("Ini"+color)

        self.points["Dep1"] = geo.get("Dep1"+color)
        self.points["Dep2"] = geo.get("Dep2"+color)
        self.points["Dep3"] = geo.get("Dep3"+color)

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

    def stop_match(self):
        import time
        time.sleep(100)
        wheeledbase.stop()
        gripper.open()
        pushers.up()
        arm.up()
        manager.disconnect()

    def run(self):
        Thread(target=self.stop_match).start()
        begin = time.time()

        sens_manager.start()
        wheeledbase.reset_parameters()
        wheeledbase.max_linvel.set(700)
        wheeledbase.max_angvel.set(10)
        wheeledbase.lookahead.set(150.0)
        wheeledbase.max_linacc.set(500.0)
        wheeledbase.max_lindec.set(700.0)

        print("max lin vel : ", wheeledbase.max_linvel.get())
        print("max ang vel : ", wheeledbase.max_angvel.get())

        print("disable back")
        # sens_manager.disable_back()
        # sens_manager.set_thresold(150)
        print("sens_manager start")
        # sens_manager.start()
        # time.sleep(2)
        print("max ang vel : ", wheeledbase.max_angvel.get())
        print("max lin vel : ", wheeledbase.max_linvel.get())

        disp.points = 0
        disp.start()

        pushers.up()
        gripper.open()
        
        # Vers l'accélérateur
        print("Vers l'accélérateur")
        wheeledbase.purepursuit([wheeledbase.get_position()[:2], self.points["Dep1"], self.points["Dep2"],
                                 self.points["Dep3"], self.points["Gold2"]], direction="forward", lookahead=150, lookaheadbis=3)
        wheeledbase.wait()
        print(wheeledbase.get_position())
        wheeledbase.turnonthespot(0)
        wheeledbase.wait()
        wheeledbase.set_velocities(-200, 0)
        time.sleep(2)
        wheeledbase.set_velocities(0, 0)

        if self.side == Bornibus.YELLOW :
            wheeledbase.goto_delta(100, 30)
            
        if self.side == Bornibus.PURPLE :
            wheeledbase.goto_delta(100, -30)
            
        wheeledbase.wait()

        # wheeledbase.turnonthespot(-2*pi/3)
        # wheeledbase.wait()
        # arm.up()
        wheeledbase.lookaheadbis.set(150)

        # Prépare le bras
        print("Prépare le bras")
        # sens_manager.disable_front()
        if self.side == Bornibus.YELLOW:
            arm.deploy()
            wheeledbase.turnonthespot(-2*pi/3)
            while not wheeledbase.isarrived():
                time.sleep(0.1)
        if self.side == Bornibus.PURPLE:
            wheeledbase.turnonthespot(-3*pi/4)
            wheeledbase.wait()
            arm.deploy()
            wheeledbase.turnonthespot(-pi/3)
            while not wheeledbase.isarrived():
                time.sleep(0.1)

        time.sleep(0.5)
        arm.deploy()

        # Pousse
        print("Pousse")
        wheeledbase.turnonthespot(-pi/2)
        while not wheeledbase.isarrived():
            print(wheeledbase.get_position())
            time.sleep(0.1)

        arm.up()
        disp.addPoints(20)
        gripper.open()

        # Vers préparation Goldenium
        print("Vers préparation Goldenium")
        # sens_manager.enable_front()
        wheeledbase.goto(*self.points["Gold3"], theta=pi)
        while not wheeledbase.isarrived():
            print(wheeledbase.get_position())
            time.sleep(0.1)

        # sens_manager.disable_front()

        # Vers Goldenium
        print("Vers Goldenium")
        wheeledbase.right_wheel_maxPWM.set(0.2)
        wheeledbase.left_wheel_maxPWM.set(0.2)

        goldenium = 0

        while goldenium != 1 :
            wheeledbase.purepursuit([wheeledbase.get_position()[:2], self.points["Gold4"]], direction="forward", lookaheadbis=1, finalangle=pi)
            try :
                while not wheeledbase.isarrived() :
                    if endstops.get_ES2():
                        # Touched left
                        print("TOUCHED LEFT")
                        self.points["Gold4"] = (self.points["Gold4"][0], self.points["Gold4"][1]-10)
                        self.points["Gold3"] = (self.points["Gold3"][0], self.points["Gold3"][1]-10)
                        print(*self.points["Gold4"])
                        raise gripperError("Grripper left touched")
                    
                    elif endstops.get_ES1():
                        # Touched right
                        print("TOUCHED RIGHT")
                        self.points["Gold4"] = (self.points["Gold4"][0], self.points["Gold4"][1]+10)
                        self.points["Gold3"] = (self.points["Gold3"][0], self.points["Gold3"][1]+10)
                        print(*self.points["Gold4"])
                        raise gripperError("Grripper right touched")
                    time.sleep(0.1)
                goldenium = 1
            
            except gripperError as e :
                print("gripperException : ", e)
                wheeledbase.goto(*self.points["Gold3"], theta=pi, lookaheadbis=1)

            except BaseException as e :
                print("BaseException : ", e)
                goldenium=1
            
         
        wheeledbase.lookaheadbis.set(150)
        # Prise goldenium
        print("Prise Goldenium")
        gripper.close()
        time.sleep(1)

        wheeledbase.goto_delta(-100, 0)
        wheeledbase.wait()

        if endstops.get_ES3():
            disp.addPoints(20)

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

if __name__ == "__main__":
    auto = Bornibus()
    auto.set_side(Bornibus.YELLOW)
    init_robot()
    auto.set_position()
    print("ready")
    input()
    auto.run()
    pass