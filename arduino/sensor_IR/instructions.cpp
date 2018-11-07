#include "instructions.h"

// Global variables

// Montage : A relié à XSHUT au pin 14 | B relié à rien (excepté la commande I2C)
//extern MyCapteur Sensor1;
extern Vect_Sensor<MyCapteur*> Sensors;

void GET_SINGLE_MESURE(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<String>((Sensors.readRangeSingleMillimeters()));
}

void GET_CONTINUOUS_MESURE(SerialTalks& inst, Deserializer& input, Serializer& output)
{
    output.write<String>(Sensors.readRangeContinuousMillimeters()); // contient l'ancien readRange et le Average 
}

void START_CONTINUOUS(SerialTalks& inst, Deserializer& input, Serializer& output){
    int nb_echantillon_MG = (int)(input.read<int>());
    if (nb_echantillon_MG>0 && nb_echantillon_MG<256){
      Sensors.setNbEchantillonsMG(nb_echantillon_MG); // On réinitialise avec la bonne valeur
    }
    Sensors.startContinuous();
}

void STOP_CONTINUOUS(SerialTalks& inst, Deserializer& input, Serializer& output){
    Sensors.setNbEchantillonsMG(0);
    Sensors.stopContinuous();
}
