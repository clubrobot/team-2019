#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"
#include "../common/AX12/AX12.h"

#define ATTACH_OPCODE               0X10
#define DETACH_OPCODE               0X11
#define RESET_OPCODE                0X12

#define PING_AX_OPCODE              0X13

#define SET_ID_OPCODE               0X14
#define SET_BD_OPCODE               0X15

#define MOVE_OPCODE                 0X16
#define MOVE_SPEED_OPCODE           0X17

#define SET_ENDLESS_MODE_OPCODE     0X18

#define SET_TEMP_LIMIT_OPCODE       0X1C
#define SET_ANGLE_LIMIT_OPCODE      0X1D
#define SET_VOLTAGE_LIMIT_OPCODE    0X1E
#define SET_MAX_TORQUE_OPCODE       0X1F
#define SET_MAX_TORQUE_RAM_OPCODE   0X20
#define SET_SRL_OPCODE              0X21
#define SET_RDT_OPCODE              0X22
#define SET_LED_ALARM_OPCODE        0X23
#define SET_SUTDOWN_ALARM_OPCODE    0X24
#define SET_CMARGIN_OPCODE          0X25
#define SET_CSLOPE_OPCODE           0X26
#define SET_PUNCH_OPCODE            0X27

#define READ_TEMPERATURE_OPCODE     0X28
#define READ_VOLTAGE_OPCODE         0X29
#define READ_POSITION_OPCODE        0X2A
#define READ_SPEED_OPCODE           0X2B
#define READ_TORQUE_OPCODE          0X2C

void ATTACH(SerialTalks &inst, Deserializer &input, Serializer &output);
void DETACH(SerialTalks &inst, Deserializer &input, Serializer &output);
void RESET(SerialTalks &inst, Deserializer &input, Serializer &output);

void PING_AX(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_ID(SerialTalks &inst, Deserializer &input, Serializer &output);
void SET_BD(SerialTalks &inst, Deserializer &input, Serializer &output);

void MOVE(SerialTalks &inst, Deserializer &input, Serializer &output);
void MOVE_SPEED(SerialTalks &inst, Deserializer &input, Serializer &output);

void SET_ENDLESS_MODE(SerialTalks &inst, Deserializer &input, Serializer &output);		
void SET_TEMP_LIMIT(SerialTalks &inst, Deserializer &input, Serializer &output);
void SET_ANGLE_LIMIT(SerialTalks &inst, Deserializer &input, Serializer &output);
void SET_VOLTAGE_LIMIT(SerialTalks &inst, Deserializer &input, Serializer &output);
void SET_MAX_TORQUE(SerialTalks &inst, Deserializer &input, Serializer &output);
void SET_MAX_TORQUE_RAM(SerialTalks &inst, Deserializer &input, Serializer &output);
void SET_SRL(SerialTalks &inst, Deserializer &input, Serializer &output);
void SET_RDT(SerialTalks &inst, Deserializer &input, Serializer &output);
void SET_LED_ALARM(SerialTalks &inst, Deserializer &input, Serializer &output);
void SET_SUTDOWN_ALARM(SerialTalks &inst, Deserializer &input, Serializer &output);
void SET_CMARGIN(SerialTalks &inst, Deserializer &input, Serializer &output);
void SET_CSLOPE(SerialTalks &inst, Deserializer &input, Serializer &output);
void SET_PUNCH(SerialTalks &inst, Deserializer &input, Serializer &output);

void READ_TEMPERATURE(SerialTalks &inst, Deserializer &input, Serializer &output);
void READ_VOLTAGE(SerialTalks &inst, Deserializer &input, Serializer &output);
void READ_POSITION(SerialTalks &inst, Deserializer &input, Serializer &output);
void READ_SPEED(SerialTalks &inst, Deserializer &input, Serializer &output);
void READ_TORQUE(SerialTalks &inst, Deserializer &input, Serializer &output);

#endif //__INSTRUCTIONS_H__