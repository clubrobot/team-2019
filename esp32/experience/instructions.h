#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/tcptalks.h"
#include "../common/Pickle.h"

#define PING_OPCODE      0x10

#define SET_START_OPCODE	 0X11
#define GET_START_OPCODE	 0X12



void PING(TCPTalks &inst, UnPickler& input, Pickler& output);

void SET_START(TCPTalks &inst, UnPickler& input, Pickler& output);

void GET_START(TCPTalks &inst, UnPickler& input, Pickler& output);



#endif //__INSTRUCTIONS_H__
