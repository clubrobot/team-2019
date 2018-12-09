#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../../common/SerialTalks.h"
#include "../../common/DCMotor.h"
#include "../../common/ShiftRegister.h"

#define _RESET_DRIVER_OPCODE   	0X11

#define _ENABLE_MOTOR1_OPCODE	0X12
#define _ENABLE_MOTOR2_OPCODE	0X13

#define _DISABLE_MOTOR1_OPCODE	0X14
#define _DISABLE_MOTOR2_OPCODE	0X15

#define _IS_ENABLED1_OPCODE		0X16
#define _IS_ENABLED2_OPCODE		0X17

#define _SET_VELOCITY1_OPCODE	0X18
#define _SET_VELOCITY2_OPCODE	0X19

void RESET_DRIVER(SerialTalks& talks, Deserializer& input, Serializer& output);

void ENABLE_MOTOR1(SerialTalks& talks, Deserializer& input, Serializer& output);

void ENABLE_MOTOR2(SerialTalks& talks, Deserializer& input, Serializer& output);

void DISABLE_MOTOR1(SerialTalks& talks, Deserializer& input, Serializer& output);

void DISABLE_MOTOR2(SerialTalks& talks, Deserializer& input, Serializer& output);

void IS_ENABLED1(SerialTalks& talks, Deserializer& input, Serializer& output);

void IS_ENABLED2(SerialTalks& talks, Deserializer& input, Serializer& output);

void SET_VELOCITY1(SerialTalks& talks, Deserializer& input, Serializer& output);

void SET_VELOCITY2(SerialTalks& talks, Deserializer& input, Serializer& output);


#endif //__INSTRUCTIONS_H__

