#include <Arduino.h>
#include "../../common/interrupt/PinChangeInt/PinChangeInt.h"

#include "PIN.h"
#include "instructions.h"
#include "../../common/SerialTalks.h"

void inter_switch1();
void inter_switch2();

void setup()
{
	Serial.begin(SERIALTALKS_BAUDRATE);
  	talks.begin(Serial);

  	pinMode(INTER1, INPUT_PULLUP);
  	pinMode(INTER2, INPUT_PULLUP);

  	PCintPort::attachInterrupt(INTER1, inter_switch1, FALLING);
  	PCintPort::attachInterrupt(INTER2, inter_switch2, FALLING);
}

void loop()
{
	talks.execute();
}

void inter_switch1()
{
	static unsigned long last_time = 0;

  	unsigned long time = millis();

  	if ((time - last_time) > 2) 
  	{
    	Serializer inter1 = talks.getSerializer();
    	inter1.write<byte>(1);
        talks.send(1, inter1);
    	last_time = time;
  	}

}
void inter_switch2()
{
	static unsigned long last_time = 0;

  	unsigned long time = millis();

  	if ((time - last_time) > 2) 
  	{
    	Serializer inter2 = talks.getSerializer();
    	inter2.write<byte>(1);
        talks.send(2, inter2);
    	last_time = time;
  	}
}
