#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from threading import Thread, Event, RLock
from time import sleep
import sys

from common.tcptalks import TCPTalksServer, TCPTalks

# La position 
# Des mutex
_BEACON_PORT = 25568
_BORNIBUS_ID = 1
_R128_ID     = 2

_GET_RESSOURCE_OPCODE = 0x11
_RELEASE_RESSOURCE_OPCODE = 0x30
_GET_POS_OPCODE = 0x10
_GET_OTHER_OPCODE = 0x20
_PING_OPCODE    = 0x40
_IS_OK_OPCODE   = 0x50

class ClientGS(TCPTalks):
    def __init__(self, ROBOT, ip="192.168.12.1",port=_BEACON_PORT):
        TCPTalks.__init__(self,ip=ip, port=port, id=ROBOT)
        self.bind(_PING_OPCODE, self._refresh)
        self.bind(_GET_POS_OPCODE, self._get_my_pos)

    def get_ressource(self, name):
        return self.execute(_GET_RESSOURCE_OPCODE, self.id, name)

    def release_ressource(self, name):
        try:
            self.send(_RELEASE_RESSOURCE_OPCODE, self.id, name)
        except:
            pass

    def is_active(self):
        if not self.is_connected:
            return False
        try:
            return self.execute(_IS_OK_OPCODE, self.id, timeout=1)
        except:
            return False

    def _refresh(self):
        return True
    
    def get_pos(self):
        other = _BORNIBUS_ID if self.id == _R128_ID else _R128_ID 
        return self.execute(_GET_OTHER_OPCODE, other)

    def _get_my_pos(self):
        return (666,666+self.id)


class ServerGS(TCPTalksServer):

    def __init__(self):
        TCPTalksServer.__init__(self, _BEACON_PORT)
        self.ressources = {"balance": -1, "gold": -1}
        self.mutex = RLock()
        self.bornibus_id = -1
        self.r128_id = -1
        self.bind(_GET_OTHER_OPCODE, self.get_pos)
        self.bind(_GET_RESSOURCE_OPCODE, self.get_ressource)
        self.bind(_RELEASE_RESSOURCE_OPCODE, self.release_ressource)
        self.bind(_IS_OK_OPCODE, self._is_ok)

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


                
    def _is_ok(self, idx):
        if _BORNIBUS_ID in list(self.client.keys()) and _R128_ID in list(self.client.keys()):
            try:
                if idx==_BORNIBUS_ID:
                    self.execute(_PING_OPCODE, id=_R128_ID)
                else:
                    self.execute(_PING_OPCODE, id=_BORNIBUS_ID)
                return True
            except:
                return False
        else:
            return False

    def get_pos(self, idx):
        if not idx in list(self.client.keys()):
            return (-1000,-1000)

        pos = self.execute(_GET_POS_OPCODE, id=idx)
        return pos

    def get_ressource(self, idx, name):
        if not self.mutex.acquire(timeout=0.5):
            return False 
        
        if not name in list(self.ressources.keys()):
            self.mutex.release()
            raise RuntimeError("Unknown ressource !")
        
        if self.ressources[name]!=-1 and self.ressources[name]!=idx:
            try:
                self.execute(_PING_OPCODE,id=self.ressources[name])
            except (ConnectionError, TimeoutError, KeyError):
                self.ressources[name] = idx
                self.mutex.release()
                return True    
            self.mutex.release()
            return False
        else:
            self.ressources[name] = idx
            self.mutex.release()
            return True

    def release_ressource(self, idx, name):
        if not self.mutex.acquire(timeout=0.5):
            return 
        
        if not name in list(self.ressources.keys()):
            self.mutex.release()
        
        if self.ressources[name]!=idx:
            self.mutex.release()
            return
        else:
            self.ressources[name] = -1
            self.mutex.release()
            return
 