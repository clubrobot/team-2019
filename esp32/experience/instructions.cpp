#include "instructions.h"
#include "../common/ExperienceEffects.h"


extern ExperienceEffects Animation;

void PING(TCPTalks& talk, UnPickler& input, Pickler& output)
{
	output.dump<bool>(1);
}


void GET_START(TCPTalks& talk, UnPickler& input, Pickler& output)
{
	output.dump<bool>(1);
}

void SET_START(TCPTalks& talk, UnPickler& input, Pickler& output)
{
	Animation.start();
}

void SET_TASK(TCPTalks &talk, UnPickler &input, Pickler &output){
	digitalWrite(2, LOW);
}


void IS_CONNECTED(TCPTalks& talk, UnPickler& input, Pickler& output)
{
	output.dump<bool>(1);
}
