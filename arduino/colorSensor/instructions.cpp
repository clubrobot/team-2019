#include "instructions.h"
#include <Arduino.h>
#include "../common/Adafruit_TCS34725.h"
#include <stdlib.h>
#include <math.h>
#include "../common/Wire.h"

// Global variables
byte gammatable[256];
extern Adafruit_TCS34725 sensor;
extern bool activated; 
uint16_t redada, greenada, blueada, clearada;
//tcs.begin();

void GET_COLOR(SerialTalks& inst, Deserializer& input, Serializer& output)
{  
    sensor.setInterrupt(false);
    sensor.getRawData(&redada, &greenada, &blueada, &clearada);
    output.write<int>(redada);
    output.write<int>(greenada);
    output.write<int>(blueada);

}

void ENABLE_LED(SerialTalks& inst, Deserializer& input, Serializer& output){
        activated = input.read<bool>();
        digitalWrite(12, HIGH);
}

void DISABLE_LED(SerialTalks& inst, Deserializer& input, Serializer& output){
    activated = input.read<bool>();
    digitalWrite(12, LOW);
}

