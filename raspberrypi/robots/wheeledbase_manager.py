from robots.get_robot_name import *
from robots.sensors_manager import *
from math import copysign, hypot, pi
from common.sync_flag_signal import Flag
from time import sleep
from threading import RLock, Event
class Mover:
    QUICK  = 0
    SLOW   = 1
    MEDIUM = 2

    def __init__(self, wheeledbase, log_meth, sensorsFront, sensorsBack):
        self.wheeledbase = wheeledbase
        
        self.logger = log_meth
        self.front_center = SensorListener(sensorsFront[1])
        self.front_left   = SensorListener(sensorsFront[2])
        self.front_right  = SensorListener(sensorsFront[0])

        self.back_center = SensorListener(sensorsBack[1])
        self.back_left   = SensorListener(sensorsBack[2])
        self.back_right  = SensorListener(sensorsBack[0])

        self.front_flag = Flag(self.front_obstacle)

        self.isarrived = False
        self.interupted_lock = RLock()
        self.interupted_status = Event()
        self.interupted_timeout = Event()
        self.goto_interrupt = Event()
        self.running = Event()
        self.direction = "forward"
        self.params ={}
        self.goal = (0, 0, 0)
        self.timeout = 1

    def front_obstacle(self):
        #interuption quand obstacle devant:
        self.logger("MOVER : ", "Object in the front detected !")
        if not self.interupted_lock.acquire(blocking=True, timeout=0.5):
            self.logger("MOVER : ", "Abort !")
            return
        self.interupted_status.set()
        self.wheeledbase.stop()
        self.logger("MOVER : ", "Waiting for a second !")
        sleep(1)
        x, y, _ = self.wheeledbase.get_position()
        self.wheeledbase.purepursuit(((x,y),(self.goal)),**self.params)

        self.interupted_status.clear()
        self.interupted_lock.release()

    def reset(self):
        self.interupted_lock.acquire()
        # self.wheeledbase.reset_parameters()
        self.front_flag.clear()
        self.interupted_lock.release()
        self.params = {}

    def goto(self, x, y, **params):
        self.params = params
        self.goal = (x, y)
        #TODO ami
        if False :
            self.in_path_flag.bind(self.friend_listener.signal)
        if self.params.get("direction","forward") =='forward':
            self.front_flag.bind(self.front_center.signal)
        else:
            self.front_flag.bind(self.back_center.signal)

        self.isarrived = False
        x, y, _ = self.wheeledbase.get_position()
        self.logger("MOVER : ", "path ", ((x,y),(self.goal)))
        self.wheeledbase.purepursuit(((x,y),(self.goal)),**self.params)
        # while hypot(x-self.goal[0], y-self.goal[1])>100:
        while not self.isarrived or self.interupted_status.is_set():
            try:
                if(self.goto_interrupt.is_set()):
                    break

                self.isarrived = self.wheeledbase.isarrived()
                sleep(0.1)
            except RuntimeError:
                if not self.interupted_lock.acquire(blocking=True, timeout=0.5):
                    while self.interupted_status.is_set():
                        sleep(0.1)
                    continue
                #self.logger("MOVER : ", "Spin! ")
                x, y, _ = self.wheeledbase.get_position()
                vel, ang = self.wheeledbase.get_velocities_wanted(True)
                self.wheeledbase.set_velocities(copysign(150, -vel), copysign(1, ang))
                time.sleep(1)  # 0.5
                self.wheeledbase.set_velocities(copysign(150, vel), 0)
                time.sleep(1.2)
                self.wheeledbase.purepursuit(((x,y),(self.goal)),**self.params)
                self.interupted_lock.release()
            except TimeoutError:
                self.isarrived = False

        # self.on_path_flag.clear()
        
        self.reset()
        if (self.goto_interrupt.is_set()):
            raise RuntimeError()