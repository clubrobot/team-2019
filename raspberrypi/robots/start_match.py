from robots.buttons_manager import ButtonsManager


def start_preparation():
    import robots.goldenium_bornibus as Bornibus
    manager = Bornibus.Manager()
    manager.connect()
    Bornibus.init_robot()
    ButtonsManager(Bornibus.map_loading, Bornibus.def_pos, start_robot).begin()


def start_robot(points, couleur):
    import robots.goldenium_bornibus as Bornibus
    try:
        Bornibus.start(points, couleur)
    except RuntimeError as e:
        print("error : ", e)
    finally:
        Bornibus.disp.stop()
