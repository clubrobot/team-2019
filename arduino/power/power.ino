#include <Arduino.h>
#include "configuration.h"
#include "instructions.h"
#include "../common/SerialTalks.h"

float voltage = 0;
int percentage = 0;

void getVoltage();

void setup(){
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    talks.bind(_GET_EMERGENCY_STOP_STATE_OPCODE, GET_EMERGENCY_STOP_STATE);
    talks.bind(_GET_VOLTAGE_OPCODE, GET_VOLTAGE);
    talks.bind(_GET_BATTERY_CHARGE_OPCODE, GET_BATTERY_CHARGE);

	pinMode(CURRENT_PIN,INPUT);
	pinMode(VOLTAGE_PIN,INPUT);
	pinMode(EMERGENCY_BUTTON_PIN,INPUT);
	pinMode(LED1,OUTPUT);
	pinMode(LED2,OUTPUT);
	pinMode(LED3,OUTPUT);
	pinMode(LED4,OUTPUT);  

	getVoltage();
    
}

void loop(){
     talks.execute();   
	 getVoltage();
}

void getVoltage(){
	voltage = analogRead(VOLTAGE_PIN);
	voltage = (voltage / VOLTAGE_MAX_NORMALIZED)*VOLTAGE_MAX;
	percentage = (voltage - VOLTAGE_MIN) *100 / (VOLTAGE_MAX - VOLTAGE_MIN);
}



