from common.components import *

if "manager" not in globals():
    hostname = ""
    if hostname == "":
        print("IP adress :  ")
        hostname = input()
        if len(hostname) == 0:
            hostname = "127.0.0.1"
        elif len(hostname.split(".")) == 1:
            hostname = "192.168.1." + hostname
        print("Try reaching raspberry at IP " + hostname + "...")

    # Connect to the Raspberry Pi and the different modules
    manager = Manager(hostname)
    manager.connect(10)