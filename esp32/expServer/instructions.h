#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"

#define IS_CONNECTED_OPCODE         0X15
#define START_OPCODE                0X16

#define EXPERIENCE_STATE_OPCODE     0X17
#define ELECTRON_STATE_OPCODE       0X18

void IS_CONNECTED(SerialTalks &inst, Deserializer &input, Serializer &output);
void START(SerialTalks &inst, Deserializer &input, Serializer &output);

void EXPERIENCE_STATE(SerialTalks &inst, Deserializer &input, Serializer &output);
void ELECTRON_STATE(SerialTalks &inst, Deserializer &input, Serializer &output);


#endif //__INSTRUCTIONS_H__
