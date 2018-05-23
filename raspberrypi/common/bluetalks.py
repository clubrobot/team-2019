#!/usr/bin/python3
#-*- coding: utf-8 -*-

import bluetooth
import tcptalks
import time


# Main class

class BlueTalks(tcptalks.TCPTalks):

	@staticmethod
	def _serversocket(port, timeout):
		# Create a server
		serversocket = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
		serversocket.bind(('', port))
		serversocket.listen(1)

		# Wait for the other to connect
		serversocket.settimeout(timeout)
		try:
			clientsocket = serversocket.accept()[0]
			return clientsocket
		except bluetooth.btcommon.BluetoothError:
			raise tcptalks.ForeverAloneError('no connection request') from None
		finally:
			serversocket.close() # The server is no longer needed

	@staticmethod
	def _clientsocket(ip, port, timeout):
		# Connect to the other
		startingtime = time.monotonic()
		while timeout is None or time.monotonic() - startingtime < timeout:
			try:
				clientsocket = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
				clientsocket.connect((ip, port))
				return clientsocket
			except bluetooth.btcommon.BluetoothError:
				continue
		raise tcptalks.ForeverAloneError('no server found') from None

	_timeouterror = bluetooth.btcommon.BluetoothError
