#include "instructions.h"

// Global variables
extern int16_t range_1;
extern int16_t range_2;
extern bool failedToBoot[2];

void GET_RANGE_1(SerialTalks& inst, Deserializer& input, Serializer& output){
    output.write<int16_t>(range_1);
}

void GET_RANGE_2(SerialTalks& inst, Deserializer& input, Serializer& output){
    output.write<int16_t>(range_2);
}

void CHECK_ERROR(SerialTalks& inst, Deserializer& input, Serializer& output){
    output.write<bool>(failedToBoot[0]);
    output.write<bool>(failedToBoot[1]);
}

void GET_BOTH_RANGE(SerialTalks& inst, Deserializer& input, Serializer& output){
    output.write<int16_t>(range_1);
    output.write<int16_t>(range_2);
}
