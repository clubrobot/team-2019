from robots.sensors_manager import *
from math import copysign, hypot, pi, cos, sin, inf, isclose
from common.sync_flag_signal import Flag
from time import sleep
from threading import RLock, Event

class PositionUnreachable(RuntimeError):
    pass

class Mover:
    QUICK = 0
    SLOW = 1
    MEDIUM = 2
    MAP = ((0,0),(1600,0),(1600,3000),(100,3000),(0,0))
    SIDE_DIST = 200

    FORWARD = "forward"
    BACKWARD = "backward"

    LEFT = 0
    RIGHT = 1

    AIM=6
    SOFT=8

    LATERAL_SHIFT = 400
    LONGITUDINAL_SHIFT = 250
    MINIMUM_WALL_DIST = 200
    AVOIDING_ZONE = ((100, 1600), (0, 3000))
    BALANCE_SEP_ZONE = ((1350, 2000), (1400, 1600))
    TIME_BETWEEN_OBSTACLES = 3

    def __init__(self, daughter_cards, log_meth, sensorsFront, sensorsBack):
        self.wheeledbase = daughter_cards["wheeledbase"]
        self.display = daughter_cards["display"]

        self.logger = log_meth
        self.front_center = SensorListener(sensorsFront[1], threshold=300)
        self.front_left = SensorListener(sensorsFront[2], threshold=100)
        self.front_right = SensorListener(sensorsFront[0], threshold=100)

        self.back_center = SensorListener(sensorsBack[1], threshold=300)
        self.back_left = SensorListener(sensorsBack[2], threshold=100)
        self.back_right = SensorListener(sensorsBack[0], threshold=100)

        self.front_flag = Flag(self.front_obstacle)
        self.left_flag = Flag(lambda: self.lateral_obstacle(self.LEFT))
        self.right_flag = Flag(lambda: self.lateral_obstacle(self.RIGHT))
        self.withdraw_flag = Flag(self.withdraw_obstacle)

        self.isarrived = False
        self.interupted_lock = RLock()
        self.interupted_status = Event()
        self.interupted_timeout = Event()
        self.goto_interrupt = Event()
        self.running = Event()
        self.direction = "forward"
        self.params = {}
        self.goal = (0, 0)
        self.nb_try = 0
        self.timeout = 1
        self.try_limit = 4
        self.safe_mode = False
        self.disable_sensors()
        self.last_obstacle = None

    def disable_sensors(self):
        self.front_center.disable()
        self.front_left.disable()
        self.front_right.disable()
        self.back_center.disable()
        self.back_left.disable()
        self.back_right.disable()

    def enable_sensors(self, direction=None):
        if direction is None or direction == Mover.FORWARD:
            self.front_center.enable()
            self.front_left.enable()
            self.front_right.enable()
        if direction is None or direction == Mover.BACKWARD: 
            self.back_center.enable()
            self.back_left.enable()
            self.back_right.enable()

    def front_obstacle(self):
        if (self.goto_interrupt.is_set()):
            return

        # interuption quand obstacle devant:
        self.logger("MOVER : ", "Object in the front detected !")
        if not self.interupted_lock.acquire(blocking=True, timeout=0.5):
            self.logger("MOVER : ", "Abort !")
            return
        self.interupted_status.set()
        self.wheeledbase.set_velocities(0,0)
        sleep(1)
        if self.safe_mode:
            self.logger("MOVER : ", "Just wait a little (Safe mode ON) !")
            obstacle = True
            while obstacle:
                sleep(1)
                obstacle  = self.front_center.get_dist() if self.direction == Mover.FORWARD else self.back_center.get_dist()
                if obstacle: self.logger("MOVER : ", " Steel an obstacle on path, wait 1s more !")
            self.wheeledbase.start_purepursuit()
            self.interupted_status.clear()
            self.interupted_lock.release()
            self.logger("Mover : ", "Interuption end")
            return


        self.wheeledbase.stop()
        self.display.angry()
        x, y, theta = self.wheeledbase.get_position()

        if hypot(x-self.goal[0],y-self.goal[1])<300:
            # Si on est proche de l'arrivé on fait rien 
            obstacle = True
            while obstacle and self.nb_try<=self.try_limit :
                self.nb_try +=1
                sleep(1)
                self.logger("MOVER : Try number {}, {} tries remaning",self.nb_try, 4)
                obstacle  = self.front_center.get_dist() if self.direction == Mover.FORWARD else self.back_center.get_dist()
                if obstacle: self.logger("MOVER : ", " Steel an obstacle on the objectif!")
                if self.nb_try>self.try_limit:
                    self.goto_interrupt.set()
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
            self.logger("MOVER : ", "go LEFT !")
        else:
            self.logger("MOVER : ", "go RIGHT !")
        if self.direction == self.FORWARD:
            self.wheeledbase.goto_delta(-self.LONGITUDINAL_SHIFT, 0)
        else:
            self.wheeledbase.goto_delta(self.LONGITUDINAL_SHIFT, 0)
        sleep(0.5)
        self.wheeledbase.stop()
        self.wheeledbase.purepursuit(((x,y),(x+Mover.SIDE_DIST*robot_droite_vec[0]*side,y+Mover.SIDE_DIST*robot_droite_vec[1]*side),(self.goal)),**self.params)
        sleep(0.3)

        self.last_obstacle = time.time()

        self.interupted_status.clear()
        self.interupted_lock.release()


    def lateral_obstacle(self, side):
        if (self.goto_interrupt.is_set()):
            return

        if self.last_obstacle and time.time() - self.last_obstacle < self.TIME_BETWEEN_OBSTACLES:
            self.logger("MOVER : last front obstacle too close !")
            return

        # interuption quand obstacle devant:
        self.logger("MOVER : ", "Lateral object detected !", "LEFT" if side==self.LEFT else "RIGHT")

        if not self.interupted_lock.acquire(blocking=True, timeout=0.5):
            self.logger("MOVER : ", "Abort !")
            return
        self.interupted_status.set()
        self.wheeledbase.set_velocities(0,0)
        sleep(1)
        self.wheeledbase.stop()
        self.display.angry()

        x, y, theta = self.wheeledbase.get_position()
        if self.safe_mode:
            self.logger("MOVER : ", "Just wait a little (Safe mode ON) !")
            sleep(1)
            self.wheeledbase.start_purepursuit()
            self.interupted_status.clear()
            self.interupted_lock.release()
            self.logger("Mover : ", "Interuption end")
            return


        if hypot(x-self.goal[0],y-self.goal[1])<300:
            # Si on est proche de l'arrivé on fait rien 
            sleep(1.5)
            self.nb_try +=1
            if self.nb_try>self.try_limit:
                self.goto_interrupt.set()
            self.interupted_status.clear()
            self.interupted_lock.release()
            return 
        
        sleep(0.5)
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
            sleep(0.5)
            self.wheeledbase.purepursuit([(x, y), self.goal], **self.params)



        elif self.BALANCE_SEP_ZONE[0][0] - self.MINIMUM_WALL_DIST < x2 < self.BALANCE_SEP_ZONE[0][1] + self.MINIMUM_WALL_DIST and \
                self.BALANCE_SEP_ZONE[1][0] - self.MINIMUM_WALL_DIST < y2 < self.BALANCE_SEP_ZONE[1][1] + self.MINIMUM_WALL_DIST:
            self.logger("MOVER : ", "Cannot avoid")
            if self.direction == self.FORWARD:
                self.wheeledbase.goto_delta(-self.LONGITUDINAL_SHIFT, 0)
            else:
                self.wheeledbase.goto_delta(self.LONGITUDINAL_SHIFT, 0)

        else:
            self.logger("MOVER : ", "Launch avoiding")
            self.wheeledbase.purepursuit([(x, y), avoiding_pt, self.goal], **self.params)

        self.interupted_status.clear()
        self.interupted_lock.release()

    def reset(self):
        self.front_flag.clear()
        self.interupted_status.clear()
        self.goto_interrupt.clear()
        self.params ={}
        self.disable_sensors()
        self.nb_try = 0
        self.try_limit = 4

    def goto(self, x, y, safe_mode=False,**params):
        
        if params.get("direction", None) is None:
            x0, y0, theta0 = self.wheeledbase.get_position()
            if math.cos(math.atan2(y - y0, x - x0) - theta0) >= 0:
                params["direction"]  = 'forward'
            else:
                params["direction"] = 'backward'
        self.purepursuit([self.wheeledbase.get_position()[:2], (x, y)],safe_mode=safe_mode, **params)
        if not params.get("theta", None)  is None:
            self.turnonthespot(params["theta"])
            #self.wheeledbase.wait()

    def purepursuit(self, path, nb_try=4, safe_mode=False, **params):
        self.params = params

        self.goal = path[-1]
        self.path = path
        self.nb_try = 0
        self.try_limit = nb_try
        self.safe_mode = safe_mode
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

        self.enable_sensors(direction=self.direction)
        
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
                self.logger("MOVER : ", "Spin! ")
                self.display.sick()
                if self.safe_mode:
                    sleep(0.2)
                    #self.wheeledbase.purepursuit(self.path, **self.params)
                    self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.goal], **self.params)
                    self.interupted_lock.release()
                    continue
                x, y, _ = self.wheeledbase.get_position()
                vel, ang = self.wheeledbase.get_velocities_wanted(True)

                threshold = 200
                if vel > 0:
                    while self.back_center.getter(threshold) or self.back_left.getter(threshold) or self.back_right.getter(threshold):
                        time.sleep(0.3)
                else:
                    while self.front_center.getter(threshold) or self.front_left.getter(threshold) or self.front_right.getter(threshold):
                        time.sleep(0.3)

                self.wheeledbase.set_velocities(copysign(150, -vel), copysign(1, ang))
                time.sleep(1)  # 0.5
                self.wheeledbase.stop()
                if vel < 0:
                    while self.back_center.getter(threshold) or self.back_left.getter(threshold) or self.back_right.getter(threshold):
                        time.sleep(0.3)
                else:
                    while self.front_center.getter(threshold) or self.front_left.getter(threshold) or self.front_right.getter(threshold):
                        time.sleep(0.3)
                self.wheeledbase.set_velocities(copysign(150, vel), 0)
                time.sleep(1)
                self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.goal], **self.params)
                self.interupted_lock.release()
            except TimeoutError:
                self.isarrived = False

        # self.on_path_flag.clear()
        self.reset()
        if (self.goto_interrupt.is_set()):
            raise PositionUnreachable()
    
    def turnonthespot_dir(self, theta, try_limit=3, direction="clock"):
        self.enable_sensors()
        self.goal = self.wheeledbase.get_position()[:-1]
        self.nb_try = 0
        self.try_limit = try_limit
        arrived = False
        while not arrived and self.nb_try<self.try_limit:
            try:
                self.wheeledbase.turnonthespot(theta, direction=direction)
                self.wheeledbase.wait()
                arrived = True
            except:
                self.logger("MOVER : Turnonthespot spin")
                self.nb_try+=1
                if self.nb_try==1:
                    sleep(0.6)
                # on regarde les capteurs
                if self.nb_try>=2:
                    sleep(0.2)
                    if direction=="clock":
                        if self.back_right.getter(60) or self.front_left.getter(60):
                            self.logger("MOVER ; Object on my side, wait 1s")
                            self.wheeledbase.left_wheel_maxPWM.set(0.5)
                            self.wheeledbase.right_wheel_maxPWM.set(0.5) 
                            sleep(1)
                    if direction=="trig":
                        if self.back_left.getter(60) or self.front_right.getter(60):
                            self.logger("MOVER ; Object on my side, wait 1s")
                            self.wheeledbase.left_wheel_maxPWM.set(0.5)
                            self.wheeledbase.right_wheel_maxPWM.set(0.5) 
                            sleep(1)
        if self.try_limit>0:
            self.wheeledbase.left_wheel_maxPWM.set(1)
            self.wheeledbase.right_wheel_maxPWM.set(1)
        self.disable_sensors()
        if  self.nb_try>=self.try_limit:
            raise PositionUnreachable()
  
    def turnonthespot(self, theta, try_limit=3, way="forward"):
        self.enable_sensors()
        self.goal = self.wheeledbase.get_position()[:-1]
        self.nb_try = 0
        self.try_limit = try_limit
        arrived = False
        while not arrived and self.nb_try<self.try_limit:
            try:
                self.wheeledbase.turnonthespot(theta, way=way)
                self.wheeledbase.wait()
                arrived = True
            except: 
                self.nb_try+=1
                if self.nb_try==1:
                    sleep(0.6)
                # on regarde les capteurs
                if self.nb_try>=2:
                    sleep(0.2)
                    _, ang_vel = self.wheeledbase.get_velocities_wanted(True)

                    if ang_vel>=0:
                        if self.back_right.getter(60) or self.front_left.getter(60):
                            self.logger("MOVER ; Object on my side, wait 1s")
                            self.wheeledbase.left_wheel_maxPWM.set(0.5)
                            self.wheeledbase.right_wheel_maxPWM.set(0.5) 
                            sleep(1)
                    else:
                        if self.back_left.getter(60) or self.front_right.getter(60):
                            self.logger("MOVER ; Object on my side, wait 1s")
                            self.wheeledbase.left_wheel_maxPWM.set(0.5)
                            self.wheeledbase.right_wheel_maxPWM.set(0.5) 
                            sleep(1)
        self.disable_sensors()
        if self.try_limit>0:
                self.wheeledbase.left_wheel_maxPWM.set(1)
                self.wheeledbase.right_wheel_maxPWM.set(1)             
        if  self.nb_try>=self.try_limit:
            raise PositionUnreachable()

    def turnonthespot_hard(self, theta, try_limit=3, way="forward"):
        self.enable_sensors()
        self.goal = self.wheeledbase.get_position()[:-1]
        self.nb_try = 0
        self.try_limit = try_limit
        arrived = False
        while not arrived and self.nb_try<self.try_limit:
            try:
                self.wheeledbase.turnonthespot(theta, way=way)
                self.wheeledbase.wait()
                arrived = True
            except: 
                self.nb_try+=1
                sleep(0.2)
                _, ang_vel = self.wheeledbase.get_velocities_wanted(True)
                self.wheeledbase.left_wheel_maxPWM.set(0.5)
                self.wheeledbase.right_wheel_maxPWM.set(0.5) 
                if ang_vel>=0:
                    self.wheeledbase.goto_delta(-100, 0)
                else:
                    self.wheeledbase.goto_delta(100, 0)

                sleep(0.4)
                self.wheeledbase.stop()
        self.disable_sensors()
        if self.try_limit>0:
                self.wheeledbase.left_wheel_maxPWM.set(1)
                self.wheeledbase.right_wheel_maxPWM.set(1)             
        if  self.nb_try>=self.try_limit:
            raise PositionUnreachable()

    def withdraw_obstacle(self):
        if (self.goto_interrupt.is_set()):
            return
        if not self.interupted_lock.acquire(blocking=True, timeout=0.5):
            return

        self.interupted_status.set()
        # interuption quand obstacle devant:
        self.logger("MOVER : ", "Withdraw obstacle => stop")

        self.wheeledbase.stop()
        time.sleep(1)
        self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.goal], direction=self.direction)

        self.interupted_status.clear()
        self.interupted_lock.release()

    def withdraw(self, x, y, direction="forward", timeout=5):
        self.enable_sensors()
        w_x, w_y, _ = self.wheeledbase.get_position()
        self.goal = (w_x + x, w_y + y)
        self.timeout = timeout
        self.wheeledbase.max_linvel.set(200)
        self.direction = direction
        if direction == "forward":
            self.logger("MOVER : ", "forward")
            self.withdraw_flag.bind(self.front_center.signal)
            self.withdraw_flag.bind(self.front_left.signal)
            self.withdraw_flag.bind(self.front_right.signal)

        else:
            self.logger("MOVER : ", "backward")
            self.withdraw_flag.bind(self.back_center.signal)
            self.withdraw_flag.bind(self.back_left.signal)
            self.withdraw_flag.bind(self.back_right.signal)

        self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.goal], direction=self.direction)
        while not self.isarrived or self.interupted_status.is_set():
            try:
                if self.interupted_status.is_set():
                    sleep(0.2)
                else:
                    self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.goal],
                                                 direction=self.direction)
                self.isarrived = True

            except RuntimeError:
                if self.interupted_timeout.is_set():
                    self.wheeledbase.stop()
                    time.sleep(1)
                    self.wheeledbase.purepursuit([self.wheeledbase.get_position()[:2], self.goal],
                                                 direction=self.direction)
                else:
                    pass
        self.reset()
        if (self.goto_interrupt.is_set()):
            raise PositionUnreachable()
