#include "ButtonCard.h"
#include <Arduino.h>
#include "../buttonCard/PIN.h"

int ledRemap[7]={LED_1, LED_2, LED_3, LED_4, ALED_1, ALED_2, ALED_3};
int buttonRemap[4]={BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4};

void ButtonCard::setup(){
  pinMode(BUTTON_1, INPUT);
  digitalWrite(BUTTON_1, HIGH);
  pinMode(BUTTON_2, INPUT);
  digitalWrite(BUTTON_2, HIGH);
  pinMode(BUTTON_3, INPUT);
  digitalWrite(BUTTON_3, HIGH);
  pinMode(BUTTON_4, INPUT);
  digitalWrite(BUTTON_4, HIGH);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(TIRETTE, INPUT);
  digitalWrite(TIRETTE, HIGH);
  pinMode(INVERTER_DEV, INPUT);
  digitalWrite(INVERTER_DEV, HIGH);
  pinMode(INVERTER_PLAY, INPUT);
  digitalWrite(INVERTER_PLAY, HIGH);
  pinMode(EMERGENCY, INPUT);
  pinMode(ALED_1, OUTPUT);
  pinMode(ALED_2, OUTPUT);
  pinMode(ALED_3, OUTPUT);

}

void ButtonCard::setLed(int no, bool value){
    if (value == 0){
        digitalWrite(ledRemap[no-1], LOW);
    }
    if (value == 1){
        digitalWrite(ledRemap[no-1], HIGH);
    }
}

bool ButtonCard::readButton(int no){
    return(digitalRead(buttonRemap[no]));
}

bool ButtonCard::readTirette(){
    return(digitalRead(TIRETTE));
}

bool ButtonCard::readEmergency(){
    return(digitalRead(EMERGENCY));
}

bool ButtonCard::readMode(){
    if (digitalRead(INVERTER_PLAY) == HIGH){
        return 0;
    }
    if (digitalRead(INVERTER_DEV) == HIGH){
        return 1;
    }
}