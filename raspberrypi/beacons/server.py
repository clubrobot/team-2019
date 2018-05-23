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
GET_PANEL_STATUS_OPCODE = 0x17
BIG_ROBOT = 0
LITTLE_ROBOT = 1

class Server(TCPTalksServer):
    def __init__(self):
        TCPTalksServer.__init__(self, PORT_BALISE)
        self.bind(GET_POSITION_OPCODE, self.getPosition)
        self.bind(SET_COLOR_OPCODE, self.setSide)
        self.bind(GET_PANEL_STATUS_OPCODE, self.get_panel_status)
        self.beacon = Anchor()
        self.beacon.connect()
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

    def get_panel_status(self):
        return bool(self.beacon.is_panel_on())

    def changeChannel(self):
        try:
            self.beacon.change_channel()
        except:
            pass