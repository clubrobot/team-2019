#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import pygame
import time
from pygame.locals import *
from robots.setup_wheeledbase import *
from robots.display_manager import *

# l.set_motor_velocity(0)
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)


class TextPrint:
    def __init__(self):
        self.reset()
        self.font = pygame.font.Font(None, 20)

    def print(self, screen, textString):
        textBitmap = self.font.render(textString, True, BLACK)
        screen.blit(textBitmap, [self.x, self.y])
        self.y += self.line_height

    def reset(self):
        self.x = 10
        self.y = 10
        self.line_height = 15

    def indent(self):
        self.x += 10

    def unindent(self):
        self.x -= 10


# Constants

linvel = 850
angvel = 3
linearVelocityActual = 0
angularVelocityActual = 0

launcherPower = 0

# set of key currently pressed,
# the functions which can't be call at the same time will be in the same list
# can be generated by a function with CategoryFunction
functionCurrentlyPressed = {
    "wheeledbase": {
        "move": list(),
        "rotate": list()
    }
}


# def main functions
def addFunctionToStack(fct):
    cat, underCat = CategoryFunction[fct]
    functionCurrentlyPressed[cat][underCat].append(fct)


def avancer(coef=1):
    global linearVelocityActual
    global angularVelocityActual
    addFunctionToStack(avancer)
    l = coef * linvel
    if (linearVelocityActual * l < 0):
        angularVelocityActual *= -1
    wheeledbase.set_velocities(l, angularVelocityActual)
    linearVelocityActual = l


def reculer(coef=1):
    global linearVelocityActual
    global angularVelocityActual
    addFunctionToStack(reculer)
    l = coef * (-linvel)
    if (linearVelocityActual * l < 0):
        angularVelocityActual *= -1
    wheeledbase.set_velocities(l, angularVelocityActual)
    linearVelocityActual = l


def gauche(coef=1):
    global linearVelocityActual
    global angularVelocityActual
    addFunctionToStack(gauche)
    a = coef * angvel * math.copysign(1, linearVelocityActual)
    wheeledbase.set_velocities(linearVelocityActual, a)
    angularVelocityActual = a


def droite(coef=1):
    global linearVelocityActual
    global angularVelocityActual
    addFunctionToStack(droite)
    a = coef * (-angvel) * math.copysign(1, linearVelocityActual)
    wheeledbase.set_velocities(linearVelocityActual, a)
    angularVelocityActual = a


def stop():
    wheeledbase.stop()


def stopMove():
    global linearVelocityActual
    global angularVelocityActual
    wheeledbase.set_velocities(0, angularVelocityActual)
    linearVelocityActual = 0


def stopRotate():
    global linearVelocityActual
    global angularVelocityActual
    wheeledbase.set_velocities(linearVelocityActual, 0)
    angularVelocityActual = 0


def stopCdt(fct):
    cat, underCat = CategoryFunction[fct]
    if len(functionCurrentlyPressed[cat][underCat]) > 0:
        functionCurrentlyPressed[cat][underCat].pop()()  # on recupere la derniere fonction, qu on appelle
    else:
        if (cat, underCat) in whatToDoToStopInCategory:
            whatToDoToStopInCategory[(cat, underCat)]()


def removeFromListAndEventuallyStop(fct):
    cat, underCat = CategoryFunction[fct]
    functionCurrentlyPressed[cat][underCat].remove(fct)
    stopCdt(fct)

# define the category of each method,
# two method in the same category can't be executed at the same time
CategoryFunction = {
    avancer: ("wheeledbase", "move"),
    reculer: ("wheeledbase", "move"),
    gauche: ("wheeledbase", "rotate"),
    droite: ("wheeledbase", "rotate"),
}

# which function call when there is no event to do for each category
whatToDoToStopInCategory = {
    ("wheeledbase", "move"): stopMove,
    ("wheeledbase", "rotate"): stopRotate
}

# define controls
controller_keys = {
    K_q: {KEYDOWN: stop, KEYUP: stop},
    K_UP: {KEYDOWN: avancer, KEYUP: lambda: removeFromListAndEventuallyStop(avancer)},
    K_DOWN: {KEYDOWN: reculer, KEYUP: lambda: removeFromListAndEventuallyStop(reculer)},
    K_LEFT: {KEYDOWN: gauche, KEYUP: lambda: removeFromListAndEventuallyStop(gauche)},
    K_RIGHT: {KEYDOWN: droite, KEYUP: lambda: removeFromListAndEventuallyStop(droite)},
}
pygame.joystick.init()
joystick_count = pygame.joystick.get_count()
print("Joystick count : ", joystick_count)
if (joystick_count > 1):
    print("Multiples Joystick")
elif (joystick_count == 1):
    joystick = pygame.joystick.Joystick(0)
    joystick.init()

axis_tol = 0.2
rotate_coeff = 0.5
controller_joyaxis = {
    1: lambda val: avancer(-val) if val < -axis_tol else (reculer(val) if val > axis_tol else stopMove()),
    0: lambda val: droite(val*rotate_coeff) if val > axis_tol else (gauche(-val*rotate_coeff) if val < -axis_tol else stopRotate()),
    2: lambda val: avancer((val+1)/2) if (val+1) > axis_tol else stopMove(),
    5: lambda val: reculer((val+1)/2) if (val+1) > axis_tol else stopMove()
}

controller_joybuttons = {}

# Setup and launch the user interface

pygame.init()
window = pygame.display.set_mode((640, 480), RESIZABLE)
printer = TextPrint()
text = """Demo wheeledbase"""
window.fill(WHITE)
printer.reset()

[printer.print(window, t) for t in text.split("\n")]
pygame.display.flip()


def controlEvent():
    while True:
        event = pygame.event.wait()
        if event.type == QUIT:
            break
        if event.type == KEYDOWN or event.type == KEYUP:
            if event.key in controller_keys and event.type in controller_keys[event.key]:
                controller_keys[event.key][event.type]()
        if event.type == JOYAXISMOTION:
            if event.axis in controller_joyaxis.keys():
                controller_joyaxis[event.axis](event.value)
        if event.type == JOYBUTTONDOWN or event.type == JOYBUTTONUP:
            if event.key in controller_joybuttons and event.type in controller_joybuttons[event.key]:
                controller_keys[event.key][event.type]()


controlEvent()

wheeledbase.stop()
