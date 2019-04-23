#ifndef SENSOR_BASE_h
#define SENSOR_BASE_h

#include "Traitements.h"

class Sensor_base : public MoyenneGlissante
{
  public :
    Sensor_base(){};
    virtual void begin()=0;
    virtual void bind(void)=0;
    virtual void init(void)=0;
    virtual void configureDefault(void)=0;
    virtual void startContinuous(uint32_t period_ms =0)=0; // Par défaut : Aussi rapide que possible
    virtual uint16_t readRangeContinuousMillimeters(void)=0;
    virtual void stopContinuous(void)=0;
    virtual uint16_t readRangeSingleMillimeters(void)=0;
    virtual void setTimeout(uint16_t timeout)=0;
    virtual bool timeoutOccurred(void)=0;
    virtual void setNbEchantillonsMG(int nb){MoyenneGlissante::reset((uint8_t)nb);};
    virtual void update(){ if (getNbEchantillonsMG()>0) AddElement(readRangeContinuousMillimeters());};
};

#endif
