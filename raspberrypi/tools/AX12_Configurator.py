#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import time
import math

from common.serialutils import Deserializer
from common.serialtalks import BYTE, INT, LONG, FLOAT, SerialTalks


_ATTACH_OPCODE               = 0X10
_DETACH_OPCODE               = 0X11
_RESET_OPCODE                = 0X12

_PING_AX_OPCODE              = 0X13

_SET_ID_OPCODE               = 0X14
_SET_BD_OPCODE               = 0X15

_MOVE_OPCODE                 = 0X16
_MOVE_SPEED_OPCODE           = 0X17

_SET_ENDLESS_MODE_OPCODE     = 0X18

_SET_TEMP_LIMIT_OPCODE       = 0X1C
_SET_ANGLE_LIMIT_OPCODE      = 0X1D
_SET_VOLTAGE_LIMIT_OPCODE    = 0X1E
_SET_MAX_TORQUE_OPCODE       = 0X1F
_SET_MAX_TORQUE_RAM_OPCODE   = 0X20
_SET_SRL_OPCODE              = 0X21
_SET_RDT_OPCODE              = 0X22
_SET_LED_ALARM_OPCODE        = 0X23
_SET_SUTDOWN_ALARM_OPCODE    = 0X24
_SET_CMARGIN_OPCODE          = 0X25
_SET_CSLOPE_OPCODE           = 0X26
_SET_PUNCH_OPCODE            = 0X27

_READ_TEMPERATURE_OPCODE     = 0X28
_READ_VOLTAGE_OPCODE         = 0X29
_READ_POSITION_OPCODE        = 0X2A
_READ_SPEED_OPCODE           = 0X2B
_READ_TORQUE_OPCODE          = 0X2C

class AX12Configurator(SerialTalks):
    def __init__(self, uuid='/dev/tty.SLAB_USBtoUART'):
        SerialTalks.__init__(self, uuid)

    def attach(self, id):
        self.send(_ATTACH_OPCODE, BYTE(id))

    def detach(self):
        self.send(_DETACH_OPCODE, BYTE(id))

    def reset(self):
        self.send(_RESET_OPCODE)

    def ping(self):
        output = self.execute(_PING_AX_OPCODE)
        return bool(output.read(BYTE))

    def setID(self, newID):
        self.send(_SET_ID_OPCODE, BYTE(newID))

    def setBD(self, newBD):
        self.send(_SET_BD_OPCODE, INT(newBD))

    def move(self, Pos):
        self.send(_MOVE_OPCODE, FLOAT(Pos))
    
    def moveSpeed(self, Pos, Speed):
        self.send(_MOVE_SPEED_OPCODE, FLOAT(Pos), FLOAT(Speed))

    def setEndlessMode(self, Status):
        self.send(_SET_ENDLESS_MODE_OPCODE, BYTE(Status))

    def setTempLimit(self, Temp):
        self.send(_SET_TEMP_LIMIT_OPCODE, BYTE(Temp))

    def setAngleLimit(self, CWLimit, CCWLimit):
        self.send(_SET_ANGLE_LIMIT_OPCODE, FLOAT(CWLimit), FLOAT(CCWLimit))

    def setVoltageLimit(self, DVoltage, UVoltage):
        self.send(_SET_VOLTAGE_LIMIT_OPCODE, BYTE(DVoltage), BYTE(UVoltage))

    def setMaxTorque(self, MaxTorque):
        self.send(_SET_MAX_TORQUE_OPCODE, INT(MaxTorque))
    
    def setMaxTorqueRAM(self, MaxTorque):
        self.send(_SET_MAX_TORQUE_RAM_OPCODE, INT(MaxTorque))
    
    def setSRL(self, srl):
        self.send(_SET_SRL_OPCODE, BYTE(srl))
    
    def setRDT(self, rdt):
        self.send(_SET_RDT_OPCODE, BYTE(rdt))
    
    def setLEDAlarm(self, LEDAlarm):
        self.send(_SET_LED_ALARM_OPCODE, BYTE(LEDAlarm))

    def setShutdownAlarm(self, SAlarm):
        self.send(_SET_SUTDOWN_ALARM_OPCODE, BYTE(SAlarm))
    
    def setCMargin(self, CWCMargin, CCWCMargin):
        self.send(_SET_CMARGIN_OPCODE, BYTE(CWCMargin), BYTE(CCWCMargin))
    
    def setCSlope(self, CWCSlope, CCWCSlope):
        self.send(_SET_CSLOPE_OPCODE, BYTE(CWCSlope), BYTE(CCWCSlope))

    def setPunch(self, punch):
        self.send(_SET_PUNCH_OPCODE, INT(punch))
    
    def readTemperature(self):
        output = self.execute(_READ_TEMPERATURE_OPCODE)
        return output.read(INT)

    def readVoltage(self):
        output = self.execute(_READ_VOLTAGE_OPCODE)
        return output.read(FLOAT)

    def readPosition(self):
        output = self.execute(_READ_POSITION_OPCODE)
        return output.read(FLOAT)
    
    def readSpeed(self):
        output = self.execute(_READ_SPEED_OPCODE)
        return output.read(FLOAT)

    def readTorque(self):
        output = self.execute(_READ_TORQUE_OPCODE)
        return output.read(INT)


if __name__ == "__main__":
    arm = AX12Configurator()
    arm.connect()