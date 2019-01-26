#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/SerialTalks.h"


// Opcodes declaration  
#define GET_ROBOT_ACC_OPCODE		    0x10
#define CALIBRATE_COMPAS_OPCODE         0x11
#define GET_ORIENTATION_OPCODE          0x12
#define GET_MEASURED_ORIENTATION_OPCODE 0x13
#define GET_SENSOR_VALUE_OPCODE         0x14
#define GET_SENSOR_STATE_OPCODE         0x15

// Instructions prototypes
void GET_ROBOT_ACC(SerialTalks& inst, Deserializer& input, Serializer& output);
void CALIBRATE_COMPAS(SerialTalks& inst, Deserializer& input, Serializer& output);
void GET_ORIENTATION(SerialTalks& inst, Deserializer& input, Serializer& output);
void GET_MEASURED_ORIENTATION(SerialTalks& inst, Deserializer& input, Serializer& output);
void GET_SENSOR_VALUE(SerialTalks& inst, Deserializer& input, Serializer& output);
void GET_SENSOR_STATE(SerialTalks& inst, Deserializer& input, Serializer& output);

#endif
