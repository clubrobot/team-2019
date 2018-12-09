#include "instructions.h"
#include "../common/PannelEffects.h"


extern PannelEffects Animation;
extern TCPTalks talk;

void PING(TCPTalks &inst, UnPickler& input, Pickler& output)
{
	output.dump<bool>(1);
}


void SET_BAR(TCPTalks &inst, UnPickler& input, Pickler& output)
{
	Animation.set_bar_animation((int)input.load<long>());
	output.dump<char*>("CHANGED");
}

void GET_BAR(TCPTalks &inst, UnPickler& input, Pickler& output)
{
	output.dump<long>(Animation.get_bar_animation());
}

void SET_LOGO(TCPTalks &inst, UnPickler& input, Pickler& output)
{
	Animation.set_logo_animation(input.load<long>());
	output.dump<char*>("CHANGED");
}

void GET_LOGO(TCPTalks &inst, UnPickler& input, Pickler& output)
{
	output.dump<long>(Animation.get_logo_animation());
}

void SET_ENGR(TCPTalks &inst, UnPickler& input, Pickler& output)
{
	Animation.set_engr_animation(input.load<long>());
	output.dump<char*>("CHANGED");
}

void GET_ENGR(TCPTalks &inst, UnPickler& input, Pickler& output)
{
	output.dump<long>(Animation.get_engr_animation());
}

void IS_CONNECTED(TCPTalks &inst, UnPickler& input, Pickler& output)
{
	output.dump<bool>(talk.is_connected());
}
