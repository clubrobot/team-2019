#ifndef __PIN_H__
#define __PIN_H__

#include <Arduino.h>

// DC motors driver

#define LEFT_MOTOR_EN    25
#define LEFT_MOTOR_PWM   32
#define LEFT_MOTOR_DIR   33
#define RIGHT_MOTOR_EN   14
#define RIGHT_MOTOR_PWM 26
#define RIGHT_MOTOR_DIR 27
#define DRIVER_RESET    13
#define DRIVER_FAULT    35
#define DRIVER_OTW      39
#define PWM_FREQUENCY 30000
  
//PWM Chanel

#define LEFT_MOTOR_CHANNEL  0
#define RIGHT_MOTOR_CHANNEL 1

// Codewheels

#define QUAD_COUNTER_XY     17
#define QUAD_COUNTER_SEL1    23
#define QUAD_COUNTER_SEL2    22
#define QUAD_COUNTER_OE     21
#define QUAD_COUNTER_RST_X  19
#define QUAD_COUNTER_RST_Y  18
#define SHIFT_REG_DATA      34
#define SHIFT_REG_LATCH      4
#define SHIFT_REG_CLOCK      16

#define QUAD_COUNTER_X_AXIS  0 // Not pin
#define QUAD_COUNTER_Y_AXIS  1 // Not pin

#endif // __PIN_H__
