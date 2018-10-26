#include "EndStop.h"
#include <Arduino.h>


void EndStop::attach(int pin){
    m_pin = pin;
    pinMode(m_pin, INPUT_PULLUP);
}

bool EndStop::getState(){
 return (digitalRead(m_pin) == LOW);
}

void EndStop::detach(){
     pinMode(m_pin, INPUT);
    m_pin = 0;
    
}