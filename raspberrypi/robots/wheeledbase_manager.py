from robots.get_robot_name import *
from robots.sensors_manager import *
from math import copysign, hypot, pi, cos, sin, inf, isclose
from common.sync_flag_signal import Flag
from time import sleep
from threading import RLock, Event
class Mover:
    QUICK  = 0
    SLOW   = 1
    MEDIUM = 2
    MAP = ((0,0),(1600,0),(1600,3000),(0,3000),(0,0))
    SIDE_DIST = 200
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
        self.lat_left_flag = Flag(lambda : self.lat_obstacle("left"))
        self.lat_right_flag = Flag(lambda: self.lat_obstacle("right"))

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


    def lat_obstacle(self, side="left"):
        self.logger("MOVER : ", "Object in the lat detected !")
        self.logger("MOVER : ", "Object in the front detected !")   
        if not self.interupted_lock.acquire(blocking=True, timeout=0.5):
            self.logger("MOVER : ", "Abort !")
            return
        self.interupted_status.set()
        self.wheeledbase.stop()
        vel, _ = self.wheeledbase.get_velocities_wanted(True)
        ang = {"left":1, "right":-1}[side]
        self.wheeledbase.set_velocities(copysign(150, -vel), copysign(1, ang))
        time.sleep(1)  # 0.5
        self.wheeledbase.set_velocities(copysign(150, vel), 0)
        time.sleep(1.2)
        self.wheeledbase.stop()
        self.interupted_lock.release()
        self.interupted_status.clear()

    def reset(self):
        self.interupted_lock.acquire()
        self.wheeledbase.reset_parameters()
        self.front_flag.clear()
        self.interupted_lock.release()
        self.params ={}

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

        self.logger("MOVER : ", "path ", ((x,y),(self.goal)))
        self.isarrived = False
        x, y, _ = self.wheeledbase.get_position()
        self.wheeledbase.purepursuit(((x,y),(self.goal)),**self.params)


        while hypot(x-self.goal[0],y-self.goal[1])>100:
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
                    self.logger("MOVER : ", "Spin! ")
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

            x, y, _ = self.wheeledbase.get_position()

        # self.on_path_flag.clear()
        
        self.reset()
        if (self.goto_interrupt.is_set()):
            raise RuntimeError()