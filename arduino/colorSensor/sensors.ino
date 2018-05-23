#include <Arduino.h>
//#include "configuration.h"
#include "instructions.h"
#include "../common/SerialTalks.h"
#include "../common/Adafruit_TCS34725.h"
#include "../common/Wire.h"


bool activated;

Adafruit_TCS34725 sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);
  talks.bind(GET_COLOR_OPCODE, GET_COLOR);
  talks.bind(ENABLE_LED_OPCODE, ENABLE_LED);
  talks.bind(DISABLE_LED_OPCODE, DISABLE_LED)
  sensor.begin();
  pinMode(12, OUTPUT);
}

void loop() {
  talks.execute();

}