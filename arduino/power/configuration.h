#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <Arduino.h>


// Pin definition
#define EMERGENCY_BUTTON_PIN 2

#define LED1 9
#define LED2 10
#define LED3 11
#define LED4 12

#define CURRENT_PIN A0
#define VOLTAGE_PIN A1

// Constants definition

#define VOLTAGE_MAX 13
#define VOLTAGE_MAX_RANGE VOLTAGE_MAX/3
#define VOLTAGE_MAX_NORMALIZED VOLTAGE_MAX_RANGE/5*1023

#define VOLTAGE_MIN 10

#endif // __CONFIGURATION_H__
