#include "instructions.h"
#include <Arduino.h>
#include <stdlib.h>
#include "AccelerationController.h"


// Global variables
extern AccelerationController acc;
extern MagneticCompas cmp;
extern Lsm303 acmaSensor;

void GET_ROBOT_ACC(SerialTalks& inst, Deserializer& input, Serializer& output){
    output.write<int>(acc.getMeanAcceleration_X());
    output.write<int>(acc.getMeanAcceleration_Y());
    acc.resetMean();
}