#include <Arduino.h>
#include "Pickle.h"

Pickler::Pickler(uint8_t* frame)
{
	current_frame = frame;
	ptr = 0;
	num = 0;
}

void Pickler::start_frame()
{
	// frame[ptr] = (byte)PROTO; 
 //    ptr++;
 //    frame[ptr] = (byte)DEFAULT_PROTOCOL;
 //    ptr++;
 //    frame[ptr] = (byte)SHORT_BINBYTES;
 //    ptr++;
 //    frame[ptr] = (byte)0X01;
 //    ptr++;
    // frame[ptr] = (byte)TCPTALKS_SLAVE_BYTE;
    // ptr++;
    // frame[ptr] = (byte)BINPUT;
    // ptr++;
    // frame[ptr] = (byte)0X00;
    // ptr++;
}

void Pickler::end_frame()
{
	size_t size;

	if(num == 1)
		current_frame[ptr] = (uint8_t)TUPLE1;
	else if(num == 2)
		current_frame[ptr] = (uint8_t)TUPLE2;
	else if(num == 3)
		current_frame[ptr] = (uint8_t)TUPLE3;
	else if (num >= 4)
	    current_frame[ptr] = (uint8_t)TUPLE;
		
	ptr++;
	current_frame[ptr] = (uint8_t)BINPUT;
	ptr++;
	current_frame[ptr] = (uint8_t)0X01;
	ptr++;
	current_frame[ptr] = (uint8_t)'\0';	
}

template<>
void Pickler::dump<bool>(bool var)
{
	num++;
	if(var)
		current_frame[ptr] = (uint8_t) NEWTRUE;

	else
		current_frame[ptr] = (uint8_t) NEWFALSE;
	ptr += 1;
}

template<>
void Pickler::dump<long>(long var)
{
	num++;

	if(var <= 0xff && var >= 0)
	{
		current_frame[ptr] = (uint8_t)BININT1;
		ptr++;
		current_frame[ptr] = (uint8_t)var;
		ptr++;
		return;
	}
	else if(var <= 0xffff && var >= 0)
	{
		current_frame[ptr] = (uint8_t) BININT2; // long en uint8_t
		ptr++;
		current_frame[ptr] = (uint8_t) var;
		ptr++;
		current_frame[ptr] = (uint8_t) (var>>8);
		ptr++;
		return;
	}
	else
	{
		Serial.println(sizeof(var));
		current_frame[ptr] = BININT;
        ptr++;
		current_frame[ptr] = (uint8_t) var;
		ptr++;
		current_frame[ptr] = (uint8_t) (var>>8);
		ptr++;
		current_frame[ptr] = (uint8_t) (var>>16);
		ptr++;
		current_frame[ptr] = (uint8_t) (var>>24);
		ptr++;
		current_frame[ptr] = 0X00;
        ptr++;
		return;
	}
}

template<>
void Pickler::dump<double>(double var)
{
	num++;
	current_frame[ptr] = (uint8_t)BINFLOAT;
	ptr++;

	uint8_t p[8];

	memcpy(p, &var, sizeof(var));
	
	int len = sizeof(p);

	for(int i =0; i<len/2; i++)
	{
		uint8_t tmp = p[i];

        p[i] = p[len-i-1];

        p[len-i-1] = tmp;
		
	}

	memcpy(current_frame+ptr,p , len);

	ptr += len;
}

template<>
void Pickler::dump<char*>(char* var)
{
	num++;
	long len = strlen(var);

	Serial.println(sizeof(len));

	if(len <= 0xff)
	{
		current_frame[ptr] = (uint8_t)SHORT_BINUNICODE;
	}
	else if(len > 0xffffffff)
	{
		current_frame[ptr] = (uint8_t)BINUNICODE8;
	}
	else
	{
		current_frame[ptr] = (uint8_t)BINUNICODE;
	}

	ptr++;
	current_frame[ptr] = (uint8_t) len;
	ptr++;
	current_frame[ptr] = (uint8_t) 0X00;
	ptr++;
	current_frame[ptr] = (uint8_t) 0X00;
	ptr++;
	current_frame[ptr] = (uint8_t) 0X00;
	ptr++;

	
	strcat((char*)current_frame,var);
	ptr += strlen(var);

	current_frame[ptr] = (uint8_t)BINPUT;
	ptr++;
	current_frame[ptr] = (uint8_t)0X00;
	ptr++;
}

template<>
void Pickler::dump<char>(char var)
{
	uint8_t tmp[2] = {BINPUT, 0X00};
	current_frame[ptr] = (uint8_t)SHORT_BINBYTES;
	ptr++;
	current_frame[ptr] = (uint8_t)0X01;
	ptr++;
	current_frame[ptr] = (uint8_t)var;
	ptr++;
    
    memcpy(current_frame+ptr, tmp, 2);
    ptr+=2;

}

UnPickler::UnPickler(uint8_t* frame)
{
	current_frame = frame;
	ptr = 0;
	remove_start_frame();
}
void UnPickler::remove_start_frame()
{
	ptr+=2;
}

void UnPickler::remove_end_frame()
{
	ptr++;
}

void UnPickler::remove_tuple_header()
{
	ptr+=1;
}

template<>
bool UnPickler::load<bool>()
{
	if(current_frame[ptr] == NEWTRUE)
	{
		ptr ++;
		return true;
	}
	else
		return false;
}

template<>
long UnPickler::load<long>()
{
	if(current_frame[ptr] == BININT1)
	{
		ptr++;
		long var = (long)current_frame[ptr];
		ptr ++;
		return var;
	}

	if(current_frame[ptr] == BININT2)
	{
		ptr += 2;
		long var = (long)((current_frame[ptr] << 8) | current_frame[ptr - 1] );
		ptr ++;
		return var;
	}
	if(current_frame[ptr] == BININT)
	{
		ptr += 4;
		long var = (long)((current_frame[ptr] << 24) | (current_frame[ptr-1] << 16) | (current_frame[ptr-2] << 8) | current_frame[ptr - 3] );
		ptr ++;
		return var;
	}
	if(current_frame[ptr] == LONG1)
	{
		ptr += 5;
		long var = (long)((current_frame[ptr] << 24) | (current_frame[ptr-1] << 16) | (current_frame[ptr-2] << 8) | current_frame[ptr - 3] );
		ptr +=2;
		return var;
	}
}

template<>
float UnPickler::load<float>()
{
	if(current_frame[ptr] = BINFLOAT)
	{
		ptr++;

		uint8_t tab[8];

		/* convert to little endian */
		tab[7] = current_frame[ptr];
		tab[6] = current_frame[ptr+1];
		tab[5] = current_frame[ptr+2];
		tab[4] = current_frame[ptr+3];
		tab[3] = current_frame[ptr+4];
		tab[2] = current_frame[ptr+5];
		tab[1] = current_frame[ptr+6];
		tab[0] = current_frame[ptr+7];
	
	double var; 
	memcpy(&var, &tab, sizeof(tab));
		
	return (float)var;
	}
}

template<>
char* UnPickler::load<char*>()
{	
	char* tab;

	if(current_frame[ptr] == BINUNICODE)
	{
		ptr+=4;
		long len = (long)((current_frame[ptr-3] << 24) | (current_frame[ptr-2] << 16) | (current_frame[ptr-1] << 8) | current_frame[ptr] );
		memcpy(tab,(char*)current_frame,len);
	}
	if(current_frame[ptr] == BINUNICODE8)
	{
		ptr+=4;
		long len = (long)((current_frame[ptr-3] << 24) | (current_frame[ptr-2] << 16) | (current_frame[ptr-1] << 8) | current_frame[ptr] );
		memcpy(tab,(char*)current_frame,len);
	}
	if(current_frame[ptr] == SHORT_BINUNICODE)
	{
		ptr+=4;
		long len = (long)((current_frame[ptr-3] << 24) | (current_frame[ptr-2] << 16) | (current_frame[ptr-1] << 8) | current_frame[ptr] );
		memcpy(tab,(char*)current_frame,len);
	}
	ptr ++;
	/* switch ending byte frame */
	ptr+= 2;

	return tab; 
}

bool UnPickler::is_tuple()
{
	if(current_frame[ptr] == '(')
	{
		return true;
	}
	else
		return false;
}

