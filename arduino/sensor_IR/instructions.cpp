#include "instructions.h"

// Global variables

// Montage : A relié à XSHUT au pin 14 | B relié à rien (excepté la commande I2C)
extern MyCapteur Sensor;
extern MoyenneGlissante MG;
extern int activation_MG;

void GET_SINGLE_MESURE(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<float>((float) (Sensor.readRangeSingleMillimeters()));
}

void GET_CONTINUOUS_MESURE(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    if (activation_MG==1) output.write<float>((float) (MG.getAverage()));
    else output.write<float>((float) (Sensor.readRangeContinuousMillimeters()));
}

void START_CONTINUOUS(SerialTalks& inst, Deserializer& input, Serializer& output){
    activation_MG = (int)(input.read<int>());
    Sensor.startContinuous();
}

void STOP_CONTINUOUS(SerialTalks& inst, Deserializer& input, Serializer& output){
    activation_MG = 0;
    Sensor.stopContinuous();
}
