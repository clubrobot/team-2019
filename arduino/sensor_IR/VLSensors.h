#ifndef __VLSENSORS_H__
#define __VLSENSORS_H__

#define VL53 0
#define VL61 1


#include <Arduino.h>
#include "../common/Wire.h"
#include "../common/PeriodicProcess.h"
#include "VL53L0X.h"
#include "VL6180X.h"

class VLSensors : public PeriodicProcess
{
  public:

    VLSensors(int EN_VL53_PIN, int EN_VL61_PIN);

    void init();    
  
    int getState(){return _state;}
    void setThreshold(uint16_t thresh){_threshold = thresh;}
    uint16_t getThreshold(){return _threshold;}

    bool useI2C(){return _i2cused;}

    void setVL53Address(uint8_t add);
    void setVL61Address(uint8_t add);

    uint16_t getVL53range(){return _range[VL53];}
    uint16_t getVL61range(){return _range[VL61];}

    uint16_t readRangeMillimeters(void);

    void shutBothVL();
    void startBothVL();

  protected:
	
    void process(float timestep);
    void onProcessEnabling(){startBothVL();}
  
  private: 
    void requestDataVL53(uint8_t reg, uint8_t len);
    void requestDataVL61(uint16_t reg, uint8_t len);

    void startRangingVL53();

    bool readLen(uint8_t *buffer, uint8_t len);
    uint8_t read8VL53(uint8_t reg);

    
    void write8VL53 (uint8_t reg, uint8_t value);
    void write16VL53(uint8_t reg, uint16_t value);
    void write32VL53(uint8_t reg, uint32_t value);

    void write8VL61 (uint16_t reg, uint8_t value);
    void write16VL61(uint16_t reg, uint16_t value);
    void write32VL61(uint16_t reg, uint32_t value);    
    
    bool timeout();

    void whatToDoNext();
      
    bool _useSensors[2];             
    typedef enum{ REQUEST_VL53 = 0, WAITING_VL53 = 2,
                  REQUEST_VL61 = 1, WAITING_VL61 = 3,
                  MESURING_VL53 = 4
                }State_t;
    
    State_t _state;

    uint16_t _range[2];
    uint16_t _threshold;

    
    uint8_t _addressVL53;
    uint8_t _addressVL61;

    int _EN_VL53;
    int _EN_VL61; 
    
    bool _VL53Error;
    bool _VL61Error;
    
    bool _i2cused;

    uint8_t stop_variable;

  Clock _timeoutchecker;
    

};

#endif
