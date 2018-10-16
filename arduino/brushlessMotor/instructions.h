#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"
#include "PIN.h"


// Opcodes declaration
#define GET_VELOCITY_OPCODE         0x10
#define GET_CONSTANT_OPCODE         0x11
#define GET_WHEELRADIUS_OPCODE      0x12
#define GET_MAX_VELOCITY_OPCODE     0x13

#define IS_ENABLED_OPCODE           0x14

#define SET_VELOCITY_OPCODE         0x15
#define SET_CONSTANT_OPCODE         0x16
#define SET_WHEELRADIUS_OPCODE      0x17

#define ENABLE_OPCODE               0X18
#define DISABLE_OPCODE              0X19

#define LOAD_OPCODE                 0X1A
#define SAVE_OPCODE                 0X1B


// Instructions prototypes

void GET_VELOCITY(SerialTalks& inst, Deserializer& input, Serializer& output);

void GET_CONSTANT(SerialTalks& inst, Deserializer& input, Serializer& output);

void GET_WHEELRADIUS(SerialTalks& inst, Deserializer& input, Serializer& output);

void GET_MAX_VELOCITY(SerialTalks& inst, Deserializer& input, Serializer& output);

void IS_ENABLED(SerialTalks& inst, Deserializer& input, Serializer& output);

void SET_VELOCITY(SerialTalks& inst, Deserializer& input, Serializer& output);

void SET_CONSTANT(SerialTalks& inst, Deserializer& input, Serializer& output);

void SET_WHEELRADIUS(SerialTalks& inst, Deserializer& input, Serializer& output);

void ENABLE(SerialTalks& inst, Deserializer& input, Serializer& output);

void DISABLE(SerialTalks& inst, Deserializer& input, Serializer& output);

void LOAD(SerialTalks& inst, Deserializer& input, Serializer& output);

void SAVE(SerialTalks& inst, Deserializer& input, Serializer& output);


#endif //__INSTRUCTIONS_H__

