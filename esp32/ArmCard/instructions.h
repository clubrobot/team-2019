#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/IK/Picker.h"
#include "../common/SerialTalks.h"
#include "../common/VacumPump.h"

#define ADD_MOVE_OPCODE             0x10
#define RUN_BATCH_OPCODE            0X11 
#define STOP_BATCH_OPCODE           0X12 
#define IS_ARRIVED_OPCODE           0x13
#define GET_MOTORS_STATE_OPCODE     0x14

#define START_PUMP_OPCODE           0x15
#define STOP_PUMP_OPCODE            0X16 
#define START_SLUICE_OPCODE         0X17 
#define STOP_SLUICE_OPCODE          0x18
#define CLEAR_MOTOR_ERR_OPCODE      0x19

void ADD_MOVE(SerialTalks& talks, Deserializer& input, Serializer& output);

void RUN_BATCH(SerialTalks& talks, Deserializer& input, Serializer& output);

void STOP_BATCH(SerialTalks& talks, Deserializer& input, Serializer& output);

void IS_ARRIVED(SerialTalks& talks, Deserializer& input, Serializer& output);

void GET_MOTORS_STATE(SerialTalks& talks, Deserializer& input, Serializer& output);

void START_PUMP(SerialTalks& talks, Deserializer& input, Serializer& output);

void STOP_PUMP(SerialTalks& talks, Deserializer& input, Serializer& output);

void START_SLUICE(SerialTalks& talks, Deserializer& input, Serializer& output);

void STOP_SLUICE(SerialTalks& talks, Deserializer& input, Serializer& output);

void CLEAR_MOTOR_ERR(SerialTalks& talks, Deserializer& input, Serializer& output);

#endif //__INSTRUCTIONS_H__

