#ifndef __SERIALUTILS_H__
#define __SERIALUTILS_H__

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

	Deserializer(byte buffer[]) : buffer(buffer){}

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
};

template<> inline String Deserializer::read<String>()
{
	String string((char*)(buffer));
	buffer += string.length() + 1;
	return string; 
}

#endif // __SERIALUTILS_H__
