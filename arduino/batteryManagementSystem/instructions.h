#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"

// Opcodes declaration
#define GET_RSOC_OPCODE                 0x04
#define GET_ASOC_OPCODE                 0x05
#define GET_VOLTAGE_OPCODE              0x06
#define GET_CYCLE_COUNT_OPCODE          0x07
#define GET_CURRENT_OPCODE              0x08
#define GET_AVERAGE_CURRENT_OPCODE      0x09
#define GET_REMAINING_CAP_OPCODE        0x0A
#define GET_AVERAGE_TIME_EMPTY_OPCODE   0x0B
#define GET_AVERAGE_TIME_FULL_OPCODE    0x0C

// Instructions prototypes

void GET_RSOC(SerialTalks& inst, Deserializer& input, Serializer& output);
void GET_ASOC(SerialTalks& inst, Deserializer& input, Serializer& output);
void GET_VOLTAGE(SerialTalks& inst, Deserializer& input, Serializer& output);
void GET_CYCLE_COUNT(SerialTalks& inst, Deserializer& input, Serializer& output);
void GET_CURRENT(SerialTalks& inst, Deserializer& input, Serializer& output);
void GET_AVERAGE_CURRENT(SerialTalks& inst, Deserializer& input, Serializer& output);
void GET_REMAINING_CAP(SerialTalks& inst, Deserializer& input, Serializer& output);
void GET_AVERAGE_TIME_EMPTY(SerialTalks& inst, Deserializer& input, Serializer& output);
void GET_AVERAGE_TIME_FULL(SerialTalks& inst, Deserializer& input, Serializer& output);


#endif //__INSTRUCTIONS_H__
