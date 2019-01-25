#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import os
from types import MethodType

from common.tcptalks import *

COMPONENTS_SERVER_DEFAULT_PORT = 25566

CREATE_SERIALTALKS_COMPONENT_OPCODE = 0x10
CREATE_SWITCH_COMPONENT_OPCODE      = 0x11
CREATE_LIGHTBUTTON_COMPONENT_OPCODE = 0x12
CREATE_PICAMERA_COMPONENT_OPCODE    = 0x13

MAKE_COMPONENT_EXECUTE_OPCODE       = 0x20
GET_COMPONENT_ATTRIBUTE_OPCODE      = 0x21
SET_COMPONENT_ATTRIBUTE_OPCODE      = 0x22

UPDATE_MANAGER_PICAMERA_OPCODE      = 0x30
MAKE_MANAGER_EXECUTE_OPCODE         = 0x50


class NoAvailiableComponent(Exception): pass
class NoAvailiableMethode(Exception): pass



class Component():

	def _setup(self): pass
	def _cleanup(self): pass


class SerialTalksComponent(Component):

	def __init__(self, uuid):
		for methodname in ('connect','disconnect','poll'):
			def method(self, *args, methodname=methodname, **kwargs):
				pass
			object.__setattr__(self, methodname, MethodType(method, self))
		self.uuid = uuid
		self.methodlist = dict()

	def add_method(self,opcode,function):
		if opcode in self.methodlist:
			raise Exception('Opcode already use')
		self.methodlist[opcode] = function
		
	def send(self,opcode, *args):
		if opcode in self.methodlist:
			self.methodlist[opcode](*args)

	def execute(self,opcode, *args,timeout=5):
		if opcode in self.methodlist:
			return self.methodlist[opcode](*args)
		else:
			raise Exception('No availiable methode')

	def getuuid(self,timeout=5):
		return self.uuid

	def setuuid(self,uuid, timeout=5):
		self.uuid = uuid

	def geterr(self,timeout=5):
		return 'not availiable'
	
	def getout(self,timeout=5):
		return 'not availiable in this environement'
	

class SwitchComponent(Component):
	def __init__(self, switchpin):
		pass

class LightButtonComponent(Component):
	def __init__(self, switchpin, ledpin):
		pass
		


class Server(TCPTalksServer):

	def __init__(self, port=COMPONENTS_SERVER_DEFAULT_PORT, password=None):
		TCPTalksServer.__init__(self, port=port, password=password)
		self.bind(CREATE_SERIALTALKS_COMPONENT_OPCODE, self.CREATE_SERIALTALKS_COMPONENT)
		self.bind(CREATE_SWITCH_COMPONENT_OPCODE,      self.CREATE_SWITCH_COMPONENT)
		self.bind(CREATE_LIGHTBUTTON_COMPONENT_OPCODE, self.CREATE_LIGHTBUTTON_COMPONENT)
		self.bind(CREATE_PICAMERA_COMPONENT_OPCODE,    self.CREATE_PICAMERA_COMPONENT)
		self.bind(MAKE_COMPONENT_EXECUTE_OPCODE,       self.MAKE_COMPONENT_EXECUTE)
		self.bind(GET_COMPONENT_ATTRIBUTE_OPCODE,      self.GET_COMPONENT_ATTRIBUTE)
		self.bind(SET_COMPONENT_ATTRIBUTE_OPCODE,      self.SET_COMPONENT_ATTRIBUTE)
		self.components = {}
		self.availiable_components = {}
	
	def disconnect(self):
		TCPTalks.disconnect(self)
		self.availiable_components = self.components
		self.components = {}

		
	def cleanup(self):
		for comp in self.components.values():
			comp._cleanup()
		self.components = {}
		self.availiable_components = {}

	def addcomponent(self, comp):
		if not comp.getuuid() in self.availiable_components:
			self.availiable_components[comp.getuuid()] = comp

	def MAKE_COMPONENT_EXECUTE(self,id, compid, methodname, args, kwargs):
		comp = self.components[id][compid]
		#print("Demande d'execution recu : " + str(methodname) + "avec les args"+ str(args))

		output = getattr(comp, methodname)(*args, **kwargs)
		#print(output)
		return output

	def GET_COMPONENT_ATTRIBUTE(self,id, compid, attrname):
		comp = self.components[id][compid]
		comp._setup()
		return getattr(comp, attrname)

	def SET_COMPONENT_ATTRIBUTE(self,id, compid, attrname, attrvalue):
		comp = self.components[compid]
		comp._setup()
		return setattr(comp, attrname, attrvalue)

	def CREATE_SERIALTALKS_COMPONENT(self,id, uuid):
		if uuid in self.availiable_components:
			if isinstance(self.availiable_components[uuid],SerialTalksComponent):
				try:
					self.components[id][uuid] = self.availiable_components[uuid]
				except KeyError:
					self.components[id] = dict()
					self.components[id][uuid] = self.availiable_components[uuid]

				del self.availiable_components[uuid]
				return uuid
		else :
			raise Exception('No availiable components')
		

	def CREATE_SWITCH_COMPONENT(self,id, switchpin):
		comp = SwitchComponent(switchpin)
		compid = (switchpin,)
		self.addcomponent(comp, compid)
		comp.SetFunction(self.send, MAKE_MANAGER_EXECUTE_OPCODE, compid)
		return compid

	def CREATE_LIGHTBUTTON_COMPONENT(self,id, switchpin, ledpin):
		comp = LightButtonComponent(switchpin, ledpin)
		compid = (switchpin, ledpin)
		self.addcomponent(comp, compid)
		comp.SetFunction(self.send, MAKE_MANAGER_EXECUTE_OPCODE, compid)
		return compid

	def CREATE_PICAMERA_COMPONENT(self,id, resolution, framerate):
		raise Exception('No availiable component !')
