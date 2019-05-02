#include <Wire.h>
#include "lib/Pololulu/VL53L0X.h"
#include <Arduino.h>
#include "PIN.h"
#include "instructions.h"

#include "../common/SerialTalks.h"

VL53L0X s1 = VL53L0X();
VL53L0X s2 = VL53L0X();

int16_t range_1 = 3000;
int16_t range_2 = 3000;
bool failedToBoot[] = {true, true};

void enableSensor(uint8_t shutpin, bool mode);

void talksExecuteWrapper(){
    talks.execute();
}

void setup() {

    Wire.begin();

    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);
    talks.bind(GET_RANGE_1_OPCODE, GET_RANGE_1);
    talks.bind(GET_RANGE_2_OPCODE, GET_RANGE_2);
    talks.bind(GET_BOTH_RANGE_OPCODE, GET_BOTH_RANGE);
    talks.bind(CHECK_ERROR_OPCODE, CHECK_ERROR);

    //Shutdown both VL61 as they are not used yet
    enableSensor(XSHUT_VL61_1, false);
    enableSensor(XSHUT_VL61_2, false);

    //Wake VL53 one at a time in order to change theirs addresses
    enableSensor(XSHUT_VL53_1, true);
    enableSensor(XSHUT_VL53_2, false);


    failedToBoot[0] = false;

    s1.setAddress(0x44);

    if (!s1.init()) {
        failedToBoot[0] = true;
    }

    //Same for the 2nd VL53
    enableSensor(XSHUT_VL53_2, true);
    s2.setAddress(0x45);

    failedToBoot[1] = false;
    if (!s2.init()) {
        failedToBoot[1] = true;
    }

    s1.setTimeout(30);
    s2.setTimeout(30);
    s1.startContinuous();
    s2.startContinuous();
}


void loop() {
  talks.execute();
  //Retrieve last measure and execute SerialTalks loop while waiting for new data
  range_1 = s1.readRangeContinuousMillimeters(talksExecuteWrapper);
  range_2 = s2.readRangeContinuousMillimeters(talksExecuteWrapper);
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
