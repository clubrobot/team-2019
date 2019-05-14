from common.tcptalks import *
from time import sleep
from threading import Thread, Event, RLock

FRIEND_PORT = 11021

LOCK_BALANCE_OPCODE          = 0x12
UNLOCK_BALANCE_OPCODE        = 0x13
GET_BALANCE_OPCODE           = 0x14

class ClientManager(TCPTalks,Thread):
    def __init__(self, name, timestep, logger, ip="192.168.12.1"):

        self.clientname = name
        #Init TCPTalks as client with server IP
        TCPTalks.__init__(self,ip,FRIEND_PORT)

        Thread.__init__(self)
        self.daemon         = True
        self.stopped        = Event()

        self.stopped.clear()
        Thread(target=self.try_connect, daemon=True).start()

        # Intern arguments
        self.timestep       = timestep
        self.logger         = logger
    
    def try_connect(self):
        connected = False
        while not connected:
            try:
                self.connect(timeout=5)
                self.logger("CLIENT MANAGER[{}] : ".format(self.clientname), "Connected")
                connected = True
            except Exception as e:
                self.logger("CLIENT MANAGER[{}] : ".format(self.clientname), "Not connected" + str(e))
                pass
    
    def run(self):
        while not self.stopped.is_set():
            sleep(self.timestep)

    def lock_balance(self):
        self.logger("CLIENT MANAGER[{}] : ".format(self.clientname), "Lock Balance")
        return self.execute(LOCK_BALANCE_OPCODE, self.clientname)

    def unlock_balance(self):
        self.logger("CLIENT MANAGER[{}] : ".format(self.clientname), "UnLock Balance")
        return self.execute(UNLOCK_BALANCE_OPCODE, self.clientname)

    def get_balance_state(self):
        self.logger("CLIENT MANAGER[{}] : ".format(self.clientname), "Get Balance state")
        return self.execute(GET_BALANCE_OPCODE, self.clientname)

if __name__ == "__main__":
    from common.logger import *
    import sys
    log = Logger(Logger.SHOW)

    robot    = ClientManager(str(sys.argv[1]), 0.5, log, "127.0.0.1")

    print("Launch {} client".format(str(sys.argv[1])))
    while not robot.is_connected:
        sleep(0.1)