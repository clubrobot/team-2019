#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"
#include "../common/Pickle.h"

#define PING_OPCODE      0x10

#define SET_START_OPCODE	 0X11
#define GET_START_OPCODE	 0X12


void PING(SerialTalks& talks, Deserializer& input, Serializer& output);

void SET_START(SerialTalks& talks, Deserializer& input, Serializer& output);

void GET_START(SerialTalks& talks, Deserializer& input, Serializer& output);



#endif //__INSTRUCTIONS_H__
