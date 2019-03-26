#ifndef __VLSENSORS_H__
#define __VLSENSORS_H__

#include <Arduino.h>
#include "Wire.h"
#include "PeriodicProcess.h"

class VLSensors : public PeriodicProcess
{
  public:

    VLSensors(int EN_VL53_PIN, int EN_VL61_PIN);

    bool init();    
  
    int getState(){return _state;}

    bool useI2C(){return _i2cused;}

    void startContinuous(uint32_t period_ms = 0);
    void setVL53Address(uint8_t add);
    void setVL61Address(uint8_t add);
    uint16_t readRangeMillimeters(void);
    
  private : 

    void requestdata(uint8_t address, uint8_t reg, uint8_t len);
    
    bool readLen(uint8_t *buffer, uint8_t len);

    
    void write8(uint8_t address, uint8_t reg, uint8_t value);
    void write16(uint8_t address, uint8_t reg, uint16_t value);
    void write32(uint8_t address, uint8_t reg, uint32_t value);

    void whatToDoNext();
      
    bool _useSensors[2];             
    typedef enum{ REQUEST_VL53 = 0, WAITING_VL53 = 2,
                  REQUEST_VL61 = 1, WAITING_VL61 = 3,
                }State_t;
    
    State_t _state;

    uint16_t range[2];
    int threshold;

    
    uint8_t _addressVL53;
    uint8_t _addressVL61;

    int _EN_VL53;
    int _EN_VL61; 
    
    bool _VL53Error;
    bool _VL61Error;
    
    bool _i2cused;

  protected:
	
    void process(float timestep);
    void onProcessEnabling();
};

#endif
