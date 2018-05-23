#include "instructions.h"
#include <Arduino.h>
#include <stdlib.h>
#include <math.h>
#include "../common/ButtonCard.h"


extern ButtonCard buttonCard;

void ENABLE_LED(SerialTalks& inst, Deserializer& input, Serializer& output){
    buttonCard.setLed(input.read<int>(), 1);
}

void DISABLE_LED(SerialTalks& inst, Deserializer& input, Serializer& output){
    buttonCard.setLed(input.read<int>(), 0);
}