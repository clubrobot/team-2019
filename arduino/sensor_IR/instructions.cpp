#include "instructions.h"
#include <Arduino.h>
#include <stdlib.h>
#include "VLSensors.h"

extern VLSensors sensors_IR;

void GET_DIST(SerialTalks& inst, Deserializer& input, Serializer& output){
    input.write<int>(sensors_IR.readRangeMillimeters())
}