from common.serialtalks import *
from math import pi

s = SerialTalks("/tmp/arduino/esp32")
s.connect();

x 	= 10
y	= 10
phi = 0

output = s.execute(0x10,FLOAT(x),FLOAT(y),FLOAT(phi));


(th1,th2,th3) = output.read(FLOAT,FLOAT,FLOAT)
print("th1 end:",th1," th2:",th2," th3:",th3)



s.disconnect();