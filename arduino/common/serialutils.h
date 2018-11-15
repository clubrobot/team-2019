#ifndef __SERIALUTILS_H__
#define __SERIALUTILS_H__

// This library is free software from Club robot Insa Rennes sources; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.


#ifdef ARDUINO
#include <Arduino.h>
#else
#include <string>
typedef unsigned char byte;
typedef std::string String;
#endif


// Serializer

struct Serializer
{
	byte* buffer;

	Serializer(byte buffer[]) : buffer(buffer){}

	template<typename T> Serializer& operator<<(const T& object)
	{
		write(object);
		return *this;
	}

	template<typename T> void write(const T& object)
	{
		*(T*)(buffer) = object;
		buffer += sizeof(T);
	}

	void write(const char* string)
	{
		do
			write(*string);
		while (*(string++) != '\0');
	}
};

template<> inline void Serializer::write<String>(const String& string)
{
	write(string.c_str());
}


// Deserializer

struct Deserializer
{
	byte* buffer;
	byte* adr;

	Deserializer(byte buffer[]) : buffer(buffer), adr(buffer){}

	template<typename T> Deserializer& operator>>(T& object)
	{
		object = read<T>();
		return *this;
	}

	Deserializer& operator>>(char* string)
	{
		do
			*string = read<char>();
		while ((*string++) != '\0');
		return *this;
	}

	template<typename T> T read()
	{
		byte* address = buffer;
		buffer += sizeof(T);
		return *(T*)(address);
	}

	void clear()
	{
		free(adr);
	}

};

template<> inline String Deserializer::read<String>()
{
	String string((char*)(buffer));
	buffer += string.length() + 1;
	return string; 
}



#endif // __SERIALUTILS_H__
