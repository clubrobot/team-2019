#!/usr/bin/env python3
# coding: utf-8

from threading import Thread, Event
from time import sleep
from math import hypot

from common.sync_flag_signal import Signal


class PositionListener(Thread):
    def __init__(self, getter, timestep=0.1, threshold=10):
        Thread.__init__(self)
        self.signal = Signal()
        self.getter = getter
        self.timestep = timestep
        self.stop = Event()
        self.threshold = threshold
        self.error = 0
        self.position = (-1000, -1000)  # self.getter()
        self.start()

    def run(self):
        while not self.stop.is_set():
            sleep(self.timestep)
            x, y = self.getter()

            if (hypot(y - self.position[1], x - self.position[0]) + self.error) > self.threshold:
                self.signal.ping()

                self.error = 0
            else:
                self.error += hypot(y - self.position[1], x - self.position[0])
            self.position = (x, y)
