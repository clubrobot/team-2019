#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
from threading import Thread, Event, Lock
import math
from common.sync_flag_signal import Signal
from robots.automaton import *



class SensorListener(Thread):
    def __init__(self, sensors, timestep = 0.05, threshold = 300):
        Thread.__init__(self)
        self.daemon = True
        self.signal   = Signal()
        self.getter   = sensors.obstacle
        self.timestep = timestep
        self.stop  = Event()
        self.threshold = threshold
        self.error    = 0
        self.start()
        self.run_event = Event()
        self.run_event.set()

    def enable(self):
        self.run_event.set()
    
    def disable(self):
        self.run_event.clear()

    def get_dist(self):
        return self.getter(self.threshold)

    def run(self):
        while not self.stop.is_set():
            time.sleep(self.timestep)
            if not self.run_event.is_set():
                self.run_event.wait()
            try:
                obstacle = self.getter(self.threshold)
            except TimeoutError:
                continue
            if obstacle:
                self.signal.ping()


class Sensor:
    HILL_ZONE = ((300, 1400), (200, 2800))
    SCALE_ZONE = ((1150, 1600), (1200, 1800))
    START_ZONE = ((0, 0), (0, 0))

    def __init__(self, wheeledbase, name, dist, pos, angle, connexion_flag):
        self.dist = dist
        self.pos = pos
        self.angle = angle
        self.enabled = True
        self.name = name
        self.wheeledbase = wheeledbase
        self.is_connected = connexion_flag

    def set_side(self, side):
        if side == Automaton.YELLOW:
            self.START_ZONE = ((0, 1200), (0, 500))
        else:
            self.START_ZONE = ((0, 1200), (2500, 3000))

    def disable(self):
        self.enabled = False

    def obstacle(self, threshold):
        dist = self.dist()
        if dist > threshold:
            return False
        wheeledbase_pos = self.wheeledbase.get_position()
        
        x = wheeledbase_pos[0] + self.pos[0]*math.cos(wheeledbase_pos[2]) - self.pos[1]*math.sin(wheeledbase_pos[2]) \
            + math.cos(self.angle+wheeledbase_pos[2])*dist
        y = wheeledbase_pos[1] + self.pos[0]*math.sin(wheeledbase_pos[2]) + self.pos[1]*math.cos(wheeledbase_pos[2]) \
            + math.sin(self.angle+wheeledbase_pos[2])*dist

        # zone pente
        if x < Sensor.HILL_ZONE[0][0] or x > Sensor.HILL_ZONE[0][1] or \
           y < Sensor.HILL_ZONE[1][0] or y > Sensor.HILL_ZONE[1][1]:
            return False

        # zone balance
        if Sensor.SCALE_ZONE[0][0] < x < Sensor.SCALE_ZONE[0][1] and \
                Sensor.SCALE_ZONE[1][0] < y < Sensor.SCALE_ZONE[1][1]:
            return False

        if Sensor.START_ZONE[0][0] < x < Sensor.START_ZONE[0][1] and \
           Sensor.START_ZONE[1][0] < y < Sensor.START_ZONE[1][1]:
            return False

        return True


class SensorsManager(Thread):
    SENSORS_FREQ = 0.2
    def __init__(self, wheeledbase, sensors_front, sensors_back, sensors_lat, threshold=300):
        Thread.__init__(self)
        self.daemon         = False

        wheeledbase_pos    = wheeledbase
        self.max_linvel     = wheeledbase_pos.max_linvel.get()
        self.max_angvel     = wheeledbase_pos.max_angvel.get()

        self.sensors_front = sensors_front
        self.sensors_back  = sensors_back
        self.sensors_lat   = sensors_lat

        self.front_disable  = Event()
        self.back_disable   = Event()

        self.threshold      = threshold
        self.lock           = Lock()

        self.stopped = False
        self.stop_thread = Event()

    def run(self):
        while not self.stop_thread.is_set():
            begin = time.time()
            obstacle = False
            wheeledbase_pos = self.wheeledbase.get_position()
            if wheeledbase_pos.direction == wheeledbase_pos.FORWARD:
                for sensor in self.sensors_front:
                    if sensor.enabled and sensor.obstacle(wheeledbase_pos):
                        obstacle = True

            if wheeledbase_pos.direction == wheeledbase_pos.BACKWARD:
                for sensor in self.sensors_back:
                    if sensor.enabled and sensor.obstacle(wheeledbase_pos):
                        obstacle = True

            if obstacle:
                if not self.stopped:
                    self.max_linvel     = wheeledbase_pos.max_linvel.get()
                    self.max_angvel     = wheeledbase_pos.max_angvel.get()
                wheeledbase_pos.max_linvel.set(0)
                wheeledbase_pos.max_angvel.set(0)
                self.stopped = True

            else:
                wheeledbase_pos.max_linvel.set(self.max_linvel)
                wheeledbase_pos.max_angvel.set(self.max_angvel)
                self.stopped = False

            time.sleep(max(self.SENSORS_FREQ - (time.time() - begin), 0))

        wheeledbase_pos.max_linvel.set(self.max_linvel)
        wheeledbase_pos.max_angvel.set(self.max_angvel)

    def set_thresold(self, thresold):
        self.lock.acquire()
        self.threshold = thresold
        self.lock.release()

    def stop(self):
        self.stop_thread.set()


