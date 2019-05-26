#include "instructions.h"
#include "../common/BNO055.h"
#include "pin.h"
#include "AccelerationController.h"

extern BNO055 bno;
extern bool connected;
extern AccelerationController meanAcc;


void GET_ACCELERATION(SerialTalks &talks, Deserializer &input, Serializer &output){
    BNO055::Vector_t mes = bno.getAcc();
    output.write<int16_t>(mes.getx());
    output.write<int16_t>(mes.gety());
    output.write<int16_t>(mes.getz());
}
void GET_MAGNETIC(SerialTalks &talks, Deserializer &input, Serializer &output){
    BNO055::Vector_t mes = bno.getMag();
    output.write<int16_t>(mes.getx());
    output.write<int16_t>(mes.gety());
    output.write<int16_t>(mes.getz());
}
void GET_ORIENTATION(SerialTalks &talks, Deserializer &input, Serializer &output){
    BNO055::Vector_t mes = bno.getAbsO();
    output.write<int16_t>(mes.getx());
    output.write<int16_t>(mes.gety());
    output.write<int16_t>(mes.getz());
}
void GET_GYROSCOPE(SerialTalks &talks, Deserializer &input, Serializer &output){
    BNO055::Vector_t mes = bno.getAngVel();
    output.write<int16_t>(mes.getx());
    output.write<int16_t>(mes.gety());
    output.write<int16_t>(mes.getz());
}
void GET_LINEAR_ACCELERATION(SerialTalks &talks, Deserializer &input, Serializer &output){
    BNO055::Vector_t mes = bno.getLinAcc();
    output.write<int16_t>(mes.getx());
    output.write<int16_t>(mes.gety());
    output.write<int16_t>(mes.getz());
}
void GET_TEMPERATURE(SerialTalks &talks, Deserializer &input, Serializer &output){
    output.write<int16_t>(bno.getTemp());
}
void GET_STATE(SerialTalks &talks, Deserializer &input, Serializer &output){
    output.write<int16_t>(bno.getState());
}

void READ_BYTE(SerialTalks &talks, Deserializer &input, Serializer &output){
    int16_t address = input.read<int16_t>();
    output.write<int16_t>(bno.read8(static_cast<BNO055::bno055_reg_t>(address)));
}

void READ_LEN(SerialTalks &talks, Deserializer &input, Serializer &output){
    int16_t address = input.read<int16_t>();
    int16_t len = input.read<int16_t>();
    byte buffer[len];
    bno.readLen(static_cast<BNO055::bno055_reg_t>(address), buffer,len);
    for(int16_t i = 0; i< len; i++){
        output.write<int16_t>(buffer[i]);
    }
}

void IS_CONNECTED(SerialTalks &talks, Deserializer &input, Serializer &output){
    output.write<bool>(connected);
}

void BEGIN_COM(SerialTalks &talks, Deserializer &input, Serializer &output){
    connected = bno.begin(PIN_SDA, PIN_SCL);
    output.write<bool>(connected);
}

void GET_ABSOLUTE_ACCELERATION(SerialTalks &talks, Deserializer &input, Serializer &output){
    
    output.write<int16_t>(meanAcc.getMeanAcceleration_X());
    output.write<int16_t>(meanAcc.getMeanAcceleration_Y());
    output.write<int16_t>(meanAcc.getOrientation());
    
    meanAcc.resetMean();
}
void SET_ORIENTATION(SerialTalks &talks, Deserializer &input, Serializer &output){
    meanAcc.setOrientation(input.read<int16_t>(), bno.getAbsO().getx());
}