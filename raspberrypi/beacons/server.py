#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from threading import Thread, Event
from time import sleep
import sys

from common.tcptalks import TCPTalksServer
from beacons.panel import *
from beacons.anchor import *
from common.gpiodevices import *

PORT_BALISE = 26657
GET_POSITION_OPCODE = 0x14
SET_COLOR_OPCODE = 0x15
ISONTOP_OPCODE = 0x17
START_EXPERIENCE_OPCODE = 0x19
BIG_ROBOT = 0
LITTLE_ROBOT = 1

class Server(TCPTalksServer):
    def __init__(self):
        TCPTalksServer.__init__(self, PORT_BALISE)
        self.bind(GET_POSITION_OPCODE, self.getPosition)
        self.bind(SET_COLOR_OPCODE, self.setSide)
        self.bind(ISONTOP_OPCODE, self.isElectronOnTop)
        self.bind(START_EXPERIENCE_OPCODE, self.startElectron)
        self.beacon = Anchor()
        self.expServ = Electron()
        self.beacon.connect()
        self.expServ.connect()
        Switch(5, self.changeChannel)

    def run(self):
        while True:
            try:
                while not self.full():
                    self.connect(timeout=100)
                self.sleep_until_one_disconnected()

            except KeyboardInterrupt:
                break
            except Exception as e:
                sys.stderr.write('{}: {}\n'.format(type(e).__name__, e))
                continue

    def setSide(self, side):
        try:
            self.beacon.update_color(side)
        except:
            pass

    def getPosition(self, id):
        try:
            return self.beacon.get_position(id)
        except:
            return (-1000, -1000)

        return (-1000, -1000)

    def isElectronOnTop(self):
        return bool(self.expServ.isOnTop())

    def startElectron(self):
        self.expServ.start()

    def changeChannel(self):
        try:
            self.beacon.change_channel()
        except:
            pass
