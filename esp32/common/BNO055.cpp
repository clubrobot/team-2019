#include "BNO055.h"

BNO055::BNO055( int32_t sensorID, uint8_t address){
    _sensorID = sensorID;
    _address = address;
    for(int i = 0; i< 6; i++){_useData[i] = true;}
    _newDataTemp = false;
    _state = NEW_ACCEL_REQUEST;
}

void BNO055::whatToDoNext(){ 
  int j = 0;
  int i = _state - 5;
  while(!_useData[i] && j<6){
    j++;
    i = (i + 1) % 6;
  }
  _state = static_cast<BNO055::State_t>(i);
  if(j>=6){
    disable();
  }
}

void BNO055::process(float timestep){
  switch(_state){
    case NEW_ACCEL_REQUEST :
      requestData(BNO055_ACCEL_DATA_X_LSB_ADDR, 6);
      _state = WAITING_ACCEL;
      break;

    case WAITING_ACCEL :
      if(readVector(_accel)){
        whatToDoNext();
      }
      break;

    case NEW_MAGNET_REQUEST:
      requestData(BNO055_MAG_DATA_X_LSB_ADDR, 6);
      _state = WAITING_MAGNET;
      break;

    case WAITING_MAGNET:
      if(readVector(_mag)){
        whatToDoNext();
      }
      break;
    case NEW_ORIENTATION_REQUEST :      
      requestData(BNO055_EULER_H_LSB_ADDR, 6);
      _state = WAITING_ORIENTATION;
      break;

    case WAITING_ORIENTATION :
      if(readVector(_orientation)){
        whatToDoNext();
      }
      break;
    case NEW_ANGVEL_REQUEST :
      requestData(BNO055_GYRO_DATA_X_LSB_ADDR, 6);
      _state = WAITING_ANGVEL;
      break;

    case WAITING_ANGVEL :
      if(readVector(_angvel)){
        whatToDoNext();
      }
      break;
    case NEW_LINACC_REQUEST:
      requestData(BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR, 6);
      _state = WAITING_LINACC;
      break;

    case WAITING_LINACC :
    if(readVector(_linAccel)){
        whatToDoNext();
      }
      break;
    case NEW_TEMP_REQUEST :
      requestData(BNO055_TEMP_ADDR, 6);
      _state = WAITING_TEMP;
      break;

    case WAITING_TEMP :
      if(readTemp()){
        whatToDoNext();
      }
      break;
  }

}
void BNO055::onProcessEnabling(){
  modeConfig(false);
  _state = WAITING_TEMP;
  whatToDoNext();
}
void BNO055::onProcessDisabling(){
  modeConfig(true);
}



bool BNO055::begin(int SDA, int SCL){
  /* Enable I2C */
  Wire.begin(SDA, SCL);

  /* Make sure we have the right device */
  uint8_t id = read8(BNO055_CHIP_ID_ADDR);
  if(id != BNO055_ID)
  {
    return false;  
  }
  _sensorID = id;
  modeConfig(true);
  // Set to normal power mode 
  write8(BNO055_PWR_MODE_ADDR, POWER_MODE_NORMAL);
  delay(10);

  write8(BNO055_PAGE_ID_ADDR, 0);

  write8(BNO055_SYS_TRIGGER_ADDR, 0x0);
  delay(10);
  // Set the requested operating mode (see section 3.3) 
  modeConfig(false);
  delay(10);
  return true;
}

void BNO055::modeConfig(bool on_off){
  
  if(on_off){
    //set the BNO055 in config mode to change its configuration register 
    write8(BNO055_OPR_MODE_ADDR, OPERATION_MODE_CONFIG);
    delay(10);
  }
  else{
    write8(BNO055_OPR_MODE_ADDR, OPERATION_MODE_NDOF);
    delay(10);
  }
}


void BNO055::requestData(bno055_reg_t reg, uint8_t len)
{
  // Read the magnetometer
  Wire.beginTransmission(_address);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();
  Wire.requestFrom(_address, (byte)len);
}

bool BNO055::readVector(Vector_t & vect){
  // Wait around until enough data is available
  if( (Wire.available() < 6)){
    return false;
  };
  uint8_t buffer[6];
  for (uint8_t i = 0; i < 6; i++)
  {
    buffer[i] = Wire.read();
  }

  // Shift values to create properly formed integer (low byte first)
  vect._x =  ((int16_t)buffer[0])  |  (((int16_t)buffer[1]) << 8);
  vect._y =  ((int16_t)buffer[2])  |  (((int16_t)buffer[3]) << 8);
  vect._z =  ((int16_t)buffer[4])  |  (((int16_t)buffer[5]) << 8);

  vect._x =  vect._x / vect._fact;
  vect._y =  vect._y / vect._fact;
  vect._z =  vect._z / vect._fact;

  vect._newData = true;
  return true;
}

bool BNO055::readTemp(){
  // Wait around until enough data is available
  if( (Wire.available() < 1)){
    return false;
  };
  _temp = (int) Wire.read();
  _newDataTemp = true;
  return true;
}


/*!
    @brief  Writes an 8 bit value over I2C
*/
/**************************************************************************/
bool BNO055::write8(bno055_reg_t reg, byte value)
{
  Wire.beginTransmission(_address);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)value);
  Wire.endTransmission();
  return true;
}

/**************************************************************************/
/*!
    @brief  Reads an 8 bit value over I2C
*/
/**************************************************************************/
byte BNO055::read8(bno055_reg_t reg )
{
  byte value = 0;

  Wire.beginTransmission(_address);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();
  Wire.requestFrom(_address, (byte)1);
  value = Wire.read();

  return value;
}

/**************************************************************************/
/*!
    @brief  Reads the specified number of bytes over I2C
*/
/**************************************************************************/
bool BNO055::readLen(bno055_reg_t reg, byte * buffer, int len)
{
  Wire.beginTransmission(_address);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();
  Wire.requestFrom(_address, (byte)len);
  for (int i = 0; i < len; i++)
  {
    buffer[i] = Wire.read();
  }
  return true;
}