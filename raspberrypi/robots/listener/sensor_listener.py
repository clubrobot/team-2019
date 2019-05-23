#!/usr/bin/env python3
# coding: utf-8


from threading        import Thread, Event
from common.sync_flag_signal import Signal
from time import sleep
from math import hypot

class SensorListener(Thread):
    def __init__(self, getter, timestep = 0.1, threshold = 220):
        Thread.__init__(self)
        self.daemon = True
        self.signal   = Signal()
        self.getter   = getter
        self.timestep = timestep
        self.stop  = Event()
        self.threshold = threshold
        self.error    = 0
        self.start()

    def run(self):
        while not self.stop.is_set():
            sleep(self.timestep)
            try:
                left , right = self.getter()
            except TimeoutError:
                continue
            if left<self.threshold or right<self.threshold :
                self.signal.ping()



