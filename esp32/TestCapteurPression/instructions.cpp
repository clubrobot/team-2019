#include "instructions.h"
#include "../common/PressureSensor.h"


extern PressureSensor pSensor;

void GET_PRESSURE(SerialTalks& talks, Deserializer& input, Serializer& output){
	output.write<int>(pSensor.getPressurekPa());
}

void ATMOSPHERE_PRESSURE(SerialTalks& talks, Deserializer& input, Serializer& output){
	output.write<bool>(pSensor.currentlyAtmospherePressure());
}
