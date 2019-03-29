#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"

#define GET_PRESSURE_OPCODE	             0X18
#define ATMOSPHERE_PRESSURE_OPCODE	     0X19

void GET_PRESSURE(SerialTalks& talks, Deserializer& input, Serializer& output);

void ATMOSPHERE_PRESSURE(SerialTalks& talks, Deserializer& input, Serializer& output);

#endif //__INSTRUCTIONS_H__
