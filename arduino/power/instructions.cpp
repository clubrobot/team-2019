#include "instructions.h"
#include "configuration.h"

extern float voltage ;
extern int percentage;

// Return the emmergency button state (boolean value : 0 = Emergency stop; 1 = Normal)
void GET_EMERGENCY_STOP_STATE(SerialTalks &inst, Deserializer &input, Serializer &output)
{
    output << digitalRead(EMERGENCY_BUTTON_PIN);
}

// Return the battery voltage (float value)
void GET_VOLTAGE(SerialTalks &inst, Deserializer &input, Serializer &output)
{
   	output << voltage;
}

// Return the percentage of charge of the batery (int value)
void GET_BATTERY_CHARGE(SerialTalks &inst, Deserializer &input, Serializer &output)
{
	output << percentage;
}
