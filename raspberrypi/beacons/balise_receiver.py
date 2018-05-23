#!/usr/bin/env python3
# coding: utf-8

from common.tcptalks import *
from threading import Thread

BIG_ROBOT = 0
LITTLE_ROBOT = 1

GET_POSITION_OPCODE = 0x14
SET_COLOR_OPCODE = 0x15
GET_PANEL_STATUS_OPCODE = 0x17
PORT_BALISE = 26657


class BaliseReceiver(TCPTalks):
    def __init__(self, ip=None, port=PORT_BALISE, password=None):
        TCPTalks.__init__(self, ip=ip, port=port)

    def get_position(self, robotID):
        if not  self.is_connected : return (-1000,-1000)
        try:
            output = self.execute(GET_POSITION_OPCODE, robotID)
        except:
            return (-1000, -1000)
        return output

    def get_panel_status(self):
        try:
            result =  self.execute(GET_PANEL_STATUS_OPCODE)
        except:
            return True
        return result

    def launch(self):
        Thread(target=self.try_connect, daemon=True).start()

    def set_color(self, color):
        Thread(target=self._set_color(color), daemon=True).start()

    def _set_color(self, color):
        while not self.is_connected:
            time.sleep(1)
        self.send(SET_COLOR_OPCODE, color)

    def try_connect(self):
        connected = False
        while not connected:
            try:
                self.connect(timeout=5)
                connected = True
            except Exception as e:
                pass
