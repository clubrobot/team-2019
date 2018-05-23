#include <Arduino.h>

#include "../common/BrushlessMotor.h"
#include "../common/SerialTalks.h"
#include "instructions.h"
#include "PIN.h"
 

BrushlessMotor Motor;
float val ;

void setup() {
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    talks.bind(GET_VELOCITY_OPCODE, GET_VELOCITY);
    talks.bind(GET_CONSTANT_OPCODE, GET_CONSTANT);
    talks.bind(GET_WHEELRADIUS_OPCODE, GET_WHEELRADIUS);
    talks.bind(GET_MAX_VELOCITY_OPCODE, GET_MAX_VELOCITY);

    talks.bind(IS_ENABLED_OPCODE, IS_ENABLED);

    talks.bind(SET_VELOCITY_OPCODE, SET_VELOCITY);
    talks.bind(SET_CONSTANT_OPCODE, SET_CONSTANT);
    talks.bind(SET_WHEELRADIUS_OPCODE, SET_WHEELRADIUS);

    talks.bind(ENABLE_OPCODE, ENABLE);
    talks.bind(DISABLE_OPCODE, DISABLE);

    talks.bind(LOAD_OPCODE, LOAD);
    talks.bind(SAVE_OPCODE, SAVE);

    Motor.attach(ESC_PIN);

    Motor.setVelocity(MIN_VELOCITY); //in range [0,1];
}


void loop() {
  talks.execute();
}
