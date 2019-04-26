#include "../common/Wire.h"
#include "lib/Adafruit_VL53L0X.h"
#include <Arduino.h>
#include "PIN.h"
#include "instructions.h"

#include "../common/SerialTalks.h"

Adafruit_VL53L0X s1 = Adafruit_VL53L0X();
Adafruit_VL53L0X s2 = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t measure;
VL53L0X_RangingMeasurementData_t measure2;

int16_t range_1 = 3000;
int16_t range_2 = 3000;
bool failedToBoot[] = {true, true};

void enableSensor(uint8_t shutpin, bool mode);

void setup() {


  //Shutdown both VL61 as they are not used yet
  enableSensor(XSHUT_VL61_1, false);
  enableSensor(XSHUT_VL61_2, false);
  
  //Wake VL53 one at a time in order to change theirs addresses
  enableSensor(XSHUT_VL53_1, true);
  enableSensor(XSHUT_VL53_2, false);
  
  failedToBoot[0] = false;
  //Serial.println(F("Adafruit VL53L0X"));
  if (!s1.begin(0x44, false)) {
    //Serial.println(F("Failed to boot VL53L0X 1"));
    failedToBoot[0] = true;
  }

  //Same for the 2nd VL53
  enableSensor(XSHUT_VL53_2, true);
  
  failedToBoot[1] = false;
  if (!s2.begin(0x46, false)) {
    //Serial.println(F("Failed to boot VL53L0X 2"));
    failedToBoot[1] = true;
  }
  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);
  talks.bind(GET_RANGE_1_OPCODE, GET_RANGE_1);
  talks.bind(GET_RANGE_2_OPCODE, GET_RANGE_2);
  talks.bind(CHECK_ERROR_OPCODE, CHECK_ERROR);
}


void loop() {
  talks.execute();
     
  s1.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  
  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    //Serial.print(F("VL53L0X 1 - Distance (mm): ")); Serial.println(measure.RangeMilliMeter);
    range_1 = measure.RangeMilliMeter;
  } else {
    //Serial.println(F("VL53L0X 1 - out of range "));
    range_1 = 3000;
  }

  
  s2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
  if (measure2.RangeStatus != 4) {  // phase failures have incorrect data
    //Serial.print(F("VL53L0X 2 - Distance (mm): ")); Serial.println(measure2.RangeMilliMeter);
    range_2 = measure2.RangeMilliMeter;
  } else {
    //Serial.println(F("VL53L0X 2 - out of range "));
    range_2 = 3000;
  }
}

void enableSensor(uint8_t shutpin, bool mode)
{
  if(mode){
    pinMode(shutpin, INPUT);
    delay(2);               // Not ideal but we need to wait for the sensors to boot
  }
  else{
    pinMode(shutpin, OUTPUT);
    digitalWrite(shutpin, LOW);
  }
}