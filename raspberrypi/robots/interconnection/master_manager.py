from common.tcptalks import *
from time import sleep
from threading import Thread, Event, RLock

MASTER_PORT = 11021

LOCK_BALANCE_OPCODE          = 0x12
UNLOCK_BALANCE_OPCODE        = 0x13
GET_BALANCE_OPCODE           = 0x14

class MasterManager(TCPTalks,Thread):
    def __init__(self, timestep, logger):
        #Init TCPTalks as server for robots
        TCPTalks.__init__(self,port=MASTER_PORT)

        # bind Action
        self.bind(LOCK_BALANCE_OPCODE   , self._lock_balance)
        self.bind(UNLOCK_BALANCE_OPCODE , self._unlock_balance)
        self.bind(GET_BALANCE_OPCODE    , self._get_balance)

        Thread.__init__(self)
        self.daemon         = True
        self.stopped        = Event()

        self.stopped.clear()
        Thread(target = self.try_connect, daemon = True).start()

        # Intern arguments
        self.timestep       = timestep
        self.logger         = logger

        #self.locker_name
        self.locker_name = None
        # Lock event
        self.Balance        = Event()

        self.Balance.clear()
    
    def try_connect(self):
        connected = False
        while not connected:
            try:
                self.connect(timeout=5)
                self.logger("MASTER MANAGER : ", "Connected")
                connected = True
            except Exception as e:
                self.logger("MASTER MANAGER : ", "Not connected" + str(e))
                pass
    
    def run(self):
        while not self.stopped.is_set():
            sleep(self.timestep)

    def _lock_balance(self, *args, **kwargs):
        if not self.Balance.is_set():
            self.locker_name = args[0]
            self.logger("MASTER MANAGER : ", "[{}] Lock balance".format(args[0]))
            self.Balance.set()
            return True
        else:
            self.logger("MASTER MANAGER : ", "Balance already lock by {}".format(self.locker_name))
            return False


    def _unlock_balance(self, *args, **kwargs):
        if self.Balance.is_set() and (self.locker_name == args[0]):
            self.logger("MASTER MANAGER : ", "[{}] UnLock balance".format(args[0]))
            self.Balance.clear()
            return True
        else:
            self.logger("MASTER MANAGER : ", "Cannot UnLock : Balance already lock by {}".format(self.locker_name))
            return False

    def _get_balance(self, *args, **kwargs):
        if not self.Balance.is_set():
            return True
        else:
            return False


if __name__ == "__main__":
    from common.logger import *

    log = Logger(Logger.SHOW)

    master = MasterManager(0.5, log)

    print("Launch master Manager")
    while not master.is_connected:
        sleep(0.1)