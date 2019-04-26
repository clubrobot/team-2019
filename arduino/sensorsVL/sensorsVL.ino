#include "Adafruit_VL53L0X.h"
#include "Adafruit_VL6180X.h"

#define XSHUT_VL53_2 3 //A7
#define XSHUT_VL53_1 10
#define XSHUT_VL61_2 4 //A6
#define XSHUT_VL61_1 11

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL6180X vl = Adafruit_VL6180X();
Adafruit_VL6180X vl2 = Adafruit_VL6180X();

long current,old = 0;

void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  pinMode(XSHUT_VL61_2, OUTPUT);
  digitalWrite(XSHUT_VL61_2, LOW);
  pinMode(XSHUT_VL61_1, OUTPUT);
  digitalWrite(XSHUT_VL61_1, LOW);

  //pinMode(XSHUT_VL53_1,OUTPUT);
  //digitalWrite(XSHUT_VL53_1, LOW);
  pinMode(XSHUT_VL53_2,OUTPUT);
  digitalWrite(XSHUT_VL53_2, LOW);
  
  delay(5);

  Serial.println(F("Adafruit VL53L0X"));
  if (!lox.begin(0x44, true)) {
    Serial.println(F("Failed to boot VL53L0X 1"));
    //while(1);
  }
  pinMode(XSHUT_VL53_2,INPUT);

  delay(5);

  if (!lox2.begin(0x46, true)) {
    Serial.println(F("Failed to boot VL53L0X 2"));
    //while(1);
  }

  /*pinMode(XSHUT_VL61_1,INPUT);

  delay(5);

  Serial.println(F("Adafruit VL6180X"));
  if (! vl.begin()) {
    Serial.println(F("Failed to boot VL6180X 1"));
    //while (1);
  }
  vl.setAddress(0x48);

  pinMode(XSHUT_VL61_2,INPUT);*/
  
  /*delay(5);

  if (! vl2.begin()) {
    Serial.println(F("Failed to boot VL6180X 2"));
    //while (1);
  }
  vl2.setAddress(0x49);*/

}


void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  VL53L0X_RangingMeasurementData_t measure2;

    
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  //delay(100);
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
  current = millis();
  Serial.print("Time : ");Serial.println(current-old);Serial.println();
  old = current;
  
  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print(F("VL53L0X 1 - Distance (mm): ")); Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(F("VL53L0X 1 - out of range "));
  }

  if (measure2.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print(F("VL53L0X 2 - Distance (mm): ")); Serial.println(measure2.RangeMilliMeter);
  } else {
    Serial.println(F("VL53L0X 2 - out of range "));
  }
  /*delay(100);

  uint8_t range = vl.readRange();
  uint8_t status = vl.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    Serial.print(F("VL6180X 1 - Range : ")); Serial.println(range);
  }
  delay(100);

  // Some error occurred, print it out!
  
  if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
    Serial.println(F("VL6180X 1 - System error"));
  }
  else if (status == VL6180X_ERROR_ECEFAIL) {
    Serial.println(F("VL6180X 1 - ECE failure"));
  }
  else if (status == VL6180X_ERROR_NOCONVERGE) {
    Serial.println(F("VL6180X 1 - No convergence"));
  }
  else if (status == VL6180X_ERROR_RANGEIGNORE) {
    Serial.println(F("VL6180X 1 - Ignoring range"));
  }
  else if (status == VL6180X_ERROR_SNR) {
    Serial.println(F("VL6180X 1 - Signal/Noise error"));
  }
  else if (status == VL6180X_ERROR_RAWUFLOW) {
    Serial.println(F("VL6180X 1 - Raw reading underflow"));
  }
  else if (status == VL6180X_ERROR_RAWOFLOW) {
    Serial.println(F("VL6180X 1 - Raw reading overflow"));
  }
  else if (status == VL6180X_ERROR_RANGEUFLOW) {
    Serial.println(F("VL6180X 1 - Range reading underflow"));
  }
  else if (status == VL6180X_ERROR_RANGEOFLOW) {
    Serial.println(F("VL6180X 1 - Range reading overflow"));
  }*/
  /*range = vl2.readRange();
  status = vl2.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    Serial.print(F("VL6180X 2 - Range : ")); Serial.println(range);
  }

  // Some error occurred, print it out!
  
  if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
    Serial.println(F("VL6180X 2 - System error"));
  }
  else if (status == VL6180X_ERROR_ECEFAIL) {
    Serial.println(F("VL6180X 2 - ECE failure"));
  }
  else if (status == VL6180X_ERROR_NOCONVERGE) {
    Serial.println(F("VL6180X 2 - No convergence"));
  }
  else if (status == VL6180X_ERROR_RANGEIGNORE) {
    Serial.println(F("VL6180X 2 - Ignoring range"));
  }
  else if (status == VL6180X_ERROR_SNR) {
    Serial.println(F("VL6180X 2 - Signal/Noise error"));
  }
  else if (status == VL6180X_ERROR_RAWUFLOW) {
    Serial.println(F("VL6180X 2 - Raw reading underflow"));
  }
  else if (status == VL6180X_ERROR_RAWOFLOW) {
    Serial.println(F("VL6180X 2 - Raw reading overflow"));
  }
  else if (status == VL6180X_ERROR_RANGEUFLOW) {
    Serial.println(F("VL6180X 2 - Range reading underflow"));
  }
  else if (status == VL6180X_ERROR_RANGEOFLOW) {
    Serial.println(F("VL6180X 2 - Range reading overflow"));
  }*/
  //delay(100);
}
