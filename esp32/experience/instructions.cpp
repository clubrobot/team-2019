#include "instructions.h"
#include "../common/ExperienceEffects.h"


extern ExperienceEffects Animation;
extern TCPTalks talk;

void PING(TCPTalks &inst, UnPickler& input, Pickler& output)
{
	output.dump<bool>(1);
}


void SET_START(TCPTalks &inst, UnPickler& input, Pickler& output)
{
	Animation.start();
	output.dump<char*>("CHANGED");
}

void GET_START(TCPTalks &inst, UnPickler& input, Pickler& output)
{
	output.dump<long>(Animation.getStart());
}


void IS_CONNECTED(TCPTalks &inst, UnPickler& input, Pickler& output)
{
	output.dump<bool>(talk.is_connected());
}
