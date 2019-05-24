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

    FORWARD = 0
    BACKWARD = 1

    LEFT  = 0
    RIGHT = 1

    LATERAL_SHIFT = 200
    MINIMUM_GAP = 300

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
        self.left_flag = Flag(lambda: self.lateral_obstacle(self.LEFT))
        self.right_flag = Flag(lambda: self.lateral_obstacle(self.RIGHT))

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

    def lateral_obstacle(self, side):
        # interuption quand obstacle devant:
        self.logger("MOVER : ", "Lateral object detected !")
        if not self.interupted_lock.acquire(blocking=True, timeout=0.5):
            self.logger("MOVER : ", "Abort !")
            return
        self.interupted_status.set()
        if self.direction == self.FORWARD:
            self.wheeledbase.goto_delta(-100, 0)
        else:
            self.wheeledbase.goto_delta(100, 0)
        self.logger("MOVER : ", "Waiting for backward")
        sleep(2)
        x, y, theta = self.wheeledbase.get_position()
        if side == self.LEFT:
            lateral_point = (x + math.sin(theta)*self.LATERAL_SHIFT, y + math.cos(theta)*self.LATERAL_SHIFT)
        elif side == self.RIGHT:
            lateral_point = (x - math.sin(theta)*self.LATERAL_SHIFT, y - math.cos(theta)*self.LATERAL_SHIFT)
        else:
            lateral_point = (x, y)

        self.logger("MOVER : ", "Launch evitement")
        self.wheeledbase.purepursuit(((x, y), lateral_point, self.goal), **self.params)
        time.sleep(1)
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
        if self.params.get("direction") == 'forward':
            self.logger("MOVER : ", "forward")
            self.direction = self.FORWARD
            self.front_flag.bind(self.front_center.signal)
            self.left_flag.bind(self.front_left.signal)
            self.right_flag.bind(self.front_right.signal)

        else:
            self.logger("MOVER : ", "backward")
            self.direction = self.BACKWARD
            self.front_flag.bind(self.back_center.signal)
            self.left_flag.bind(self.back_left.signal)
            self.right_flag.bind(self.back_right.signal)

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