#include "instructions.h"
#include "filter/Kalman.h"
#include "absoluteInertial/AccelerationController.h"
#include "../../../common/BNO055.h"

extern float p[2]; // Target point

extern BNO055 bno;
extern AccelerationController meanAcc;
extern Kalman filter;

void GET_POSITION(SerialTalks &talks, Deserializer &input, Serializer &output)
{
    output.write<int16_t>((int16_t)p[0]);
    output.write<int16_t>((int16_t)p[1]);
}

void SET_POSITION(SerialTalks &talks, Deserializer &input, Serializer &output){
    filter.init(p[1], p[2], (float) input.read<int16_t>(), (float) input.read<int16_t>());
    meanAcc.setOrientation(input.read<int16_t>(), bno.getAbsO().getx());
}