#include "instructions.h"

// Global variables

// Montage : A relié à XSHUT au pin 14 | B relié à rien (excepté la commande I2C)
extern MyCapteur Sensor;
extern MoyenneGlissante MG;
extern int nb_echantillon_MG;

void GET_SINGLE_MESURE(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<float>((float) (Sensor.readRangeSingleMillimeters()));
}

void GET_CONTINUOUS_MESURE(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    if (nb_echantillon_MG>0) output.write<float>((float) (MG.getAverage()));
    else output.write<float>((float) (Sensor.readRangeContinuousMillimeters()));
}

void START_CONTINUOUS(SerialTalks& inst, Deserializer& input, Serializer& output){
    nb_echantillon_MG = (int)(input.read<int>());
    if (nb_echantillon_MG>0 && nb_echantillon_MG<256){
      uint16_t* echantillon = new uint16_t[ nb_echantillon_MG ];
      MG.reset((uint8_t)(nb_echantillon_MG),echantillon); // On réinitialise avec la bonne valeur
    }
    Sensor.startContinuous();
}

void STOP_CONTINUOUS(SerialTalks& inst, Deserializer& input, Serializer& output){
    nb_echantillon_MG = 0;
    MG.reset(0,NULL); // On réinitialise à 0
    Sensor.stopContinuous();
}
