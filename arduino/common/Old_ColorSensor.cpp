#include "ColorSensor.h"
#include <Arduino.h>
#include "../colorSensorTest/PIN.h"

int red = 0, blue = 0, green = 0;
int redvalue, bluevalue, greenvalue;

void ColorSensor::setup(){
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    digitalWrite(S0,HIGH);
    digitalWrite(S1,LOW);

}

int ColorSensor::getRed(){
    digitalWrite(S2, LOW);  
    digitalWrite(S3, LOW);  
    red = pulseIn(sensorOut, digitalRead(sensorOut) == HIGH ? LOW : HIGH); 
    return (red);
    //return (map(red, 25, 115, 255, 0));
}
  
int ColorSensor::getGreen(){
    
    digitalWrite(S3, HIGH);  
    green = pulseIn(sensorOut, digitalRead(sensorOut) == HIGH ? LOW : HIGH);
    return (green);
    //return(map(green, 30, 115, 255, 0));
}
  
int ColorSensor::getBlue(){
    
    digitalWrite(S2, HIGH);  
    blue = pulseIn(sensorOut, digitalRead(sensorOut) == HIGH ? LOW : HIGH);  
    return (blue);
    //return(map(blue, 25, 140, 255, 0));
}
