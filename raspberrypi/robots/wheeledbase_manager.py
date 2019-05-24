from robots.sensors_manager import *
from math import copysign, hypot, pi, cos, sin, inf, isclose
from common.sync_flag_signal import Flag
from time import sleep
from threading import RLock, Event


class Mover:
    QUICK = 0
    SLOW = 1
    MEDIUM = 2
    MAP = ((0,0),(1600,0),(1600,3000),(100,3000),(0,0))
    SIDE_DIST = 200

    FORWARD = 0
    BACKWARD = 1

    LEFT = 0
    RIGHT = 1

    LATERAL_SHIFT = 300
    LONGITUDINAL_SHIFT = 200
    MINIMUM_WALL_DIST = 200
    AVOIDING_ZONE = ((100, 1600), (0, 3000))
    BALANCE_SEP_ZONE = ((1350, 2000), (1400, 1600))

    def __init__(self, wheeledbase, log_meth, sensorsFront, sensorsBack):
        self.wheeledbase = wheeledbase

        self.logger = log_meth
        self.front_center = SensorListener(sensorsFront[1])
        self.front_left = SensorListener(sensorsFront[2], threshold=150)
        self.front_right = SensorListener(sensorsFront[0], threshold=150)

        self.back_center = SensorListener(sensorsBack[1])
        self.back_left = SensorListener(sensorsBack[2], threshold=150)
        self.back_right = SensorListener(sensorsBack[0], threshold=150)

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
        self.params = {}
        self.goal = (0, 0, 0)
        self.timeout = 1




    def front_obstacle(self):
        # interuption quand obstacle devant:
        self.logger("MOVER : ", "Object in the front detected !")
        if not self.interupted_lock.acquire(blocking=True, timeout=0.5):
            self.logger("MOVER : ", "Abort !")
            return
        self.interupted_status.set()
        self.wheeledbase.stop()

        x, y, theta = self.wheeledbase.get_position()


        if hypot(x-self.goal[0],y-self.goal[1])<300:
            # Si on est proche de l'arrivé on fait rien 
            sleep(1)
            self.interupted_status.clear()
            self.interupted_lock.release()
            return 
        
        self.logger("MOVER : ", "Waiting for a second !")
        sleep(0.6)

        #determination du coté.

        robot_droite_vec = (cos(theta+pi/2), sin(theta+pi/2))

        distance = [0,0,0,0]

        for idx in range(len(Mover.MAP)-1):
            droite_vec = [Mover.MAP[idx+1][0]-Mover.MAP[idx][0], Mover.MAP[idx+1][1]-Mover.MAP[idx][1]]
            norm =  hypot(droite_vec[0], droite_vec[1])
            droite_vec[0] =  droite_vec[0]/norm
            droite_vec[1] = droite_vec[1]/norm
            a,b = Mover.MAP[idx]
            c,d = droite_vec
            i,j = robot_droite_vec

            if not isclose(-i*d+j*c,0,abs_tol=1e-3):
                det = -i*d+j*c
                distance[idx] = d*(x-a)/det -c*(y-b)/det

            else:
                distance[idx] = inf

        left = abs(max(filter(lambda x: x<0, distance)))
        right = abs(min(filter(lambda x: x>0, distance)))

        side = 1 if left<right else 1
        if side==1:
            print(" je vais a droite")
        else:
            print("sfsef ")
        self.wheeledbase.set_velocities(-200,0)
        sleep(0.5)
        self.wheeledbase.stop()
        self.wheeledbase.purepursuit(((x,y),(x+Mover.SIDE_DIST*robot_droite_vec[0]*side,y+Mover.SIDE_DIST*robot_droite_vec[1]*side),(self.goal)),**self.params)
        sleep(0.3)
        self.interupted_status.clear()
        self.interupted_lock.release()

    def lateral_obstacle(self, side):
        # interuption quand obstacle devant:
        self.logger("MOVER : ", "Lateral object detected !", "LEFT" if side==self.LEFT else "RIGHT")
        if not self.interupted_lock.acquire(blocking=True, timeout=0.5):
            self.logger("MOVER : ", "Abort !")
            return
        self.interupted_status.set()
        self.wheeledbase.stop()
        sleep(0.5)
        x, y, theta = self.wheeledbase.get_position()
        if self.direction == self.FORWARD:
            self.wheeledbase.goto_delta(-self.LONGITUDINAL_SHIFT, 0)
        else:
            self.wheeledbase.goto_delta(self.LONGITUDINAL_SHIFT, 0)
        self.logger("MOVER : ", "Waiting for backward")

        try:
            self.wheeledbase.wait()
        except:
            pass

        if side == self.LEFT:
            avoiding_pt = (x + math.sin(theta) * self.LATERAL_SHIFT, y + math.cos(theta) * self.LATERAL_SHIFT)
        elif side == self.RIGHT:
            avoiding_pt = (x - math.sin(theta) * self.LATERAL_SHIFT, y - math.cos(theta) * self.LATERAL_SHIFT)
        else:
            avoiding_pt = (x, y)

        x2, y2 = avoiding_pt
        self.logger("MOVER : Avoiding point ", avoiding_pt)
        if x2 < self.AVOIDING_ZONE[0][0] + self.MINIMUM_WALL_DIST or x2 > self.AVOIDING_ZONE[0][1] - self.MINIMUM_WALL_DIST or \
                y2 < self.AVOIDING_ZONE[1][0] + self.MINIMUM_WALL_DIST or y2 > self.AVOIDING_ZONE[1][1] - self.MINIMUM_WALL_DIST:
            self.logger("MOVER : ", "Cannot avoid")
            if self.direction == self.FORWARD:
                self.wheeledbase.goto_delta(-self.LONGITUDINAL_SHIFT, 0)
            else:
                self.wheeledbase.goto_delta(self.LONGITUDINAL_SHIFT, 0)

        elif self.BALANCE_SEP_ZONE[0][0] - self.MINIMUM_WALL_DIST < x2 < self.BALANCE_SEP_ZONE[0][1] + self.MINIMUM_WALL_DIST and \
                self.BALANCE_SEP_ZONE[1][0] - self.MINIMUM_WALL_DIST < y2 < self.BALANCE_SEP_ZONE[1][1] + self.MINIMUM_WALL_DIST:
            self.logger("MOVER : ", "Cannot avoid")
            if self.direction == self.FORWARD:
                self.wheeledbase.goto_delta(-self.LONGITUDINAL_SHIFT, 0)
            else:
                self.wheeledbase.goto_delta(self.LONGITUDINAL_SHIFT, 0)

        else:
            self.logger("MOVER : ", "Launch avoiding")
            self.wheeledbase.purepursuit([(x, y), avoiding_pt, *self.path], **self.params)

        self.interupted_status.clear()
        self.interupted_lock.release()


    def reset(self):
        self.interupted_lock.acquire()
        self.front_flag.clear()
        self.interupted_lock.release()
        self.params ={}

    def goto(self, x, y, **params):
        self.goto([self.wheeledbase.get_position()[:2], (x, y)], **params)

    def purepursuit(self, path, **params):
        self.params = params
        self.goal = path[-1]
        self.path = path
        # TODO ami
        # if False:
        #     self.in_path_flag.bind(self.friend_listener.signal)
        if self.params.get("direction") is 'forward' or self.params.get("direction") is None:
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
        self.logger("MOVER : ", "path ", self.path)
        self.wheeledbase.purepursuit(self.path, **self.params)
        # while hypot(x-self.goal[0], y-self.goal[1])>100:
        while not self.isarrived or self.interupted_status.is_set():
            try:
                if (self.goto_interrupt.is_set()):
                    break

                self.isarrived = self.wheeledbase.isarrived()
                sleep(0.1)
            except RuntimeError:
                if not self.interupted_lock.acquire(blocking=True, timeout=0.5):
                    while self.interupted_status.is_set():
                        sleep(0.1)
                    continue
                # self.logger("MOVER : ", "Spin! ")
                x, y, _ = self.wheeledbase.get_position()
                vel, ang = self.wheeledbase.get_velocities_wanted(True)
                self.wheeledbase.set_velocities(copysign(150, -vel), copysign(1, ang))
                time.sleep(1)  # 0.5
                self.wheeledbase.set_velocities(copysign(150, vel), 0)
                time.sleep(1.2)
                self.wheeledbase.purepursuit(self.path, **self.params)
                self.interupted_lock.release()
            except TimeoutError:
                self.isarrived = False

        # self.on_path_flag.clear()
        self.reset()
        if (self.goto_interrupt.is_set()):
            raise RuntimeError()