#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "../common/tcptalks.h"
#include "../common/Pickle.h"

#define PING_OPCODE      0x10

#define SET_START_OPCODE	 0x11
#define GET_START_OPCODE	 0x12
#define SET_TASK_DEBUG	 0x13


void PING(TCPTalks& talks, UnPickler& input, Pickler& output);

void SET_START(TCPTalks& talks, UnPickler& input, Pickler& output);

void GET_START(TCPTalks& talks, UnPickler& input, Pickler& output);

void SET_TASK(TCPTalks& talks, UnPickler& input, Pickler& output);

#endif //__INSTRUCTIONS_H__
