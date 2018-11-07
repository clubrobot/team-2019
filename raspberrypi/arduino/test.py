from Sensor_IR import *


path = "/dev/tty.wchusbserial1420"
Sensors = Sensors(path) # tous en même temps
#Sensor0 = Sensors(path,0) # Juste le numéro 0
#Sensor1 = Sensors(path,1)
#Sensor2 = Sensors(path,2)
#Sensor02 = Sensor(path, [Sensors(path,0),Sensors(path,2)]) # les 2 en même temps


Sensors.connect()
#Sensor0.connect()
