#include "instructions.h"

#define RANGE_TRESHOLD 100

// Global variables
extern uint16_t range_1_med;
extern uint16_t range_2_med;
extern uint16_t range_3_med;
extern uint16_t range_4_med;
extern bool failedToBoot[4];

void GET_RANGE_1(SerialTalks& inst, Deserializer& input, Serializer& output){
    uint16_t finalRange1 = (range_3_med > RANGE_TRESHOLD) ? range_1_med : range_3_med;
    output.write<uint16_t>(finalRange1);
}

void GET_RANGE_2(SerialTalks& inst, Deserializer& input, Serializer& output){
    uint16_t finalRange2 = (range_4_med > RANGE_TRESHOLD) ? range_2_med : range_4_med;
    output.write<uint16_t>(finalRange2);
}

void CHECK_ERROR(SerialTalks& inst, Deserializer& input, Serializer& output){
    output.write<bool>(failedToBoot[0]);
    output.write<bool>(failedToBoot[1]);
    output.write<bool>(failedToBoot[2]);
    output.write<bool>(failedToBoot[3]);
}

void GET_BOTH_RANGE(SerialTalks& inst, Deserializer& input, Serializer& output){
    uint16_t finalRange1 = (range_3_med > RANGE_TRESHOLD) ? range_1_med : range_3_med;
    output.write<uint16_t>(finalRange1);
    uint16_t finalRange2 = (range_4_med > RANGE_TRESHOLD) ? range_2_med : range_4_med;
    output.write<uint16_t>(finalRange2);
}
