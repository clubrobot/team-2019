#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
from threading import Thread, Event, Lock
import math
from common.sync_flag_signal import Signal


class SensorListener(Thread):
    def __init__(self, sensors, timestep = 0.1, threshold = 220):
        Thread.__init__(self)
        self.daemon = True
        self.signal   = Signal()
        self.getter   = sensors.obstacle
        self.timestep = timestep
        self.stop  = Event()
        self.threshold = threshold
        self.error    = 0
        self.start()

    def run(self):
        while not self.stop.is_set():
            time.sleep(self.timestep)
            try:
                obstacle = self.getter(self.threshold)
            except TimeoutError:
                continue
            if obstacle:
                self.signal.ping()


class Sensor:
    HILL_ZONE = ((300, 1400), (200, 2800))
    SCALE_ZONE = ((1150, 1600), (1200, 1800))

    def __init__(self, wheeledbase, name, dist, pos, angle):
        self.dist = dist
        self.pos = pos
        self.angle = angle
        self.enabled = True
        self.name = name
        self.wheeledbase = wheeledbase

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

        print("obstacle en : ", round(x), round(y))
        print("wheeledbase_pos : ", wheeledbase_pos)
        print("capteur", self.name)
        print("distance : ", dist)
        print()
        return True
