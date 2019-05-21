#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
from threading import Thread, Event, Lock
import math


class Sensor:
    HILL_ZONE = ((300, 1400), (200, 2800))
    SCALE_ZONE = ((1150, 1600), (1200, 1800))
    DIST_MIN = 300

    def __init__(self, name, dist, pos, angle):
        self.dist = dist
        self.pos = pos
        self.angle = angle
        self.enabled = True
        self.name = name

    def disable(self):
        self.enabled = False

    def obstacle(self, wheeledbase):
        dist = self.dist()
        if dist > Sensor.DIST_MIN:
            return False

        x = wheeledbase[0] + self.pos[0]*math.cos(wheeledbase[2]) - self.pos[1]*math.sin(wheeledbase[2]) \
            + math.cos(self.angle+wheeledbase[2])*dist
        y = wheeledbase[1] + self.pos[0]*math.sin(wheeledbase[2]) + self.pos[1]*math.cos(wheeledbase[2]) \
            + math.sin(self.angle+wheeledbase[2])*dist

        # zone pente
        if x < Sensor.HILL_ZONE[0][0] or x > Sensor.HILL_ZONE[0][1] or \
           y < Sensor.HILL_ZONE[1][0] or y > Sensor.HILL_ZONE[1][1]:
            return False

        # zone balance
        if Sensor.SCALE_ZONE[0][0] < x < Sensor.SCALE_ZONE[0][1] and \
           Sensor.SCALE_ZONE[1][0] < y < Sensor.SCALE_ZONE[1][1]:
            return False

        print("obstacle en : ", round(x), round(y))
        print("wheeledbase : ", wheeledbase)
        print("capteur", self.name)
        print("distance : ", dist)
        print()
        return True


class SensorsManager(Thread):
    SENSORS_FREQ = 0.2
    def __init__(self, wheeledbase, sensors_front, sensors_back, sensors_lat, threshold=300):
        Thread.__init__(self)
        self.daemon         = False

        self.wheeledbase    = wheeledbase
        self.max_linvel     = self.wheeledbase.max_linvel.get()
        self.max_angvel     = self.wheeledbase.max_angvel.get()

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
            if self.wheeledbase.direction == self.wheeledbase.FORWARD:
                for sensor in self.sensors_front:
                    if sensor.enabled and sensor.obstacle(wheeledbase_pos):
                        obstacle = True

            if self.wheeledbase.direction == self.wheeledbase.BACKWARD:
                for sensor in self.sensors_back:
                    if sensor.enabled and sensor.obstacle(wheeledbase_pos):
                        obstacle = True

            if obstacle:
                if not self.stopped:
                    self.max_linvel     = self.wheeledbase.max_linvel.get()
                    self.max_angvel     = self.wheeledbase.max_angvel.get()
                self.wheeledbase.max_linvel.set(0)
                self.wheeledbase.max_angvel.set(0)
                self.stopped = True

            else:
                self.wheeledbase.max_linvel.set(self.max_linvel)
                self.wheeledbase.max_angvel.set(self.max_angvel)
                self.stopped = False

            time.sleep(max(self.SENSORS_FREQ - (time.time() - begin), 0))

        self.wheeledbase.max_linvel.set(self.max_linvel)
        self.wheeledbase.max_angvel.set(self.max_angvel)

    def disable_front(self):
        self.front_disable.set()

    def disable_back(self):
        self.back_disable.set()

    def enable_front(self):
        self.front_disable.clear()

    def enable_back(self):
        self.back_disable.clear()

    def set_thresold(self, thresold):
        self.lock.acquire()
        self.threshold = thresold
        self.lock.release()

    def stop(self):
        self.stop_thread.set()


