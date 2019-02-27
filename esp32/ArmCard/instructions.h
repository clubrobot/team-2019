#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/IK/Picker.h"
#include "../common/SerialTalks.h"
#include "../common/VacumPump.h"

#define ARM_BEGIN_OPCODE            0X10
#define ADD_MOVE_OPCODE             0x11
#define RUN_BATCH_OPCODE            0X12 
#define STOP_BATCH_OPCODE           0X13 
#define IS_ARRIVED_OPCODE           0x14

#define START_PUMP_OPCODE           0x15
#define STOP_PUMP_OPCODE            0X16 
#define START_SLUICE_OPCODE         0X17 
#define STOP_SLUICE_OPCODE          0x18

#define SET_MOTORS_ID_OPCODE        0X19
#define SET_MOTORS_OFFSET_OPCODE    0X1A
#define SET_WORKSPACE_OPCODE        0X1B
#define SET_ORIGIN_OPCODE           0X1C
#define SET_LINK_LEN_OPCODE         0X1D

void ARM_BEGIN(SerialTalks& talks, Deserializer& input, Serializer& output);

void ADD_MOVE(SerialTalks& talks, Deserializer& input, Serializer& output);

void RUN_BATCH(SerialTalks& talks, Deserializer& input, Serializer& output);

void STOP_BATCH(SerialTalks& talks, Deserializer& input, Serializer& output);

void IS_ARRIVED(SerialTalks& talks, Deserializer& input, Serializer& output);

void START_PUMP(SerialTalks& talks, Deserializer& input, Serializer& output);

void STOP_PUMP(SerialTalks& talks, Deserializer& input, Serializer& output);

void START_SLUICE(SerialTalks& talks, Deserializer& input, Serializer& output);

void STOP_SLUICE(SerialTalks& talks, Deserializer& input, Serializer& output);

void SET_MOTORS_ID(SerialTalks& talks, Deserializer& input, Serializer& output);

void SET_MOTORS_OFFSET(SerialTalks& talks, Deserializer& input, Serializer& output);

void SET_WORKSPACE(SerialTalks& talks, Deserializer& input, Serializer& output);

void SET_ORIGIN(SerialTalks& talks, Deserializer& input, Serializer& output);

void SET_LINK_LEN(SerialTalks& talks, Deserializer& input, Serializer& output);


#endif //__INSTRUCTIONS_H__

