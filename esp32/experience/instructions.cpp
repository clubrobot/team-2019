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
	Animation.start();
	output.write<long>(Animation.getStart());
}

void GET_START(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	output.write<long>(Animation.getStart());
}


void IS_CONNECTED(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	output.write<bool>(1);
}
