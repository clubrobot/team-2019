#!/usr/bin/python3
#-*- coding: utf-8 -*-
from threading import Thread, Event, RLock
from queue import Queue
from collections import namedtuple
import time

ActPoint = namedtuple('ActPoint', ['point', 'theta'])

class Action():
    def __init__(self, movingAct, actionFunc, before, after, name):
        self.movingAct      = movingAct
        self.actionFunc     = actionFunc
        self.before         = before
        self.after          = after
        self.name           = name

        self.done           = Event()
        self.done.clear()

    def moving(self):
        self.movingAct()

    def __call__(self):
        self.actionFunc()

    def __bool__(self):
        return self.done.is_set()

    def realize(self):
        self()

    def getBefore(self):
        return self.before

    def getAfter(self):
        return self.after

class Actionnable():
    def moving(self):
        pass

    def realize(self):
        pass

    def before(self):
        pass

    def after(self):
        pass

    def getAction(self):
        raise NotImplementedError("Need implementation")

class ThreadActionManager(Thread):
    def __init__(self):
        Thread.__init__(self)
        self.daemon         = False
        self.stopped        = Event()
        self.ActionEnd      = Event()
        self.queueFunc      = Queue()

    def putAction(self, func):
        self.queueFunc.put(func)
        self.ActionEnd.clear()
        
    def stop(self):
        while not self.end():
            time.sleep(0.1)
        self.stopped.set()
        self.join()

    def run(self):
        while not self.stopped.is_set():
            while not self.queueFunc.empty():
                self.ActionEnd.clear()
                # execute the current action in Queue
                self.queueFunc.get()()

            self.ActionEnd.set()
    
    def end(self):
        if self.ActionEnd.is_set() and self.queueFunc.empty():
            self.ActionEnd.clear()
            return True
        else:
            return False