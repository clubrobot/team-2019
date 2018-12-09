#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from threading import Thread, RLock


class Flag:
    def __init__(self, function):
        self.linked_signals = list()
        self.target = function
        self.lock = RLock()

    def bind(self, signal):
        self.lock.acquire()
        self.linked_signals.append(signal)
        signal._bind(self)
        self.lock.release()
    
    def attach(self, function):
        self.lock.acquire()
        self.target = function
        self.lock.release()
        
    def ping(self):
    
        if not self.lock.acquire(timeout=0.2) : return
        self.target()
        #try:
        #    self.target()
        #except Exception:
        #    _, value, _ = exc_info()
        #    print("UserWarning : Error on the frags execution : {}".format(value))
        self.lock.release()

    def clear(self):
        self.lock.acquire()
        for signal in self.linked_signals:
            signal.clear(self)
        self.linked_signals = list()
        self.lock.release()


class Signal:
    def __init__(self):    
        self.linked_flags = list()
        self.lock  = RLock()
        self.clear_lock = RLock()

    def ping(self):
        if not self.lock.acquire(blocking=False) : return
        thread_list = list()
        self.clear_lock.acquire()
        for flag in self.linked_flags:
            thread_list.append(Thread(target=flag.ping))
            thread_list[-1].start()
        self.clear_lock.release()
        status = False
        while status:
            status = False
            for thread in thread_list:
                status =  thread.is_alive() and status
            
        self.lock.release()

    def _bind(self, flag):
        self.clear_lock.acquire()
        self.linked_flags.append(flag)
        self.clear_lock.release()
    
    def clear(self, flag):
        self.clear_lock.acquire()
        self.linked_flags.remove(flag)
        self.clear_lock.release()


