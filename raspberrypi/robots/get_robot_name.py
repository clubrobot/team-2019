#!/usr/bin/env python3
# -*- coding: utf-8 -*-

BORNIBUS_ID = 0
R128_ID     = 1
UNKNOWN     = -1

ROBOT_ID = UNKNOWN
try:
    f = open("/opt/BORNIBUS")
    ROBOT_ID = BORNIBUS_ID
    f.close()
except IOError:
    pass
try:
    if ROBOT_ID!=BORNIBUS_ID:
        f = open("/opt/128")
        ROBOT_ID = R128_ID
        f.close()
except IOError:
    pass

