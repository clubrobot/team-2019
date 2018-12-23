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
void CALIBRATE_COMPAS(SerialTalks& inst, Deserializer& input, Serializer& output){
    cmp.setOrientation(input.read<int>(), acmaSensor.getMagX(), acmaSensor.getMagY());
}

void GET_ORIENTATION(SerialTalks& inst, Deserializer& input, Serializer& output){
    output.write<int>(acc.getOrientation());
}
void GET_MEASURED_ORIENTATION(SerialTalks& inst, Deserializer& input, Serializer& output){
    int mag_x = acmaSensor.getMagX();
    int mag_y = acmaSensor.getMagY();
    int o =  atan2(mag_y, mag_x) * 180 / M_PI;
    output.write<int>(o);
}

void GET_SENSOR_VALUE(SerialTalks& inst, Deserializer& input, Serializer& output){
    output.write<int>(acmaSensor.getMagX());
    output.write<int>(acmaSensor.getMagY());
    output.write<int>(acmaSensor.getMagZ());
    output.write<int>(acmaSensor.getAccX());
    output.write<int>(acmaSensor.getAccY());
    output.write<int>(acmaSensor.getAccZ());
}

void GET_SENSOR_STATE(SerialTalks& inst, Deserializer& input, Serializer& output){
    output.write<int>(acmaSensor.getState());
}