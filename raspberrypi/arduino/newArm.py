import time
import math

from common.serialtalks import BYTE, INT, LONG, FLOAT, SerialTalks
from common.serialutils import Deserializer

_GOTO_DIRECTLY_OPCODE = 0X11
_GOTO_PATH_OPCODE     = 0X12
_IS_ARRIVED_OPCODE    = 0X13
_GOTO_HOME_OPCODE     = 0X14

class arm(SerialTalks):

    def __init__(self, uuid='/dev/tty.SLAB_USBtoUART'):
        SerialTalks.__init__(self, uuid)

    def goto_directly(self, x ,y, phi):
        output = self.execute(_GOTO_DIRECTLY_OPCODE, FLOAT(x), FLOAT(y), FLOAT(phi))
        trajectory_time = output.read(FLOAT)
        return trajectory_time

    def goto_path(self, x ,y, phi):
        output = self.execute(_GOTO_PATH_OPCODE, FLOAT(x), FLOAT(y), FLOAT(phi))
        trajectory_time = output.read(FLOAT)
        return trajectory_time

    def is_arrived(self):
        output = self.execute(_IS_ARRIVED_OPCODE)
        ret = output.read(INT)
        return ret
        
    def goto_home(self):
        output = self.execute(_GOTO_HOME_OPCODE)
        trajectory_time = output.read(FLOAT)
        return trajectory_time
