#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import math
from common.serialutils import Deserializer
from common.serialtalks import BYTE, INT, LONG, FLOAT, STRING, SerialTalks
from common.components import SecureSerialTalksProxy
# Instructions

_GET_SINGLE_MESURE_OPCODE     = 0x10
_GET_CONTINUOUS_MESURE_OPCODE = 0x11
_START_CONTINUOUS_OPCODE      = 0x12
_STOP_CONTINUOUS_OPCODE       = 0x13


class Sensors(SerialTalks):
    # Default execute result
    def __init__(self, path):
        """
            Passe en paramètre le numéro du sensor à utiliser ou une liste de sensors à utiliser
            Si numero_Sensor est laissé par défaut, tous les sensors sur Arduino sont conserné
        """
        SerialTalks.__init__(self, path)
        #self.get_info()

    def get_info(self):
        pass
        #output = self.execute()
        #info = output.read()
        #return info

    def get_single_mesure(self):
        """
            Permet de récupérer une distance sans passé par le mode continu
        """
        output = self.execute(_GET_SINGLE_MESURE_OPCODE)
        dist = output.read(STRING)
        return list(map(int,dist.split(';')[:-1]))

    def get_continuous_mesure(self):
        """
            Si le mode est continu, récupère une distance (ou une MG des distances)
        """
        output = self.execute(_GET_CONTINUOUS_MESURE_OPCODE)
        dist = output.read(STRING)
        return list(map(float,dist.split(';')[:-1]))

    def start_continuous(self, MG_nb_echantillons = 0):
        """
            Active le mode continu
            MG_nb_echantillons : si 0 alors MG désactivé, sinon on effectue la moyenne glissante sur ce nombre d'échantillons
        """
        self.send(_START_CONTINUOUS_OPCODE, INT(MG_nb_echantillons))

    def stop_continuous(self):
        """
            Désactive le mode continu
        """
        self.send(_STOP_CONTINUOUS_OPCODE)
