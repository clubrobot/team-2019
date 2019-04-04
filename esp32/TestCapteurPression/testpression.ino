#include <Arduino.h>
#include "instructions.h"
#include "../common/SerialTalks.h"
#include "../common/PressureSensor.h"

#define ANALOG_PIN 12

PressureSensor pSensor = PressureSensor(ANALOG_PIN);

void setup()
{

    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    talks.bind(GET_PRESSURE_OPCODE, GET_PRESSURE);
    talks.bind(ATMOSPHERE_PRESSURE_OPCODE, ATMOSPHERE_PRESSURE);

}

void loop()
{
    talks.execute();
}
