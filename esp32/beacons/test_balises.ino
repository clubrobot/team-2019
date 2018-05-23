#include <Arduino.h>
#include "pin.h"
#include <SPI.h>
#include "../../../arduino-dw1000/src/DW1000.h"


void setup() {
  pinMode(PIN_VBAT,INPUT);
  pinMode(PIN_BUTTON,INPUT);
  pinMode(PIN_ENABLE_KEY,OUTPUT);
  pinMode(PIN_LED_OK,OUTPUT);
  pinMode(PIN_LED_FAIL,OUTPUT);
  pinMode(PIN_UWB_RST,OUTPUT);
  pinMode(PIN_IRQ,INPUT);
  pinMode(PIN_ESYNCHRO,INPUT);
  pinMode(PIN_SYNCHRO,INPUT);

  // DEBUG monitoring
  Serial.begin(9600);
  // initialize the driver
  DW1000.begin(PIN_IRQ, PIN_SPICLK, PIN_SPIMISO, PIN_SPIMOSI, PIN_UWB_RST);
  DW1000.select(PIN_SPICSN);
  Serial.println(F("DW1000 initialized ..."));
  // general configuration
  DW1000.newConfiguration();
  DW1000.setDeviceAddress(5);
  DW1000.setNetworkId(10);
  DW1000.commitConfiguration();
  Serial.println(F("Committed configuration ..."));
  // wait a bit
  delay(1000);

}

void loop() {
  //talks.execute();
  digitalWrite(PIN_LED_OK,HIGH);
  digitalWrite(PIN_LED_FAIL,LOW);
  delay(500);
  digitalWrite(PIN_LED_OK,LOW);
  digitalWrite(PIN_LED_FAIL,HIGH);
  delay(500);
  // DEBUG chip info and registers pretty printed
  char msg[128];
  DW1000.getPrintableDeviceIdentifier(msg);
  Serial.print("Device ID: ");
  Serial.println(msg);
  DW1000.getPrintableExtendedUniqueIdentifier(msg);
  Serial.print("Unique ID: ");
  Serial.println(msg);
  DW1000.getPrintableNetworkIdAndShortAddress(msg);
  Serial.print("Network ID & Device Address: ");
  Serial.println(msg);
  DW1000.getPrintableDeviceMode(msg);
  Serial.print("Device mode: ");
  Serial.println(msg);
  // wait a bit
  delay(1000);
}
