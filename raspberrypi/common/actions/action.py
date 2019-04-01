#!/usr/bin/python3
#-*- coding: utf-8 -*-

from threading import Event, Thread
from collections import namedtuple
from common.funcutils   import Job
import time

ActPoint = namedtuple('ActPoint', ['point', 'theta'])

class Action():
    def __init__(self, actionPoint, actionFunc, name, actionPrep, actionComp):
        self.actionPoint    = actionPoint

        self.actionFunc     = actionFunc
        self.actionPrep     = actionPrep
        self.actionComp     = actionComp

        self.prepJob        = None
        self.compJob        = None

        self.name           = name
        self.done           = Event()

    def __call__(self):
        self.actionFunc()
    
    def __bool__(self):
        return self.done.is_set()

    def realize(self):
        self()

    def prepare(self):
        self.prepJob    = Job(self.actionPrep)
        self.prepJob.stopped.clear()
        self.prepJob.start()

    def complete(self):
        self.compJob    = Job(self.actionComp)
        self.compJob.stopped.clear()
        self.compJob.start()

    def prepareEnd(self):
        return self.prepJob

    def completeEnd(self):
        return self.compJob

    def getActionPoint(self):
        return self.actionPoint

class Actionnable():
    def getAction(self):
        raise NotImplementedError("Need implementation")

