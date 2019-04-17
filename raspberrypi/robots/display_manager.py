#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
from threading import Thread, RLock, Event

MATCH_DURATION = 100


class DisplayPoints:
    def __init__(self, display ,right_eye, left_eye):
        self.display = display
        self.left_eye = left_eye
        self.right_eye = right_eye
        self.points = 0
        self.locker = RLock()
        self.eyes_locker = RLock()
        self.normal()
        self.display_stop = Event()

    def start(self):
        self.start_time = time.time()
        self.display_stop.clear()
        self.display_thread = Thread(target=self.run)
        self.display_thread.start()


    def stop(self):
        self.display_stop.set()

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
        self.left_eye.set_message("^^^^^%^",1, speed=200)
        self.right_eye.set_message('_____&_',1, speed=200)
        self.eyes_locker.release()

    def _reset_normal(self, duration):
        def target():
            time.sleep(duration)
            self.normal()

        Thread(target=target, daemon=True).start()

    def happy(self, duration=2):
        self.left_eye.set_message("|",1)
        self.right_eye.set_message("{",1)
        self._reset_normal(duration)

    def sleep(self, duration=2):
        self.left_eye.set_message("%",1)
        self.right_eye.set_message("&",1)
        self._reset_normal(duration)

    def love(self, duration=2):
        self.left_eye.set_message("[\\]\\",1,400)
        self.right_eye.set_message("[\\]\\",1,400)
        self._reset_normal(duration)

    def angry(self, duration=2):
        self.left_eye.set_message("}", 1)
        self.right_eye.set_message("~", 1)
        self._reset_normal(duration)

    def surprised(self, duration=2):
        self.left_eye.set_message("`", 1)
        self.right_eye.set_message("`", 1)
        self._reset_normal(duration)

    def sick(self, duration=2):
        self.left_eye.set_message("@", 2, 200)
        self.right_eye.set_message("@", 3, 200)
        self._reset_normal(duration)

    def updateDisplay(self):
        self.display.clear_messages()
        remaining_time = MATCH_DURATION-round(time.time() - self.start_time)
        if remaining_time > 0:
            try:
                self.display.set_message("T:"+str(remaining_time)+ "  P:" + str(self.points))
            except ValueError:
                self.display.set_message("P:" + str(self.points))
        else:
            self.display.set_message("P:" + str(self.points))

    def run(self):
        while not self.display_stop.is_set():
            self.locker.acquire()
            self.updateDisplay()
            self.locker.release()
            time.sleep(0.5)
        MATCH_DURATION = 0
        self.locker.acquire()
        self.updateDisplay()
        self.locker.release()

