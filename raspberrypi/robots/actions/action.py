#!/usr/bin/env python3
# coding: utf-8

from threading import Event, Thread
import time

class ActionExecutionError(RuntimeError):
    pass

class Action:
    def __init__(self, actionPoint, actionFunc):
        self.actionPoint         = actionPoint
        self.actionFunc          = actionFunc
        self.onTheRoadAction     = lambda: None
        self.beforeAction        = lambda: None
        self.done                = Event()

    def set_onTheRoadAction(self, action):
        self.onTheRoadAction = action

    def set_beforeAction(self, action):
        self.beforeAction = action

    def __call__(self):
        self.actionFunc()
    
    def realize(self):
        self()

    def __bool__(self):
        return self.done.is_set()

class Actionnable():
    def getAction(self):
        raise NotImplementedError("Need implementation")

