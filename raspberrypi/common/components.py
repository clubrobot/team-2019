#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os, time, sys
from types import MethodType
from threading import RLock
import warnings
from common.tcptalks import TCPTalks, TCPTalksServer, NotConnectedError
from common.serialtalks import RESEND_OPCODE
COMPONENTS_SERVER_DEFAULT_PORT = 25566

CREATE_SERIALTALKS_COMPONENT_OPCODE = 0x10
CREATE_SWITCH_COMPONENT_OPCODE = 0x11
CREATE_LIGHTBUTTON_COMPONENT_OPCODE = 0x12
CREATE_PICAMERA_COMPONENT_OPCODE = 0x13

MAKE_COMPONENT_EXECUTE_OPCODE  = 0x20
GET_COMPONENT_ATTRIBUTE_OPCODE = 0x21
SET_COMPONENT_ATTRIBUTE_OPCODE = 0x22
END_GAME_OPCODE                = 0x23

UPDATE_MANAGER_PICAMERA_OPCODE = 0x30
MAKE_MANAGER_REPLY_OPCODE = 0x40
MAKE_MANAGER_EXECUTE_OPCODE = 0x50


class Component():

    def _setup(self): pass

    def _cleanup(self): pass


try:
    from common.serialtalks import *


    class SerialTalksComponent(SerialTalks, Component):

        def __init__(self, parent, uuid):
            self.parent = parent
            self.uuid = uuid
            SerialTalks.__init__(self, os.path.join('/dev/arduino', uuid))

        def _setup(self):
            try:
                self.connect()
            except AlreadyConnectedError:
                pass

        def _cleanup(self):
            self.disconnect()

        def receive(self, input, timeout=0.5):
            opcode = input.read(BYTE)
            retcode = input.read(ULONG)
            try:
                output = self.instructions[opcode](input)
                if output is None: return
                content = ULONG(retcode) + output
                prefix = SLAVE_BYTE + BYTE(len(content))
                self.rawsend(prefix + content)
            except KeyError:
                pass

            try:
                output = self.parent.execute(MAKE_MANAGER_REPLY_OPCODE, opcode, input, timeout=0.5)
                if output is None: return
                content = LONG(retcode) + output
                prefix = SLAVE_BYTE + BYTE(len(content))
                self.rawsend(prefix + content)
            except ConnectionError:
                return
            except Exception:
                etype, value, _ = sys.exc_info()
                print("Error with an request from {} arduino".format(self.uuid))
                print(etype, value)
                return



except ImportError:
    pass

try:
    from common.gpiodevices import Switch, LightButton, Device

    class SwitchComponent(Switch, Component):
        def __init__(self, switchpin, active_high=True):
            Switch.__init__(self, switchpin, active_high=active_high)

        def _cleanup(self):
            self.close()


    class LightButtonComponent(LightButton, Component):
        def __init__(self, switchpin, ledpin):
            LightButton.__init__(self, switchpin, ledpin)

        def _cleanup(self):
            self.close()

except ImportError:
    Switch = None
except RuntimeError:
    pass

try:
    from picamera import PiCamera
    from threading import Thread, Event
    import time
    import io


    class PiCameraComponent(PiCamera, Component):

        def __init__(self, server, resolution, framerate):
            PiCamera.__init__(self, resolution=resolution, framerate=framerate)
            self.server = server
            self.compid = None
            self.capturing = Event()

        def generate_streams_and_send_their_values(self, server, compid):
            stream = io.BytesIO()
            while self.capturing.is_set():
                # Give the hand to the PiCamera.capture_sequence method
                stream.seek(0)
                yield stream
                # Send the captured stream
                stream.truncate()
                try:
                    server.send(UPDATE_MANAGER_PICAMERA_OPCODE, compid, stream.getvalue())
                except:
                    break

        def start_capture(self):
            if not self.capturing.is_set():
                self.capturing.set()
                streams_generator = self.generate_streams_and_send_their_values(self.server, self.compid)
                # Start the capture in a thread
                target = self.capture_sequence
                args = (streams_generator, 'jpeg')
                kwargs = {'use_video_port': True}
                thread = Thread(target=target, args=args, kwargs=kwargs, daemon=True)
                thread.start()

        def stop_capture(self):
            self.capturing.clear()

        def _cleanup(self):
            self.stop_capture()
except ImportError:
    pass


class Server(TCPTalksServer):

    def __init__(self, port=COMPONENTS_SERVER_DEFAULT_PORT, password=None, size=4):
        TCPTalksServer.__init__(self, port=port, password=password, NbClients=size)
        self.bind(CREATE_SERIALTALKS_COMPONENT_OPCODE, self.CREATE_SERIALTALKS_COMPONENT)
        self.bind(CREATE_SWITCH_COMPONENT_OPCODE, self.CREATE_SWITCH_COMPONENT)
        self.bind(CREATE_LIGHTBUTTON_COMPONENT_OPCODE, self.CREATE_LIGHTBUTTON_COMPONENT)
        self.bind(CREATE_PICAMERA_COMPONENT_OPCODE, self.CREATE_PICAMERA_COMPONENT)
        self.bind(MAKE_COMPONENT_EXECUTE_OPCODE, self.MAKE_COMPONENT_EXECUTE)
        self.bind(GET_COMPONENT_ATTRIBUTE_OPCODE, self.GET_COMPONENT_ATTRIBUTE)
        self.bind(SET_COMPONENT_ATTRIBUTE_OPCODE, self.SET_COMPONENT_ATTRIBUTE)
        self.bind(END_GAME_OPCODE, self.END_GAME)
        self.components = {}

    def disconnect(self, id=None):
        TCPTalksServer.disconnect(self, id=id)
        if len(self.client) == 0: self.cleanup()

    def cleanup(self):
        for comp in self.components.values():
            comp._cleanup()
        self.components = {}

    def END_GAME(self, *args):
        self.disconnect()
        for compid in self.components:
            if isinstance(compid,SerialTalksComponent):
                compid._cleanup()
        

    def addcomponent(self, comp, compid):
        if not compid in self.components:
            self.components[compid] = comp
        self.components[compid]._setup()

    def MAKE_COMPONENT_EXECUTE(self, compid, methodname, args, kwargs):
        comp = self.components[compid]
        comp._setup()
        return getattr(comp, methodname)(*args, **kwargs)

    def GET_COMPONENT_ATTRIBUTE(self, compid, attrname):
        comp = self.components[compid]
        comp._setup()
        return getattr(comp, attrname)

    def SET_COMPONENT_ATTRIBUTE(self, compid, attrname, attrvalue):
        comp = self.components[compid]
        comp._setup()
        return setattr(comp, attrname, attrvalue)

    def CREATE_SERIALTALKS_COMPONENT(self, uuid):
        comp = SerialTalksComponent(self, uuid)
        compid = uuid
        self.addcomponent(comp, compid)
        return compid

    def CREATE_SWITCH_COMPONENT(self, switchpin, active_high=True):
        if (switchpin,) in self.components:
            return (switchpin,)
        comp = SwitchComponent(switchpin, active_high=active_high)
        compid = (switchpin,)
        self.addcomponent(comp, compid)
        comp.set_function(self.send, MAKE_MANAGER_EXECUTE_OPCODE, compid, id=None, broadcast=True)
        return compid

    def CREATE_LIGHTBUTTON_COMPONENT(self, switchpin, ledpin):
        if (switchpin, ledpin) in self.components:
            return (switchpin, ledpin)
        comp = LightButtonComponent(switchpin, ledpin)
        compid = (switchpin, ledpin)
        self.addcomponent(comp, compid)
        comp.set_function(self.send, MAKE_MANAGER_EXECUTE_OPCODE, compid, id=None, broadcast=True)
        return compid

    def CREATE_PICAMERA_COMPONENT(self, resolution, framerate):
        comp = PiCameraComponent(self, resolution, framerate)
        compid = 'camera'
        self.addcomponent(comp, compid)
        comp.server = self
        comp.compid = compid
        return compid


class Manager(TCPTalks):
    MANAGER_CREATED = None

    def __init__(self, ip='localhost', port=COMPONENTS_SERVER_DEFAULT_PORT, password=None):
        TCPTalks.__init__(self, ip, port=port, password=password)
        # PiCamera components
        self.bind(UPDATE_MANAGER_PICAMERA_OPCODE, self.UPDATE_MANAGER_PICAMERA)
        self.picameras = {}
        # GPIO components
        self.bind(MAKE_MANAGER_EXECUTE_OPCODE, self.MAKE_MANAGER_EXECUTE)
        self.functions = {}
        self.args = {}
        # SerialTalks
        self.bind(MAKE_MANAGER_REPLY_OPCODE, self.MAKE_MANAGER_REPLY)
        self.serial_instructions = {}
        Manager.MANAGER_CREATED = self

    def UPDATE_MANAGER_PICAMERA(self, compid, streamvalue):
        cvimageflags = 1
        if compid in self.picameras:
            currentframe = cv2.imdecode(np.frombuffer(streamvalue, dtype='uint8'), cvimageflags)
            if currentframe is not None:
                self.picameras[compid].currentframe = currentframe

    def MAKE_MANAGER_EXECUTE(self, compid):
        if compid in self.functions:
            self.functions[compid](*self.args[compid])

    def MAKE_MANAGER_REPLY(self, opcode, input):
        if opcode in self.serial_instructions:
            result = self.serial_instructions[opcode](input)
            return result

    def end_game(self):
        self.send(END_GAME_OPCODE)
        self.disconnect()


class Proxy:
    def __init__(self, manager, compid, attrlist, methlist):
        object.__setattr__(self, '_manager', manager)
        object.__setattr__(self, '_compid', compid)
        object.__setattr__(self, '_attrlist', attrlist)
        for methodname in methlist:
            def method(self, *args, tcptimeout=10, methodname=methodname, **kwargs):
                return self._manager.execute(MAKE_COMPONENT_EXECUTE_OPCODE, compid, methodname, args, kwargs,
                                             timeout=tcptimeout)

            object.__setattr__(self, methodname, MethodType(method, self))

    def __getattr__(self, attrname, tcptimeout=10):
        if attrname in object.__getattribute__(self, "_attrlist"):
            return self._manager.execute(GET_COMPONENT_ATTRIBUTE_OPCODE, self._compid, attrname, timeout=tcptimeout)
        else:
            object.__getattribute__(self, attrname)

    def __setattr__(self, attrname, attrvalue, tcptimeout=10):
        if attrname in self._attrlist:
            return self._manager.execute(SET_COMPONENT_ATTRIBUTE_OPCODE, self._compid, attrname, attrvalue,
                                         timeout=tcptimeout)
        else:
            object.__setattr__(self, attrname, attrvalue)


class SerialTalksProxy(Proxy):

    def __init__(self, manager, uuid):
        compid = manager.execute(CREATE_SERIALTALKS_COMPONENT_OPCODE, uuid, timeout=6)
        attrlist = ['port', 'is_connected']
        methlist = ['connect', 'disconnect', 'send', 'poll', 'flush', 'execute', 'getuuid', 'setuuid', 'getout',
                    'geterr']
        Proxy.__init__(self, manager, compid, attrlist, methlist)

    def bind(self, opcode, function):
        if not str(opcode) + self._compid in self._manager.instructions:
            self._manager.serial_instructions[str(opcode) + self._compid] = function
        else:
            raise KeyError("Opcode already use")

class TimeoutWarning(UserWarning): pass
class NotConnectedWarning(UserWarning): pass
class MuteWarning(UserWarning): pass

class SecureSerialTalksProxy(Proxy):
    def __init__(self, manager, uuid, default_result):
        attrlist = ['port', 'is_connected']
        methlist = ['connect', 'disconnect', 'send', 'poll', 'flush', 'execute', 'getuuid', 'setuuid', 'getout',
                    'geterr']
        object.__setattr__(self, '_attrlist', attrlist)
        if not hasattr(self,"lock") : object.__setattr__(self, 'lock', RLock())
        if not hasattr(self, "default_result"): object.__setattr__(self, 'default_result', default_result)
        object.__setattr__(self, 'initialized', False)
        try:
            compid = manager.execute(CREATE_SERIALTALKS_COMPONENT_OPCODE, uuid)
            Proxy.__init__(self, manager, compid, attrlist, methlist)
            self.initialized = True
            connect_addr = self.connect
            execute_addr = self.execute
            send_addr = self.send
        except (ConnectionFailedError,TimeoutError):
            object.__setattr__(self, '_compid', uuid)
            warnings.warn("Arduino {} is unreachable !".format(uuid), NotConnectedWarning)
            def trash_none(*args,**kwargs) : return None
            def trash_return(opcode, *args, **kwargs):
                if opcode in default_result.keys():
                    return default_result[opcode].__copy__()
                else:
                    return None
            connect_addr = trash_none
            execute_addr = trash_return
            send_addr = trash_none

        self.default_result = default_result

        def connect(self, with_lock=True, **kwargs):
            if with_lock : object.__getattribute__(self, "lock").acquire()
            if self.initialized == False:
                try:
                    SecureSerialTalksProxy.__init__(self,manager, uuid, default_result)
                except:
                    pass
            try:
                connect_addr(**kwargs)
            except AlreadyConnectedError:
                pass
            except KeyError:
                etype, value, tb = sys.exc_info()
                print(etype, tb, value)
                self.initialized = False
                warnings.warn("Arduino {} is unreachable ! (KeyError)".format(uuid), NotConnectedWarning)
            except (NotConnectedError, ConnectionFailedError):
                etype, value, tb = sys.exc_info()
                print(etype, tb, value)
                warnings.warn("Arduino {} is unreachable ! (NotConnectedError or ConnectionFailedError)".format(uuid), NotConnectedWarning)
            except MuteError:
                etype, value, tb = sys.exc_info()
                print(etype, tb, value)
                warnings.warn("Arduino {} is unreachable (MuteError)!".format(uuid), NotConnectedWarning)
            if with_lock: object.__getattribute__(self, "lock").release()

        def execute(self, opcode, *args, **kwargs):
            object.__getattribute__(self, "lock").acquire()
            if self.initialized == False:
                try:
                    SecureSerialTalksProxy.__init__(self,manager, uuid, default_result)
                except:
                    pass
            try:
                result = execute_addr(opcode, *args, **kwargs)
            except (NotConnectedError, ConnectionFailedError, KeyError):
                self.connect(with_lock=False)
                if opcode in self.default_result.keys():
                    object.__getattribute__(self, "lock").release()
                    return self.default_result[opcode].__copy__()
                else:
                    object.__getattribute__(self, "lock").release()
                    return None
            except TimeoutError:
                warnings.warn("Timeout Error with {}".format(uuid), TimeoutWarning)
                if opcode in self.default_result.keys():
                    object.__getattribute__(self, "lock").release()
                    return self.default_result[opcode].__copy__()
                else:
                    object.__getattribute__(self, "lock").release()
                    return None
            object.__getattribute__(self, "lock").release()
            return result

        def send(self, opcode, *args):
            object.__getattribute__(self, "lock").acquire()
            if self.initialized == False:
                try:
                    SecureSerialTalksProxy.__init__(self,manager, uuid, default_result)
                except RuntimeError as e:
                    raise e
                except:
                    pass
            try:
                send_addr(opcode, *args)
            except (NotConnectedError, KeyError):
                self.connect(with_lock=False)
            except ConnectionFailedError:
                self.connect(with_lock=False)
            except TimeoutError:
                warnings.warn("Timeout Error with {}".format(uuid), TimeoutWarning)
            object.__getattribute__(self, "lock").release()

        object.__setattr__(self, "connect", MethodType(connect, self))
        object.__setattr__(self, "execute", MethodType(execute, self))
        object.__setattr__(self, "send", MethodType(send, self))

    def bind(self, opcode, function):
        if not str(opcode) + self._compid in self._manager.instructions:
            self._manager.serial_instructions[str(opcode) + self._compid] = function
        else:
            raise KeyError("Opcode already use")


class SwitchProxy(Proxy):

    def __init__(self, manager, switchpin, active_high=True):
        compid = manager.execute(CREATE_SWITCH_COMPONENT_OPCODE, switchpin, active_high=active_high)
        attrlist = ['state', 'input_pin']
        methlist = ['close', 'set_active_high']
        Proxy.__init__(self, manager, compid, attrlist, methlist)

    def set_function(self, function, *args):
        self._manager.functions[self._compid] = function
        self._manager.args[self._compid] = args


class LightButtonProxy(Proxy):

    def __init__(self, manager, switchpin, ledpin):
        compid = manager.execute(CREATE_LIGHTBUTTON_COMPONENT_OPCODE, switchpin, ledpin)
        attrlist = ['state', 'input_pin', 'light_pin']
        methlist = ['set_auto_switch', 'on', 'off', 'switch', 'close']
        Proxy.__init__(self, manager, compid, attrlist, methlist)

    def set_function(self, function, *args):
        self._manager.functions[self._compid] = function
        self._manager.args[self._compid] = args


try:
    import cv2
    import numpy as np


    class PiCameraProxy(Proxy):

        def __init__(self, manager, resolution, framerate):
            compid = manager.execute(CREATE_PICAMERA_COMPONENT_OPCODE, resolution, framerate)
            attrlist = ['resolution', 'framerate']
            methlist = ['start_capture', 'stop_capture']
            Proxy.__init__(self, manager, compid, attrlist, methlist)

            manager.picameras[compid] = self
            self.currentframe = np.array([])  # TODO: initialize with an empty frame

        def get_current_frame(self):
            return self.currentframe.copy()
except ImportError:
    pass
