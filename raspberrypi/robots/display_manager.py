#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
from threading import Thread, RLock

MATCH_DURATION = 100
class DisplayPoints:
    def __init__(self, display ,right_eye, left_eye):
        self.display = display
        self.left_eye = left_eye
        self.right_eye = right_eye
        self.points = 0
        self.locker = RLock()
        self.eyes_locker = RLock()
        self.left_eye.set_message("^^^^^%^",1,speed=100)
        self.right_eye.set_message('_____&_',1,speed=100)

    def start(self):
        self.start_time = time.time()
        Thread(target=self.run).start()

    
    def addPoints(self, points):
        self.locker.acquire()
        self.points += points
        self.locker.release()

    def removePoints(self, points):
        self.locker.acquire()
        self.points -= points
        self.locker.release()

    def normal(self, **kwargs):
        if not self.eyes_locker.acquire(blocking=False):
            return
        self.left_eye.set_message("^^^^^%^",1)
        self.right_eye.set_message('_____&_',1)
        self.eyes_locker.release()

    def _reset_normal(self, duration):
        def target():
            time.sleep(duration)
            self.normal()

        Thread(target=target, daemon=True).start()

    def happy(self, duration=1):
        self.left_eye.set_message("|",1)
        self.right_eye.set_message("{",1)
        self._reset_normal(duration)

    def sleep(self, duration=1):
        self.left_eye.set_message("%",1)
        self.right_eye.set_message("&",1)
        self._reset_normal(duration)

    def love(self, duration=1):
        self.left_eye.set_message("[\\]\\",1,400)
        self.right_eye.set_message("[\\]\\",1,400)
        self._reset_normal(duration)

    def angry(self, duration=1):
        self.left_eye.set_message("}", 1)
        self.right_eye.set_message("~", 1)
        self._reset_normal(duration)

    def surprised(self, duration=1):
        self.left_eye.set_message("`", 1)
        self.right_eye.set_message("`", 1)
        self._reset_normal(duration)

    def sick(self, duration=1):
        self.left_eye.set_message("@", 2, 200)
        self.right_eye.set_message("@", 3, 200)
        self._reset_normal(duration)

    def updateDisplay(self):
        remaining_time = MATCH_DURATION-2-round(time.time()- self.start_time)
        if remaining_time > 0:
            try:
                self.display.set_message("T:"+str(remaining_time)+ "  P:" + str(self.points))
            except ValueError:
                self.display.set_message("P:" + str(self.points))
        else:
            self.display.set_message("P:" + str(self.points))

    def run(self):
        while True:
            self.locker.acquire()
            self.updateDisplay()
            self.locker.release()
            time.sleep(0.5)
