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
 /**
    * \struct Serializer
    * \brief Objet destiné à creer un flux de sortie pour les programme cpp.
    *
	* Serializer permet de remplir un buffer en octet à l'aide de variable de tous type.
	* Cela permet une utilisation plus simple de SerialTalks.
	* Voir l'utilisation dans la doc python.
    */
struct Serializer
{
	byte* buffer;/*!< pointer vers le buffer à complêter */
	//! Constructeur de Serializer
	/*!
		\param buffer pointeur du buffer.
	*/
	Serializer(byte buffer[]) : buffer(buffer){}

	//! Operateur de décalage, a utilisé pour remplir le buffer. 
	/*!
		\param object Object a renvoyer dans le buffer pour transmission.

		\return Retourne le pointeur du serializer pour une utilisation plus simple
	*/
	template<typename T> Serializer& operator<<(const T& object)
	{
		write(object);
		return *this;
	}
	//! Methode pour une utilisation interne qui permet d'écrire sur le buffer après conversion en octets. 
	/*!
		\param object Object a renvoyer dans le buffer pour ecriture.

	*/
	template<typename T> void write(const T& object)
	{
		*(T*)(buffer) = object;
		buffer += sizeof(T);
	}
	//! Methode pour une utilisation interne qui permet d'écrire sur le buffer après conversion en octets. 
	/*!
		\param string Object (sous la forme d'un char) a renvoyer dans le buffer pour ecriture.

	*/
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
 /**
    * \struct Deserializer
    * \brief Objet destiné à extraire des variables d'un flux en octet.
    *
	* Deserializer permet d'extraire d'un buffer des variables.
	* Cela permet une utilisation plus simple de SerialTalks.
	* Voir l'utilisation dans la doc python. Attention a bien extraire les variables dans le bonne ordre pour éviter les problèmes d'encodage et autres.
    */
struct Deserializer
{
	byte* buffer;/*!< pointer vers le buffer à vider */
	byte* adr;

	//! Constructeur de Deserializer
	/*!
		\param buffer pointeur du buffer à utiliser.
	*/

	Deserializer(byte buffer[]) : buffer(buffer), adr(buffer){}
	//! Operateur de décalage, à utiliser pour extraire les variables du buffer. 
	/*!
		\param object Object à complêter avec le buffer. Attention le type de la variable est pris en compte dans la conversion octect -> var

		\return Retourne le pointeur du deserializer pour une utilisation plus simple.
	*/
	template<typename T> Deserializer& operator>>(T& object)
	{
		object = read<T>();
		return *this;
	}
	//! Operateur de décalage, a utilisé pour remplir le buffer uniquement pour les variables de type char. 
	/*!
		\param string Object a renvoyer dans le buffer pour transmission.

		\return Retourne le pointeur du serializer pour une utilisation plus simple
	*/
	Deserializer& operator>>(char* string)
	{
		do
			*string = read<char>();
		while ((*string++) != '\0');
		return *this;
	}
	//! Methode interne pour convertir les octets du buffer en données exploitables.
	/*!

		\return Retourne la valeur extraite du buffer.
	*/
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
