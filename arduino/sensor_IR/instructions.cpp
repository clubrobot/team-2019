#include "instructions.h"
#include <Arduino.h>
#include <stdlib.h>
#include "VLSensors.h"

extern VLSensors sensors_IR;

void GET_DIST(SerialTalks& inst, Deserializer& input, Serializer& output){
    output.write<int>(sensors_IR.readRangeMillimeters());
}

void GET_STATE(SerialTalks& inst, Deserializer& input, Serializer& output){
    output.write<int>(sensors_IR.getState());   
}

void GET_VL53(SerialTalks& inst, Deserializer& input, Serializer& output){
    output.write<int>(sensors_IR.getVL53range());
}
void GET_VL61(SerialTalks& inst, Deserializer& input, Serializer& output){
    output.write<int>(sensors_IR.getVL61range());
}