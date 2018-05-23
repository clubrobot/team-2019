from robots.beacons_manager import BeaconsManagement
from beacons.balise_receiver import BaliseReceiver
import time


anchor = BaliseReceiver("192.168.1.11")
#anchor.connect()
a = BeaconsManagement(anchor, "area.ggb")
a.start()

a.create_area("Dispenser3", patern="auxDispenser3_*")
a.create_area("Treatment0", patern="auxTreatment0_*")
a.create_area("Treatment1", patern="auxTreatment1_*")
a.create_area("Dispenser2", patern="auxDispenser2_*")
a.running.clear()
#while True:
#    print(a)
#    time.sleep(0.1)

t = a.areas["Treatment0"]
t.increase(1699.8983, 1726.21259)