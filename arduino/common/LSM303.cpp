#include "Wire.h"

#include <limits.h>

#include "LSM303.h"
#include <Arduino.h>

static float _lsm303Accel_MG_LSB     = 0.001F;   // 1, 2, 4 or 12 mg per lsb
static float _lsm303Mag_Gauss_LSB_XY = 1100.0F;  // Varies with gain
static float _lsm303Mag_Gauss_LSB_Z  = 980.0F;   // Varies with gain

/***************************************************************************
 CONSTRUCTOR
 ***************************************************************************/
bool Lsm303::begin()
{
  Wire.begin();

  // Enable the accelerometer
  write8(LSM303_ADDRESS_ACCEL, LSM303_REGISTER_ACCEL_CTRL_REG1_A, 0x27);

  // Enable the magnetometer
  write8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_MR_REG_M, 0x00);

  _accelData = {0,0,0};
  _magData = {0,0,0};

  return true;
}


void Lsm303::process(float timestep){ 
  switch(_state){
    case NEW_ACCEL_REQUEST:
        sendRequestAccel();
        _newData = true;
        _state = WAITING_ACCEL;
        break;
    case WAITING_ACCEL:
        if(readAccel())
          _state = NEW_MAGNET_REQUEST;
        break;
    case NEW_MAGNET_REQUEST:
        sendRequestMagnet();
        _state = WAITING_MAGNET;
        break;
    case WAITING_MAGNET:
        if(readMagnet());
        _state = NEW_ACCEL_REQUEST;
        break;
    default: 
        _newData = false;
        break;
  }
}
void Lsm303::onProcessEnabling(){
    begin();
    _state = NEW_ACCEL_REQUEST;
}

void Lsm303::sendRequestAccel()
{
  // Read the accelerometer
  Wire.beginTransmission((byte) LSM303_ADDRESS_ACCEL);
  Wire.write(LSM303_REGISTER_ACCEL_OUT_X_L_A | 0x80);
  Wire.endTransmission();
  Wire.requestFrom((byte)LSM303_ADDRESS_ACCEL, (byte)6);
  
}

bool Lsm303::readAccel(){
  // Wait around until enough data is available
  if( (Wire.available() < 6)){
    return false;
  };

  uint8_t xlo = Wire.read();
  uint8_t xhi = Wire.read();
  uint8_t ylo = Wire.read();
  uint8_t yhi = Wire.read();
  uint8_t zlo = Wire.read();
  uint8_t zhi = Wire.read();

  // Shift values to create properly formed integer (low byte first)
  _accelData.x = (int16_t)((uint16_t)xlo | ((uint16_t)xhi << 8)) >> 4;
  _accelData.y = (int16_t)((uint16_t)ylo | ((uint16_t)yhi << 8)) >> 4;
  _accelData.z = (int16_t)((uint16_t)zlo | ((uint16_t)zhi << 8)) >> 4;

  return true;
}
void Lsm303::sendRequestMagnet()
{
  // Read the magnetometer
  Wire.beginTransmission((byte)LSM303_ADDRESS_MAG);
  Wire.write(LSM303_REGISTER_MAG_OUT_X_H_M);
  Wire.endTransmission();
  Wire.requestFrom((byte)LSM303_ADDRESS_MAG, (byte)6);
}

bool Lsm303::readMagnet(){
  // Wait around until enough data is available
  if( (Wire.available() < 6)){
    return false;
  };

  // Note high before low (different than accel)
  uint8_t xhi = Wire.read();
  uint8_t xlo = Wire.read();
  uint8_t zhi = Wire.read();
  uint8_t zlo = Wire.read();
  uint8_t yhi = Wire.read();
  uint8_t ylo = Wire.read();

  // Shift values to create properly formed integer (low byte first)
  _magData.x = (int16_t)((uint16_t)xlo | ((uint16_t)xhi << 8));
  _magData.y = (int16_t)((uint16_t)ylo | ((uint16_t)yhi << 8));
  _magData.z = (int16_t)((uint16_t)zlo | ((uint16_t)zhi << 8));
  
  return true;
}

void Lsm303::setMagGain(lsm303MagGain gain)
{
  write8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_CRB_REG_M, (byte)gain);
}

/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/
void Lsm303::write8(byte address, byte reg, byte value)
{
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

byte Lsm303::read8(byte address, byte reg)
{
  byte value;

  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(address, (byte)1);
  value = Wire.read();
  Wire.endTransmission();

  return value;
}
