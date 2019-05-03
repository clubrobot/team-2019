from common.tcptalks import *
from time import sleep
from threading import Thread, Event, RLock
from common.logger import Logger

FRIEND_PORT = 11021

FM_LOCK_BALANCE_OPCODE          = 0x12
FM_UNLOCK_BALANCE_OPCODE        = 0x13

class FriendManager(TCPTalks,Thread):
    def __init__(self, timestep, logger, ip):

        TCPTalks.__init__(self,ip=ip,port=FRIEND_PORT)
        # bind Action
        self.bind(FM_LOCK_BALANCE_OPCODE, self._lock_balance_friend)
        self.bind(FM_UNLOCK_BALANCE_OPCODE, self._unlock_balance_friend)

        Thread.__init__(self)
        self.daemon         = True
        self.stopped        = Event()

        self.stopped.clear()
        Thread(target=self.try_connect, daemon=True).start()

        # Intern arguments
        self.timestep       = timestep
        self.logger         = logger

        # Lock event
        self.Balance        = Event()
        self.BalanceFriend  = Event()

        self.Balance.clear()
        self.BalanceFriend.clear()
    
    def try_connect(self):
        connected = False
        while not connected:
            try:
                self.connect(timeout=5)
                self.logger("FRIEND MANAGER : ", "Connected")
                connected = True
            except Exception as e:
                self.logger("FRIEND MANAGER : ", "Not connected" + str(e))
                pass
    
    def run(self):
        while not self.stopped.is_set():
            sleep(self.timestep)

    def get_balance_state(self):
        if self.Balance.is_set():
            self.logger("FRIEND MANAGER : ", "Balance is lock by me")
            return False
        elif self.BalanceFriend.is_set():
            self.logger("FRIEND MANAGER : ", "Balance is lock by my friend")
            return False
        else:
            self.logger("FRIEND MANAGER : ", "Balance Free")
            return True

    def lock_balance(self):
        if self.BalanceFriend.is_set():
            self.logger("FRIEND MANAGER : ", "[ERROR]Balance already Take by my friend")
            return False
        else:
            self.logger("FRIEND MANAGER : ", "[TX]Lock Balance")
            self.Balance.set()
            return self.execute(FM_LOCK_BALANCE_OPCODE)

    def unlock_balance(self):
        if self.BalanceFriend.is_set():
            self.logger("FRIEND MANAGER : ", "[ERROR]Balance already Take by my friend")
            return False
        else:
            self.Balance.clear()
            self.logger("FRIEND MANAGER : ", "[TX]Unlock Balance")
            return self.execute(FM_UNLOCK_BALANCE_OPCODE)
            

    def _lock_balance_friend(self):
        self.logger("FRIEND MANAGER : ", "[RX]My friend Lock balance")
        self.BalanceFriend.set()
        return True


    def _unlock_balance_friend(self):
        self.logger("FRIEND MANAGER : ", "[RX]My friend Unlock balance")
        self.BalanceFriend.clear()
        return True

if __name__ == '__main__':
    logger = Logger(Logger.SHOW)
    if(len(sys.argv) > 1):
        f = FriendManager(0.5, logger, str(sys.argv[1]))
    else:
        f = FriendManager(0.5, logger, None)
    print("Launch fake friend")
    while not f.is_connected:
        sleep(0.1)