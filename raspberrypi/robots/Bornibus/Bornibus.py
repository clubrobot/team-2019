from robots.Bornibus.setup_bornibus import *
from common.geogebra import Geogebra
from robots.electron import *
from robots.automaton import Automaton

MAX_TIME_FOR_GOLDENIUM = 2


class Bornibus(Automaton):
    def __init__(self):
        Automaton.__init__(self)
        self.side = Bornibus.UNDEFINED
        self.points = dict()

    def set_position(self):
        if self.side == Bornibus.PURPLE:
            wheeledbase.set_position(*self.points["Ini"], -pi/2)
        if self.side == Bornibus.YELLOW:
            wheeledbase.set_position(*self.points["Ini"], pi/2)

    def set_side(self, side):
        import os
        roadmap = None
        for root, dirs, files in os.walk("."):
            for file in files:
                if file == "roadmap_bornibus.ggb":
                    roadmap = os.path.join(root, file)
        test_map = Geogebra(roadmap)

        self.points["Ini"] = test_map.get("Ini"+self.side)
        self.points["Gold3"] = test_map.get("Gold3"+self.side)
        self.points["Gold2"] = test_map.get("Gold2"+self.side)
        self.points["Gold4"] = test_map.get("Gold4"+self.side)
        self.points["Gold5"] = test_map.get("Gold5"+self.side)
        self.points["Gold6"] = test_map.get("Gold6"+self.side)

        self.points["Dep1"] = test_map.get("Dep1"+self.side)
        self.points["Dep2"] = test_map.get("Dep2"+self.side)
        self.points["Dep3"] = test_map.get("Dep3"+self.side)

        self.points["Pal1"] = test_map.get("Pal1"+self.side)
        self.points["Pal2"] = test_map.get("Pal2"+self.side)
        self.points["Pal3"] = test_map.get("Pal3"+self.side)
        self.points["Pal4"] = test_map.get("Pal4"+self.side)
        self.points["Pal5"] = test_map.get("Pal5"+self.side)
        self.points["Pal6"] = test_map.get("Pal6"+self.side)
        
        self.points["tmp"] = test_map.get("tmp"+self.side)
        self.points["tmp2"] = test_map.get("tmp2"+self.side)
        self.points["tmp3"] = test_map.get("tmp3"+self.side)
        self.side = side

    def run(self):
        Thread(target=stop_match).start()

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

        disp.start()
        disp.self.points = 0

        pushers.up()
        gripper.open()

        # Vers l'accélérateur
        print("Vers l'accélérateur")
        wheeledbase.purepursuit([wheeledbase.get_position()[:2], self.points["Dep1"], self.points["Dep2"],
                                 self.points["Dep3"], self.points["Gold2"]], direction="forward", lookahead=150,
                                lookaheadbis=3)
        wheeledbase.wait()

        wheeledbase.lookaheadbis.set(150)

        # Prépare le bras
        print("Prépare le bras")
        # sens_manager.disable_front()
        if self.side == Bornibus.YELLOW:
            wheeledbase.turnonthespot(-pi/4)
            while not wheeledbase.isarrived():
                print(wheeledbase.get_position())
                time.sleep(0.1)
        if self.side == Bornibus.PURPLE:
            wheeledbase.turnonthespot(-(3*pi)/4)
            while not wheeledbase.isarrived():
                print(wheeledbase.get_position())
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

        try:
            wheeledbase.goto(*self.points["Gold4"], theta=pi)
            wheeledbase.wait()
        except ...:
            pass

        # Prise goldenium
        print("Prise Goldenium")
        gripper.close()
        time.sleep(1.5)

        if gripper.get_goldsensor_state():
            disp.addPoints(20)

        wheeledbase.right_wheel_maxPWM.set(1)
        wheeledbase.left_wheel_maxPWM.set(1)

        # Vers balance
        print("Vers Balance")
        # sens_manager.enable_back()
        wheeledbase.purepursuit([wheeledbase.get_position()[:2], self.points["Gold3"], self.points["Gold5"]],
                                direction="backward", lookahead=150, lookaheadbis=100)
        wheeledbase.wait()
        wheeledbase.max_linacc.set(300.0)
        wheeledbase.max_lindec.set(300.0)
        wheeledbase.max_linvel.set(400)
        wheeledbase.turnonthespot(0)
        wheeledbase.wait()

        # Dépose balance
        print("Dépose Balance")
        # sens_manager.disable_back()

        try:
            wheeledbase.goto(*self.points["Gold6"], theta=0)
            wheeledbase.wait()
        except ...:
            gripper.open()
            print("error")

        wheeledbase.reset_parameters()
        wheeledbase.max_linacc.set(500.0)
        wheeledbase.max_lindec.set(500.0)

        # Depose
        print("Dépose")
        time.sleep(0.5)

        if gripper.get_goldsensor_state():
            disp.addPoints(24)
        gripper.open()

        time.sleep(0.5)

        # Vers palets
        print("Vers Palets")
        # sens_manager.enable_back()
        if self.side == Bornibus.YELLOW:
            wheeledbase.purepursuit([wheeledbase.get_position()[:2], self.points["Gold5"], self.points["Pal1"],
                                     self.points["Pal4"]], direction="backward", finalangle=-pi/4)
        if self.side == Bornibus.PURPLE:
            wheeledbase.purepursuit([wheeledbase.get_position()[:2], self.points["Gold5"], self.points["Pal1"],
                                     self.points["Pal4"]], direction="backward", finalangle=pi/4)

        wheeledbase.wait()

        if self.side == Bornibus.YELLOW:
            wheeledbase.turnonthespot(-5*pi/6)
        else:
            wheeledbase.turnonthespot(5 * pi / 6)
        wheeledbase.wait()

        # Vers zone
        print("Vers Zone")
        # sens_manager.back_disable()
        # sens_manager.front_enable()
        pushers.down()

        wheeledbase.purepursuit([wheeledbase.get_position()[:2], self.points["Pal5"], self.points["Pal6"]],
                                direction="forward")
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