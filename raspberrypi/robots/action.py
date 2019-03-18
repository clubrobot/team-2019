#!/usr/bin/env python3
# coding: utf-8

from threading import Event, Thread
import time

class Action():
    def __init__(self,actionPoint,actionFunction,typ, name, points, time):
        self.actionPoint=actionPoint
        self.actionFunction=actionFunction
        self.typ=typ
        self.predecessors = []
        self.done = Event()
        self.done.clear()
        self.name = name
        self.points = points
        self.combinations = []
        self.reliability = 1
        self.time = time
        self.area = None
        self.manual_order = 0
        self.before_action = lambda: None
        
    def __call__(self):
        self.actionFunction()

    def realize(self):
        self()

    def set_predecessors(self, predecessors):
        self.predecessors = predecessors

    def check_impossible_combinations(self):
        for combi in self.combinations:
            if combi():
                return False
        return True

    def set_impossible_combination(self, lambd):
        self.combinations += [lambd]

    def set_manual_order(self, order):
        self.manual_order = order

    def set_reliability(self, rel):
        self.reliability = rel

    def link_area(self, aera_name):
        self.area = aera_name

    def __bool__(self):
        return self.done.is_set()

    def set_before_action(self, action):
        self.before_action = action

    def temp_disable(self, timeout):
        Thread(target=self._temp_disable(timeout), daemon=True).start()

    def _temp_disable(self, timeout):
        self.done.set()
        time.sleep(timeout)
        self.done.clear()


class MultipleAction:
    def __init__(self, actions):
        self.actions = actions
        for action in self.actions:
            for action2 in self.actions:
                if action == action2:
                    break
                action.set_impossible_combination(action2)

    def set_predecessors(self, predecessors):
        for action in self.actions:
            action.set_predecessors(predecessors)

    def get_actions(self):
        return self.actions

    def __bool__(self):
        for action in self.actions:
            if action:
                return True
        return False

class Actionnable():
    def getAction(self):
        raise NotImplementedError("Need implementation")

