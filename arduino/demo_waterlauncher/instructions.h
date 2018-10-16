#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"



#define _SET_MOTOR_VELOCITY_OPCODE  0x11
#define _WRITE_INDOOR_OPCODE        0x12
#define _WRITE_OUTDOOR_OPCODE       0x13
#define _WRITE_TRASH_OPCODE        	0x14
#define _GET_MOTOR_VELOCITY_OPCODE         0x15


void SET_MOTOR_VELOCITY(SerialTalks &inst, Deserializer &input, Serializer &output);

void WRITE_INDOOR(SerialTalks &inst, Deserializer &input, Serializer &output);

void WRITE_OUTDOOR(SerialTalks &inst, Deserializer &input, Serializer &output);

void WRITE_TRASH(SerialTalks &inst, Deserializer &input, Serializer &output);

void GET_MOTOR_VELOCITY(SerialTalks &inst, Deserializer &input, Serializer &output);

#endif
