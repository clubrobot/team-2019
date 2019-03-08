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

#define MOTOR1_ID_ID                0x10
#define MOTOR1_OFFSET_ID            0x11
#define MOTOR2_ID_ID                0x12
#define MOTOR2_OFFSET_ID            0x13
#define MOTOR3_ID_ID                0x14
#define MOTOR3_OFFSET_ID            0x15
#define WORKSPACE_FRONT_ID          0x16
#define WORKSPACE_BACK_ID           0x17
#define ORIGIN_ID                   0x18
#define JOINTS_ID                   0x19
#define LINK_1_ID                   0x1A
#define LINK_2_ID                   0x1B
#define LINK_3_ID                   0x1C
#define ELBOW_ID                    0x1D
#define TIMESTEP_ID                 0x1E

void ADD_MOVE(SerialTalks& talks, Deserializer& input, Serializer& output);

void RUN_BATCH(SerialTalks& talks, Deserializer& input, Serializer& output);

void STOP_BATCH(SerialTalks& talks, Deserializer& input, Serializer& output);

void IS_ARRIVED(SerialTalks& talks, Deserializer& input, Serializer& output);

void START_PUMP(SerialTalks& talks, Deserializer& input, Serializer& output);

void STOP_PUMP(SerialTalks& talks, Deserializer& input, Serializer& output);

void START_SLUICE(SerialTalks& talks, Deserializer& input, Serializer& output);

void STOP_SLUICE(SerialTalks& talks, Deserializer& input, Serializer& output);

void SET_PARAMETER_VALUE(SerialTalks& talks, Deserializer& input, Serializer& output);

void GET_PARAMETER_VALUE(SerialTalks& talks, Deserializer& input, Serializer& output);

#endif //__INSTRUCTIONS_H__

