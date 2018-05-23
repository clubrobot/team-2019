#include <Arduino.h>
#include "instructions.h"
#include "../common/SerialTalks.h"
#include "PIN.h"
#include "../common/ButtonCard.h"

ButtonCard buttonCard;
int oldreadmode = 1;
long timeInverter, timeButton, timeTirette, savedTime = 0;
int oldemergency = 0;

void setup() {
  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);
  //talks.attach(10,button);
  buttonCard.setup();
  talks.bind(ENABLE_LED_OPCODE, ENABLE_LED);
  talks.bind(DISABLE_LED_OPCODE, DISABLE_LED);
}

void loop() {
  talks.execute();
  for (int i = 0; i < 5; i++){
    if (buttonCard.readButton(i) == LOW && millis()>timeButton+500){
      Serializer button = talks.getSerializer();
         button.write<byte>(i);
         talks.send(1, button);
         timeButton = millis();
     }
}/*
if (buttonCard.readEmergency() == LOW){
  Serializer emergency = talks.getSerializer();
    oldreadmode = buttonCard.readMode();
    emergency.write<byte>(1);
    talks.send(2, emergency);
}*/
if (buttonCard.readTirette() == LOW && millis() > timeTirette + 500){
  Serializer tirette = talks.getSerializer();

    tirette.write<byte>(1);
    talks.send(3, tirette);
    timeTirette = millis();
}
/*
if (buttonCard.readMode() != oldreadmode && millis()>timeInverter+10){
  Serializer mode = talks.getSerializer();
    mode.write<byte>(buttonCard.readMode());
    oldreadmode = buttonCard.readMode();
    talks.send(4, mode);
    timeInverter = millis();
  }*/
if(analogRead(EMERGENCY)<710){
    if(millis() - savedTime > 250 && millis() - savedTime < 500){
      tone(ALED_3, 1500);
    } else if(millis() - savedTime > 500){
      noTone(ALED_3);
      savedTime = millis();
    }
}


}
