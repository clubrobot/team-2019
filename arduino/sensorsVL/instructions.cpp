#include "instructions.h"

#define RANGE_TRESHOLD 100

// Global variables
extern uint16_t range_1;
extern uint16_t range_2;
extern uint16_t range_3;
extern uint16_t range_4;
extern bool failedToBoot[4];

void GET_RANGE_1(SerialTalks& inst, Deserializer& input, Serializer& output){
    uint16_t finalRange1 = (range_3 > RANGE_TRESHOLD) ? range_1 : range_3;
    output.write<uint16_t>(finalRange1);
}

void GET_RANGE_2(SerialTalks& inst, Deserializer& input, Serializer& output){
    uint16_t finalRange2 = (range_4 > RANGE_TRESHOLD) ? range_2 : range_4;
    output.write<uint16_t>(finalRange2);
}

void CHECK_ERROR(SerialTalks& inst, Deserializer& input, Serializer& output){
    output.write<bool>(failedToBoot[0]);
    output.write<bool>(failedToBoot[1]);
    output.write<bool>(failedToBoot[2]);
    output.write<bool>(failedToBoot[3]);
}

void GET_BOTH_RANGE(SerialTalks& inst, Deserializer& input, Serializer& output){
    uint16_t finalRange1 = (range_3 > RANGE_TRESHOLD) ? range_1 : range_3;
    output.write<uint16_t>(finalRange1);
    uint16_t finalRange2 = (range_4 > RANGE_TRESHOLD) ? range_2 : range_4;
    output.write<uint16_t>(finalRange2);
}
