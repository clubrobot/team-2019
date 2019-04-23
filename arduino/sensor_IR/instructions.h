#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "Sensor_base.h"
#include "Sensor_IR.h"
#include "Vect_Sensor.h"
#include "../common/SerialTalks.h"


// Opcodes declaration
#define GET_SINGLE_MESURE_OPCODE			0x10
#define GET_CONTINUOUS_MESURE_OPCODE   	    0x11
#define START_CONTINUOUS_OPCODE             0x12
#define STOP_CONTINUOUS_OPCODE              0x13

// Instructions prototypes
void GET_SINGLE_MESURE(SerialTalks& inst, Deserializer& input, Serializer& output);

void GET_CONTINUOUS_MESURE(SerialTalks& inst, Deserializer& input, Serializer& output);

void START_CONTINUOUS(SerialTalks& inst, Deserializer& input, Serializer& output);

void STOP_CONTINUOUS(SerialTalks& inst, Deserializer& input, Serializer& output);

#endif
