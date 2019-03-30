#!/usr/bin/python3
#-*- coding: utf-8 -*-

from threading import Event, Thread
import time

class Action():
    def __init__(self, actionPoint, actionFunc, name):
        self.actionPoint    = actionPoint
        self.actionFunc     = actionFunc
        self.name           = name
        self.done           = Event()

    def __call__(self):
        self.actionFunc()
    
    def __bool__(self):
        return self.done.is_set()

    def realize(self):
        self()

    def getActionPoint(self):
        return self.actionPoint

class Actionnable():
    def getAction(self):
        raise NotImplementedError("Need implementation")

