#include "instructions.h"
#include "../common/ExperienceEffects.h"

#include "../common/SerialTalks.h"


extern ExperienceEffects Animation;

void PING(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	output.write<bool>(1);
}


void SET_START(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	output.write<int>(362);
}

void GET_START(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	output.write<long>(Animation.getStart());
}

void SET_TASK(SerialTalks &talks, Deserializer &input, Serializer &output){
	digitalWrite(2, LOW);
}


void IS_CONNECTED(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	output.write<bool>(1);
}
