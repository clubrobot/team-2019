#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"


#define _GET_EMERGENCY_STOP_STATE_OPCODE     	0x04
#define _GET_VOLTAGE_OPCODE         			0X06
#define _GET_BATTERY_CHARGE_OPCODE				0x0A


void GET_EMERGENCY_STOP_STATE(SerialTalks& inst, Deserializer& input, Serializer& output);

void GET_VOLTAGE(SerialTalks& inst, Deserializer& input, Serializer& output);

void GET_BATTERY_CHARGE(SerialTalks& inst, Deserializer& input, Serializer& output);


#endif