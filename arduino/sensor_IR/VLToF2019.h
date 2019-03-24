#ifndef __VLSENSORS_H__
#define __VLSENSORS_H__

#include <Arduino.h>
#include "Wire.h"
#include "PeriodicProcess.h"

class Vlsensors : public PeriodicProcess
{
  public:
  
    bool init();    
    void sendRequestAccel();
    void sendRequestMagnet();   

    bool  dataAvailable(){return _newData;}
    void  setDataOld(){_newData = false;}
    int getState(){return _state;}
  
    
  private : 

    void write8(byte address, byte reg, byte value);
    byte read8(byte address, byte reg);

    bool _newData;           // Tell if there is new data available to read

    enum{NEW_ACCEL_REQUEST, WAITING_ACCEL, NEW_MAGNET_REQUEST, WAITING_MAGNET} _state;

  protected:
	
	void process(float timestep);
	void onProcessEnabling();
};

#endif
