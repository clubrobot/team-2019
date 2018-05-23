#ifndef __PIN_H__
#define __PIN_H__

#include <Arduino.h>

// DC motors driver

#define LEFT_MOTOR_EN    6
#define LEFT_MOTOR_PWM   3
#define LEFT_MOTOR_DIR   5
#define RIGHT_MOTOR_EN   9
#define RIGHT_MOTOR_PWM 11
#define RIGHT_MOTOR_DIR 10
#define DRIVER_RESET    12
#define DRIVER_FAULT    A7

// Codewheels

#define QUAD_COUNTER_XY     A0
#define QUAD_COUNTER_SEL1    7
#define QUAD_COUNTER_SEL2    8
#define QUAD_COUNTER_OE     A5
#define QUAD_COUNTER_RST_X  A2
#define QUAD_COUNTER_RST_Y  A3
#define SHIFT_REG_DATA      13
#define SHIFT_REG_LATCH      4
#define SHIFT_REG_CLOCK      2

#define QUAD_COUNTER_X_AXIS  0 // Not pin
#define QUAD_COUNTER_Y_AXIS  1 // Not pin

#endif // __PIN_H__
