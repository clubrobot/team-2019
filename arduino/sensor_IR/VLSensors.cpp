#include "VLSensors.h"
#include <Arduino.h>

VLSensors::VLSensors(int EN_VL53_PIN, int EN_VL61_PIN){
    _EN_VL53 = EN_VL53_PIN;
    _EN_VL61 = EN_VL61_PIN;
    for(int i = 0; i< 2; i++){_useSensors[i] = true;}
    _state = REQUEST_VL53;
    _range[VL53] = 0;
    _range[VL61] = 0;
    shutBothVL();
    _addressVL53 = 0x29;
    _addressVL61 = 0x29;
}

void VLSensors::whatToDoNext(){ 
  int j = 0;
  int i = (_state - 3) % 2 ;
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
  uint8_t buf[2];
  
  switch(_state){
    case REQUEST_VL53 :
      _timeoutchecker.restart();
      startRangingVL53();
      _i2cused = true;
      _state = MESURING_VL53;
      Serial.println("Ranging");
      while (read8VL53(SYSRANGE_START) & 0x01){
        if(timeout()){
          _state = WAITING_VL53;
          break;
        }
      }
      requestDataVL53(RESULT_INTERRUPT_STATUS, 1);
      break;
    
    case MESURING_VL53 : 
      Serial.println("New iteration");
      if(timeout()){
        _state = WAITING_VL53;
      }
      else if(readLen(buf, 1)){
        Serial.println("Received");
        if((buf[0] & 0x07) == 0){                            //Attente que le flag indiquant la fin de mesure soit à 1
          requestDataVL53(RESULT_INTERRUPT_STATUS, 1);
          Serial.println("New request");
          Serial.print("buf = "); Serial.println(buf[0]);
        }
        else{
          Serial.println("mesure............");
          requestDataVL53(RESULT_RANGE_STATUS + 10, 2);     //Lecture du registre contenant la mesure
          _state = WAITING_VL53;
        }
      }

      break;

    case WAITING_VL53 :      
      if(timeout()){
        _VL53Error = true;
        whatToDoNext();
      }
      else if(readLen(buf, 2)){
        _range[0] = ((uint16_t) buf[0]) | (((uint16_t) buf[1]) << 8);
        Serial.print("range = "); Serial.println(_range[0]);
        write8VL53(SYSTEM_INTERRUPT_CLEAR, 0x01);
        whatToDoNext();
        _i2cused = false;
        _VL53Error = false;
      }

      break;

    case REQUEST_VL61:
      _i2cused = true;
      requestDataVL61(RESULT__RANGE_VAL, 1);
      _state = WAITING_VL61;
      break;

    case WAITING_VL61:
      if(readLen(buf, 1)){
        _range[VL61] = (uint16_t) buf[0];
        write8VL61(SYSTEM__INTERRUPT_CLEAR, 0x01);
        whatToDoNext();
        _i2cused = false;
      }
      break;
  }
}

void VLSensors::startRangingVL53(){
  
  write8VL53(0x80, 0x01);
  write8VL53(0xFF, 0x01);
  write8VL53(0x00, 0x00);
  write8VL53(0x91, stop_variable);
  write8VL53(0x00, 0x01);
  write8VL53(0xFF, 0x00);
  write8VL53(0x80, 0x00);

  write8VL53(SYSRANGE_START, 0x01);
}

uint16_t VLSensors::readRangeMillimeters(){
  if((_range[1] <= _threshold) && ( _range[0] < _threshold)){
    return _range[1];
  }
  else if((_range[1] > _threshold) && ( _range[0] >= _threshold)){
    return _range[0];
  }
  else
  {
    return _range[0] + _range[1] / 2; 
  }
  return 0xFFFF;
}


void VLSensors::requestDataVL53(uint8_t reg, uint8_t len)
{
  // Read the magnetometer
  Wire.beginTransmission(_addressVL53);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();
  Wire.requestFrom(_addressVL53, (byte)len);
}

bool VLSensors::readLen(uint8_t * buff, uint8_t len){
  // Wait around until enough data is available
  if( (Wire.available() < len)){
    return false;
  }
  for (uint8_t i = 0; i < len; i++)
  {
    buff[i] = Wire.read();
  }
  return true;
}

void VLSensors::requestDataVL61(uint16_t reg, uint8_t len)
{
  // Read the magnetometer
  Wire.beginTransmission(_addressVL61);
  Wire.write((reg >> 8) & 0xFF);  // reg high byte
  Wire.write(reg & 0xFF);         // reg low byte
  Wire.endTransmission();
  Wire.requestFrom(_addressVL61, (byte)len);
}


// Writes an 8-bit register
void VLSensors::write8VL61(uint16_t reg, uint8_t value)
{
  Wire.beginTransmission(_addressVL61);
  Wire.write((reg >> 8) & 0xFF);  // reg high byte
  Wire.write(reg & 0xFF);         // reg low byte
  Wire.write(value);
  Wire.endTransmission();
}

// Writes a 16-bit register
void VLSensors::write16VL61(uint16_t reg, uint16_t value)
{
  Wire.beginTransmission(_addressVL61);
  Wire.write((reg >> 8) & 0xFF);  // reg high byte
  Wire.write(reg & 0xFF);         // reg low byte
  Wire.write((value >> 8) & 0xFF);  // value high byte
  Wire.write(value & 0xFF);         // value low byte
  Wire.endTransmission();
}

// Writes a 32-bit register
void VLSensors::write32VL61(uint16_t reg, uint32_t value)
{
  Wire.beginTransmission(_addressVL61);
  Wire.write((reg >> 8) & 0xFF);  // reg high byte
  Wire.write(reg & 0xFF);         // reg low byte
  Wire.write((value >> 24) & 0xFF); // value highest byte
  Wire.write((value >> 16) & 0xFF);
  Wire.write((value >> 8) & 0xFF);
  Wire.write(value & 0xFF);         // value lowest byte
  Wire.endTransmission();
}


// Write an 8-bit register
void VLSensors::write8VL53(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(_addressVL53);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// Write a 16-bit register
void VLSensors::write16VL53(uint8_t reg, uint16_t value)
{
  Wire.beginTransmission(_addressVL53);
  Wire.write(reg);
  Wire.write((value >> 8) & 0xFF); // value high byte
  Wire.write( value       & 0xFF); // value low byte
  Wire.endTransmission();
}

// Write a 32-bit register
void VLSensors::write32VL53(uint8_t reg, uint32_t value)
{
  Wire.beginTransmission(_addressVL53);
  Wire.write(reg);
  Wire.write((value >> 24) & 0xFF); // value highest byte
  Wire.write((value >> 16) & 0xFF);
  Wire.write((value >>  8) & 0xFF);
  Wire.write( value        & 0xFF); // value lowest byte
  Wire.endTransmission();
}

void VLSensors::init(){

  write8VL61(0x207, 0x01);
    write8VL61(0x208, 0x01);
    write8VL61(0x096, 0x00);
    write8VL61(0x097, 0xFD); // RANGE_SCALER = 253
    write8VL61(0x0E3, 0x00);
    write8VL61(0x0E4, 0x04);
    write8VL61(0x0E5, 0x02);
    write8VL61(0x0E6, 0x01);
    write8VL61(0x0E7, 0x03);
    write8VL61(0x0F5, 0x02);
    write8VL61(0x0D9, 0x05);
    write8VL61(0x0DB, 0xCE);
    write8VL61(0x0DC, 0x03);
    write8VL61(0x0DD, 0xF8);
    write8VL61(0x09F, 0x00);
    write8VL61(0x0A3, 0x3C);
    write8VL61(0x0B7, 0x00);
    write8VL61(0x0BB, 0x3C);
    write8VL61(0x0B2, 0x09);
    write8VL61(0x0CA, 0x09);
    write8VL61(0x198, 0x01);
    write8VL61(0x1B0, 0x17);
    write8VL61(0x1AD, 0x00);
    write8VL61(0x0FF, 0x05);
    write8VL61(0x100, 0x05);
    write8VL61(0x199, 0x05);
    write8VL61(0x1A6, 0x1B);
    write8VL61(0x1AC, 0x3E);
    write8VL61(0x1A7, 0x1F);
    write8VL61(0x030, 0x00);

    write8VL61(SYSTEM__FRESH_OUT_OF_RESET, 0);

    // init VL53 register

  write8VL53(VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV, read8VL53(VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV) | 0x01); // set bit 0

  // "Set I2C standard mode"
  write8VL53(0x88, 0x00);

  write8VL53(0x80, 0x01);
  write8VL53(0xFF, 0x01);
  write8VL53(0x00, 0x00);
  stop_variable = read8VL53(0x91);
  write8VL53(0x00, 0x01);
  write8VL53(0xFF, 0x00);
  write8VL53(0x80, 0x00);

  // disable SIGNAL_RATE_MSRC (bit 1) and SIGNAL_RATE_PRE_RANGE (bit 4) limit checks
  write8VL53(MSRC_CONFIG_CONTROL, read8VL53(MSRC_CONFIG_CONTROL) | 0x12);

  write8VL53(SYSTEM_SEQUENCE_CONFIG, 0xFF);

  // VL53L0X_DataInit() end

  // VL53L0X_StaticInit() begin

  write8VL53(0xFF, 0x01);
  write8VL53(0x00, 0x00);

  write8VL53(0xFF, 0x00);
  write8VL53(0x09, 0x00);
  write8VL53(0x10, 0x00);
  write8VL53(0x11, 0x00);

  write8VL53(0x24, 0x01);
  write8VL53(0x25, 0xFF);
  write8VL53(0x75, 0x00);

  write8VL53(0xFF, 0x01);
  write8VL53(0x4E, 0x2C);
  write8VL53(0x48, 0x00);
  write8VL53(0x30, 0x20);

  write8VL53(0xFF, 0x00);
  write8VL53(0x30, 0x09);
  write8VL53(0x54, 0x00);
  write8VL53(0x31, 0x04);
  write8VL53(0x32, 0x03);
  write8VL53(0x40, 0x83);
  write8VL53(0x46, 0x25);
  write8VL53(0x60, 0x00);
  write8VL53(0x27, 0x00);
  write8VL53(0x50, 0x06);
  write8VL53(0x51, 0x00);
  write8VL53(0x52, 0x96);
  write8VL53(0x56, 0x08);
  write8VL53(0x57, 0x30);
  write8VL53(0x61, 0x00);
  write8VL53(0x62, 0x00);
  write8VL53(0x64, 0x00);
  write8VL53(0x65, 0x00);
  write8VL53(0x66, 0xA0);

  write8VL53(0xFF, 0x01);
  write8VL53(0x22, 0x32);
  write8VL53(0x47, 0x14);
  write8VL53(0x49, 0xFF);
  write8VL53(0x4A, 0x00);

  write8VL53(0xFF, 0x00);
  write8VL53(0x7A, 0x0A);
  write8VL53(0x7B, 0x00);
  write8VL53(0x78, 0x21);

  write8VL53(0xFF, 0x01);
  write8VL53(0x23, 0x34);
  write8VL53(0x42, 0x00);
  write8VL53(0x44, 0xFF);
  write8VL53(0x45, 0x26);
  write8VL53(0x46, 0x05);
  write8VL53(0x40, 0x40);
  write8VL53(0x0E, 0x06);
  write8VL53(0x20, 0x1A);
  write8VL53(0x43, 0x40);

  write8VL53(0xFF, 0x00);
  write8VL53(0x34, 0x03);
  write8VL53(0x35, 0x44);

  write8VL53(0xFF, 0x01);
  write8VL53(0x31, 0x04);
  write8VL53(0x4B, 0x09);
  write8VL53(0x4C, 0x05);
  write8VL53(0x4D, 0x04);

  write8VL53(0xFF, 0x00);
  write8VL53(0x44, 0x00);
  write8VL53(0x45, 0x20);
  write8VL53(0x47, 0x08);
  write8VL53(0x48, 0x28);
  write8VL53(0x67, 0x00);
  write8VL53(0x70, 0x04);
  write8VL53(0x71, 0x01);
  write8VL53(0x72, 0xFE);
  write8VL53(0x76, 0x00);
  write8VL53(0x77, 0x00);

  write8VL53(0xFF, 0x01);
  write8VL53(0x0D, 0x01);

  write8VL53(0xFF, 0x00);
  write8VL53(0x80, 0x01);
  write8VL53(0x01, 0xF8);

  write8VL53(0xFF, 0x01);
  write8VL53(0x8E, 0x01);
  write8VL53(0x00, 0x01);
  write8VL53(0xFF, 0x00);
  write8VL53(0x80, 0x00);

  // -- VL53L0X_load_tuning_settings() end
  write8VL53(SYSTEM_INTERRUPT_CONFIG_GPIO, 0x04);
  write8VL53(GPIO_HV_MUX_ACTIVE_HIGH, read8VL53(GPIO_HV_MUX_ACTIVE_HIGH) & ~0x10); // active low
  write8VL53(SYSTEM_INTERRUPT_CLEAR, 0x01);
  // -- VL53L0X_SetGpioConfig() end
   write8VL53(SYSTEM_SEQUENCE_CONFIG, 0xE8);
}

void VLSensors::setVL61Address(uint8_t new_addr)
{
  pinMode(_EN_VL61, INPUT);
  delay(5);
  write8VL61(I2C_SLAVE__DEVICE_ADDRESS, new_addr & 0x7F);
  _addressVL61 = new_addr;
  
}

void VLSensors::setVL53Address(uint8_t new_addr)
{
  pinMode(_EN_VL53, INPUT);
  delay(5);
  write8VL53(I2C_SLAVE_DEVICE_ADDRESS, new_addr & 0x7F);
  _addressVL53 = new_addr;
}

uint8_t VLSensors::read8VL53(uint8_t reg)
{
  uint8_t value;

  Wire.beginTransmission(_addressVL53);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(_addressVL53, (uint8_t)1);
  value = Wire.read();

  return value;
}

void VLSensors::shutBothVL(){
  pinMode(_EN_VL53, OUTPUT);
  digitalWrite(_EN_VL53, LOW);
  pinMode(_EN_VL61, OUTPUT);
  digitalWrite(_EN_VL61, LOW);
}

void VLSensors::startBothVL(){
  pinMode(_EN_VL53, INPUT);
  pinMode(_EN_VL61, INPUT);
}

bool VLSensors::timeout(){
  if(_timeoutchecker.getElapsedTime() >= 0.4){
    return true;
  }
  return false;
}