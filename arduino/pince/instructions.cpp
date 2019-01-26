#include "instructions.h"
#include <Servo.h>
#include "../common/SerialTalks.h"

extern Servo pince;

void SET_POSITION(SerialTalks &inst, Deserializer &input, Serializer &output){
	pince.write(input.read<int>());
	output.write<int>(4);
}