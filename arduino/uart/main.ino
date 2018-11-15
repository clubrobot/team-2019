#include <Arduino.h>

void setup()
{
  Serial.begin(250000);
}

void loop()
{
    if (Serial.available()) 
    {
      char rx_byte = Serial.read();
      Serial.write(rx_byte);
    }
}