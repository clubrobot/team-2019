#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
from daughter_cards.Sensor_IR import *
from threading import Thread, Event, Lock

class SensorsManager(Thread):
    def __init__(self, wheeledbase, sensor_front, sensor_back):
        Thread.__init__(self)
        self.daemon         = False


        self.wheeledbase    = wheeledbase
        self.max_linvel     = self.wheeledbase.max_linvel.get()
        self.max_angvel     = self.wheeledbase.max_angvel.get()

        self.sensor_front   = sensor_front
        self.sensor_back    = sensor_back

        self.front_disable  = Event()
        self.back_disable   = Event()

        self.threshold      = 300 #mm
        self.lock           = Lock()

        self.stopped = False

    def run(self):
        while True:
            obstacle = False
            self.lock.acquire()
            if not self.front_disable.is_set():
                if(self.sensor_front.get_single_mesure()[0] < self.threshold):
                    obstacle = True

            if not self.back_disable.is_set():
                if(self.sensor_front.get_single_mesure()[0] < self.threshold):
                    obstacle = True
            self.lock.release()

            if obstacle:
                if not self.stopped:
                    print("nouvel obstacle")
                    self.max_linvel     = self.wheeledbase.max_linvel.get()
                    self.max_angvel     = self.wheeledbase.max_angvel.get()
                self.wheeledbase.max_linvel.set(0)
                self.wheeledbase.max_angvel.set(0)
                self.stopped = True

            else:
                self.wheeledbase.max_linvel.set(self.max_linvel)
                self.wheeledbase.max_angvel.set(self.max_angvel)
                self.stopped = False
            time.sleep(0.1)

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


