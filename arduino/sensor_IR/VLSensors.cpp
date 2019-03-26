#include "VLSensors.h"

VLSensors::VLSensors(int EN_VL53_PIN, int EN_VL61_PIN){
    _EN_VL53 = EN_VL53_PIN;
    _EN_VL61 = EN_VL61_PIN;
    for(int i = 0; i< 2; i++){_useSensors[i] = true;}
    _state = REQUEST_VL53;
}

void VLSensors::whatToDoNext(){ 
  int j = 0;
  int i = _state - 2;
  while(!_useSensors[i] && j<2){
    j++;
    i = (i + 1) % 2;
  }
  _state = static_cast<VLSensors::State_t>(i);
  if(j>=2){
    disable();
  }
}

void VLSensors::process(float timestep){
  switch(_state){
    case REQUEST_VL53 :
      requestData();
      _i2cused = true;
      _state = WAITING_ACCEL;
      break;

    case WAITING_VL53 :
      uint8_t buf[2];
      if(readLen(buf, 2)){
        range[1] = (uint16_t) buf[1] | (uint16_t) buf[2] << 8;
        whatToDoNext();
        _i2cused = false;
      }
      break;

    case REQUEST_VL61:
      _i2cused = true;
      requestData();
      _state = WAITING_MAGNET;
      break;

    case WAITING_VL61:
      uint8_t buf[2];
      if(readLen(buf, 2)){
        range[2] = (uint16_t) buf[1] | (uint16_t) buf[2] << 8;
        whatToDoNext();
        _i2cused = false;
      }
      break;
  }
}