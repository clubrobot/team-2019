#!/usr/bin/env python3
#-*- coding: utf-8 -*-
import time
import math
import paho.mqtt.client as mqtt
import time

# Create client instance and connect to localhost
client = mqtt.Client()
client.connect("localhost",1883,60)

# Publish message to topic/iopi and set pin 1 on bus 1 to on


class Electron():

	def __init__(self, uuid='expServ'):
		#SerialTalks.__init__(self, "/dev/arduino/{}".format(uuid))
		pass

	def isOnTop(self):
		return 0

	def start(self):
		client.publish("topic/start", "2")
