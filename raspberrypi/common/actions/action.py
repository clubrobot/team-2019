#!/usr/bin/python3
#-*- coding: utf-8 -*-
from threading import Thread, Event
from Queue import Queue
from collections import namedtuple
import time

ActPoint = namedtuple('ActPoint', ['point', 'theta'])

class Action():
    def __init__(self, actionPoint, actionFunc, name, actionPrep, actionComp):
        self.actionPoint    = actionPoint

        self.actionFunc     = actionFunc
        self.actionPrep     = actionPrep
        self.actionComp     = actionComp

        self.name           = name

    def __call__(self):
        self.actionFunc()

    def realize(self):
        self()

    def prepare(self):
        self.actionPrep()

    def complete(self):
        self.actionComp()

    def getActionPoint(self):
        return self.actionPoint

class Actionnable():
    def getAction(self):
        raise NotImplementedError("Need implementation")

class ThreadActionManager(Thread):
    def __init__(self):
        Thread.__init__(self)
        self.daemon         = False
        self.stopped        = Event()
        self.jobEnd         = Event()
        self.queueFunc      = Queue()

    def putJob(self, func):
        self.queueFunc.put(func)
        self.jobEnd.clear()
        
    def stop(self):
        self.stopped.set()
        self.join()

    def run(self):
        while not self.stopped.is_set():
            while not self.queueFunc.empty():
                self.queueFunc.get()()
            self.jobEnd.set()
    
    def end(self):
        if self.jobEnd.is_set():
            self.jobEnd.clear()
            return True
        else:
            return False

def test1():
    print('1')
    time.sleep(2)

def test2():
    print('2')

if __name__ == '__main__':
    Act = ThreadActionManager()

    Act.start()

    Act.putJob(test2)

    while not Act.end():
        time.sleep(0.1)

    Act.putJob(test1)
    Act.putJob(test1)

    while not Act.end():
        time.sleep(0.1)
    
    Act.putJob(test2)

    while not Act.end():
        time.sleep(0.1)

    Act.stop()
