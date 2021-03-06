#include <Wire.h>
#include "lib/Pololulu/VL53L0X/VL53L0X.h"
#include "lib/Pololulu/VL6180X/VL6180X.h"
#include <Arduino.h>
#include "PIN.h"
#include "instructions.h"

#include "../common/SerialTalks.h"

VL53L0X s1 = VL53L0X();
VL53L0X s2 = VL53L0X();

VL6180X s3 = VL6180X();
VL6180X s4 = VL6180X();

uint16_t range_1[3] = {3000, 3000, 3000};
uint16_t range_2[3] = {3000, 3000, 3000};
uint16_t range_3[3] = {255, 255, 255};
uint16_t range_4[3] = {255, 255, 255};
uint16_t range_1_med = 3000;
uint16_t range_2_med = 3000;
uint16_t range_3_med = 255;
uint16_t range_4_med = 255;
uint8_t tabPos = 0;

bool failedToBoot[] = {true, true, true, true};

void enableSensor(uint8_t shutpin, bool mode);

void talksExecuteWrapper(){
    talks.execute();
}

uint16_t median3(uint16_t a, uint16_t b, uint16_t c)
{
    uint16_t middle;

    if ((a <= b) && (a <= c))
    {
        middle = (b <= c) ? b : c;
    }
    else if ((b <= a) && (b <= c))
    {
        middle = (a <= c) ? a : c;
    }
    else
    {
        middle = (a <= b) ? a : b;
    }
    return middle;
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
    enableSensor(XSHUT_VL53_2, false);

    //Init 1st VL53
    enableSensor(XSHUT_VL53_1, true);
    s1.setAddress(0x44);

    failedToBoot[0] = false;
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

    //Same for the 1st VL61
    enableSensor(XSHUT_VL61_1, true);
    s3.setAddress(0x46);

    failedToBoot[2] = false;
    s3.init();


    //Same for the 2nd VL61
    enableSensor(XSHUT_VL61_2, true);
    s4.setAddress(0x48);

    failedToBoot[3] = false;
    s4.init();

    s1.setTimeout(30);
    s2.setTimeout(30);
    s3.setTimeout(30);
    s4.setTimeout(30);

    s3.configureDefault();
    s4.configureDefault();

    s3.setScaling(1);
    s4.setScaling(1);

    s1.startContinuous();
    s2.startContinuous();
    //Set the period but not too low
    s3.startRangeContinuous(60);
    s4.startRangeContinuous(60);

    for(int i = 0; i<3; i++){
        range_1[i] = s1.readRangeContinuousMillimeters(talksExecuteWrapper);
        range_2[i] = s2.readRangeContinuousMillimeters(talksExecuteWrapper);
        range_3[i] = s3.readRangeContinuousMillimeters(talksExecuteWrapper);
        range_4[i] = s4.readRangeContinuousMillimeters(talksExecuteWrapper);
    }
    range_1_med = median3(range_1[0], range_1[1], range_1[2]);
    range_2_med = median3(range_2[0], range_2[1], range_2[2]);
    range_3_med = median3(range_3[0], range_3[1], range_3[2]);
    range_4_med = median3(range_4[0], range_4[1], range_4[2]);
}


void loop() {
  talks.execute();
  //Retrieve last measure and execute SerialTalks loop while waiting for new data
  range_1[tabPos] = s1.readRangeContinuousMillimeters(talksExecuteWrapper);
  range_2[tabPos] = s2.readRangeContinuousMillimeters(talksExecuteWrapper);
  range_3[tabPos] = s3.readRangeContinuousMillimeters(talksExecuteWrapper);
  range_4[tabPos] = s4.readRangeContinuousMillimeters(talksExecuteWrapper);
  tabPos = (tabPos + 1) % 3;
  range_1_med = median3(range_1[0], range_1[1], range_1[2]);
  range_2_med = median3(range_2[0], range_2[1], range_2[2]);
  range_3_med = median3(range_3[0], range_3[1], range_3[2]);
  range_4_med = median3(range_4[0], range_4[1], range_4[2]);
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
