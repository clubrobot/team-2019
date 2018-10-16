#include "instructions.h"
#include "../common/UltrasonicSensor.h"
#include "../common/SensorListener.h"

// Global variables
extern UltrasonicSensor SensorAr;
extern UltrasonicSensor SensorAv;
extern SensorListener   ListenerAv;
extern SensorListener   ListenerAr;
extern bool activated; 

void GET_NORMAL(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    int delta = input.read<int>();
    output.write(ListenerAv.getStd(delta));
    output.write(ListenerAv.getVar(delta));
    output.write(ListenerAr.getStd(delta));
    output.write(ListenerAr.getVar(delta));
}


void GET_MESURE(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<int>(SensorAr.getMesure());
    output.write<int>(SensorAv.getMesure());
}

void ACTIVATE_SENSORS(SerialTalks& inst, Deserializer& input, Serializer& output){
    activated = true;
}

void DESACTIVATE_SENSORS(SerialTalks& inst, Deserializer& input, Serializer& output){
    activated = false;
}
