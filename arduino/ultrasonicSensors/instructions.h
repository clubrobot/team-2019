#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"
#include "PIN.h"


// Opcodes declaration
#define GET_MESURE_OPCODE			0x10
#define ACTIVATE_SENSORS_OPCODE 	0x11
#define DESACTIVATE_SENSORS_OPCODE  0x12
#define GET_NORMAL_OPCODE           0x13
#define GET_LEFT_SWITCH_OPCODE   	0X14
#define GET_RIGHT_SWITCH_OPCODE	    0X15


// Instructions prototypes
void GET_NORMAL(SerialTalks& inst, Deserializer& input, Serializer& output);

void GET_MESURE(SerialTalks& inst, Deserializer& input, Serializer& output);

void ACTIVATE_SENSORS(SerialTalks& inst, Deserializer& input, Serializer& output);

void DESACTIVATE_SENSORS(SerialTalks& inst, Deserializer& input, Serializer& output);

void GET_LEFT_SWITCH(SerialTalks& inst, Deserializer& input, Serializer& output);

void GET_RIGHT_SWITCH(SerialTalks& inst, Deserializer& input, Serializer& output);

#endif






