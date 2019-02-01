#ifndef __SERIALTALKS_H__
#define __SERIALTALKS_H__

// This library is free software from Club robot Insa Rennes sources; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.


#include <Arduino.h>
#include "serialutils.h"
#include "CRC16.h"

#ifndef SERIALTALKS_BAUDRATE
#define SERIALTALKS_BAUDRATE 115200   /*!< Bauderate utiliser */
#endif

#ifndef SERIALTALKS_INPUT_BUFFER_SIZE
#define SERIALTALKS_INPUT_BUFFER_SIZE 64
#endif

#ifndef SERIALTALKS_OUTPUT_BUFFER_SIZE
#define SERIALTALKS_OUTPUT_BUFFER_SIZE 64
#endif

#ifndef SERIALTALKS_UUID_ADDRESS
#define SERIALTALKS_UUID_ADDRESS 0x0000000000
#endif

#ifndef SERIALTALKS_UUID_LENGTH
#define SERIALTALKS_UUID_LENGTH	32
#endif

#ifndef SERIALTALKS_MAX_PROCESSING
#define SERIALTALKS_MAX_PROCESSING 0x4
#endif

#ifndef SERIALTALKS_MAX_OPCODE
#define SERIALTALKS_MAX_OPCODE 0x20
#endif

#define SERIALTALKS_MASTER_BYTE 'R'
#define SERIALTALKS_SLAVE_BYTE  'A'

#define SERIALTALKS_DEFAULT_UUID_LENGTH 9

#define SERIALTALKS_PING_OPCODE       0x0
#define SERIALTALKS_GETUUID_OPCODE    0x1
#define SERIALTALKS_SETUUID_OPCODE    0x2
#define SERIALTALKS_DISCONNECT_OPCODE 0x3
#define SERIALTALKS_GETEEPROM_OPCODE  0x4
#define SERIALTALKS_SETEEPROM_OPCODE  0x5
#define SERIALTALKS_GETBUFFERSIZE_OPCODE 0x6
#define SERIALTALKS_RESEND_OPCODE    0xFE
#define SERIALTALKS_FREE_BUFFER_OPCODE 0xFA
#define SERIALTALKS_STDOUT_RETCODE 0xFFFFFFFF
#define SERIALTALKS_STDERR_RETCODE 0xFFFFFFFE

#define SERIALTALKS_CRC_SIZE 2
/** class SerialTalks
 *  \brief Object de communication serial avec un ordinateur.
 *	\author Ulysse Darmet
	\author François Gauthier-Clerc
 *  est un outil permettant à l'arduino de pouvoir répondre aux requêtes recu depuis le serial.
 *  Il utilise donc le port serial (usb) pour envoyer ou recevoir des données avec l'ordinateur ou la raspberry
 *  La classe est capable de lancer des methodes sur demande de l'ordinateur ou de la raspberry.
 */
class SerialTalks
{
public: // Public API
	/** class ostream
	 * \brief Stream virtuel pour les erreurs et autre.
	 *	est un outils pour permettre de mieux transmettre les erreurs rencontrées et les STD::OUT
	*/
	class ostream : public Print
	{
	public:

		//! Ecrit sur le serial l'octet indiqué.
		/*!
			\param c octet à passer dans le serial.
			\return Nombre d'octet transmit.
		*/
		virtual size_t write(uint8_t);
		//! Ecrit sur le serial le buffer indiqué (liste d'octets).
		/*!
			\param buffer à passer.
			\param size (taille) du buffer.
			\return Nombre d'octet transmit.
		*/
		virtual size_t write(const uint8_t *buffer, size_t size);
		//! Surcharge de l'opérateur '<<'.
		//! Cette méthode permet de passer plus facilement les objets dans le serial avec conversion en octets automatique.
		/*!
			\param object à passer dans le serial.

		*/
		template<typename T> ostream& operator<<(const T& object)
		{
			print(object);
			return *this;
		}
	
	protected:
		//! Initialise le ostream. C'est à dire expliciter le pointeur du SerialTalks et le retcode à associer.
		/*!
			\param parent SerialTalks à associer.
			\param retcode Code d'identification à utiliser pour l'utilisation du serial.

		*/
		void begin(SerialTalks& parent, long retcode);

		SerialTalks* m_parent;/*!< SerialTalks parent  */
		long         m_retcode;/*!< RetCode à associer au flux virtuel */

		friend class SerialTalks;
	};
	/*! \var typedef *Instruction
	 * \brief Instruction est un pointeur de fonction dont la signature doit être de la forme : (SerialTalks& inst, Deserializer& input, Serializer& output).
	 *
	 */
	typedef void (*Instruction)(SerialTalks& inst, Deserializer& input, Serializer& output);
	/*! \var typedef *Processing
	 * \brief Processing est un pointeur de fonction dont la signature doit être de la forme : (SerialTalks& inst, Deserializer& input).
	 *	Cette méthode sera appelée après que la raspberry traitera la requête de l'arduino.
	 */
	typedef void (*Processing)(SerialTalks& inst, Deserializer& input);
	//! Initialise le SerialTalks avec un Stream d'<arduino.h>.
	/*!
		\param stream Flux à associer pour la communication de SerialTalks.
	*/
	void begin(Stream& stream);
	//! Associe une Instruction à un OPCODE.
	/*!
		\param opcode Code à associer à la fonction.
		\param instruction Fonction à répertorier dans SerialTalks.
	*/
	void bind(byte opcode, Instruction instruction);
	//! Associe une fonction au retour de la requête de l'OPCODE.
	/*!
		\param opcode Code à associer à la fonction.
		\param instruction Fonction à répertorier dans SerialTalks.
	*/
	void attach(byte opcode, Processing processing);
	//! Lance la fonction à partir des octets reçus. La méthode lit l'OPCode et transmet à la bonne fonction l'objet Deserializer avec le reste les octets reçu non traités et un Serialiser pour la réponse à transmettre. 
	/*!
		\param inputBuffer Liste des octets reçus pour cette requête.
		\return Vrai si la fonction à renvoyé des informations.
	*/
	bool execinstruction(byte* inputBuffer);
	//! Lit les octets reçus et les traites quand ils forment une requête complête.
	/*!
		\return Vrai si une requête à renvoyé une information.
	*/

	bool execute();

	/**
	 * @brief Récupère le Serializer pour le remplir avant l'appel de la méthode SerialTalks::send.
	 * 
	 * @return Serializer à remplir. 
	 */
	Serializer getSerializer() {return Serializer(m_outputBuffer);}

	/**
	 * @brief Lance la requête avec les données chargées dans le Serializer et l'OPCODE.
	 * 
	 * @param opcode Code à utiliser pour le requête vers la Raspeberry.
	 * @param output Serializer à utiliser pour récuperer les données.
	 * @return int  Nombre d'octet envoyés.
	 */
	int send(byte opcode,Serializer output);

	//! Indique si le stream de SerialTalks est bien connecté.
	/*!
		\return Vrai si le stream est connecté.
	*/
	bool isConnected() const {return m_connected;}
	//! Méthode bloquante jusqu'a la connexion du Stream ou jusqu'au timeout.
	/*!
		\param timeout Timeout pour la méthode 
		\return Vrai si le Stream est connecté.
	*/
	bool waitUntilConnected(float timeout = -1);
	//! Ecrit sur le pointeur l'UUID enregistré dans l'EEPROM de l'Arduino.
	/*!
		\param uuid Pointeur à utiliser.
		\return Vrai si il existe bien un UUID.
	*/
	bool getUUID(char* uuid);
	//! Enregistre l'UUID dans l'EEPROM de l'Arduino.
	/*!
		\param uuid Pointeur de l'UUID à enregistrer.
	*/
	void setUUID(const char* uuid);
	//! Génère un UUID
	/*!
		\param uuid Pointeur pour renvoyer l'UUID.
		\param length Longueur en octet de l'UUID à générer.
	*/
	static void generateRandomUUID(char* uuid, int length);

	// Public attributes (yes we dare!)

	ostream     out; /*!< Flux virtuel pour les STD:OUT.  */
	ostream     err;/*!< Flux virtuel pour les STD:ERR ou erreur.  */

protected: // Protected methods

	int sendback(long retcode, const byte* buffer, int size);

	/**
	 * @brief Méthode interme pour traiter les retours de requêtes.
	 * 
	 * @param inputBuffer 
	 * @return true 
	 * @return false 
	 */
	bool receive(byte * inputBuffer);

	// Attributes

	Stream*     m_stream;/*!< Stream de communication utilisé par SerialTalks.*/
	bool		m_connected;/*!< Représente l'état de connection.*/

	Instruction	m_instructions[SERIALTALKS_MAX_OPCODE];/*!< Listes des instructions enregistrées avec un OPCode associé.*/
	Processing  m_processings[SERIALTALKS_MAX_PROCESSING];;/*!< Listes des instructions de retour enregistrées avec un OPCode associé.*/

	byte        m_inputBuffer [SERIALTALKS_INPUT_BUFFER_SIZE];/*!< Buffer d'entrée d'informations.*/
	byte        m_outputBuffer[SERIALTALKS_OUTPUT_BUFFER_SIZE];/*!< Buffer de sortie d'informations.  */

	enum //     m_state
	{
		SERIALTALKS_WAITING_STATE,///<En attente de l'arrivé d'un octet.
		SERIALTALKS_INSTRUCTION_STARTING_STATE,///< En attente du prochain octet de la requête correspondant à la taille de celle-ci.
		SERIALTALKS_CRC_RECIEVING_STATE, ///< En attente du hash d'intégrité.
		SERIALTALKS_INSTRUCTION_RECEIVING_STATE, ///< Réception des derniers octet de la requête.
	}           m_state;/// Différents états de réception.

	enum// m_order
	{
		SERIALTALKS_ORDER, ///< Requête reçu de la raspberry.
		SERIALTALKS_RETURN, ///< Retour de requête.
	}	m_order; /// Type de requête reçu.
	
	byte        m_bytesNumber;/*!< Variable pour la réception de données qui correspond à la longueur de la requête en bytes (valeur donnée dans le deuxième byte d'une requête).*/
	byte        m_bytesCounter;/*!< Variable d'incrementation pour la réception de données.*/
	long        m_lastTime;/*!< Timeout pour la réception d'octets d'une même requête.*/
	unsigned long m_lastRetcode;
	// for cyclic redundancy check
	CRC16 m_crc;

	byte m_crcBytesCounter;
	uint16_t received_crc_value;

	byte m_crc_tab[SERIALTALKS_CRC_SIZE+1];
	byte m_crc_tmp[SERIALTALKS_OUTPUT_BUFFER_SIZE];

private:
	/**
	 * @brief Méthode interne pour demander le réenvoie de la requête.
	 * 
	 */
	void launchResend(void);
	/**
	 * @brief Indique à la Raspberry que le buffer est vide.
	 */
	void freeBuffer(void);

	//! Méthode pour la requête de ping.
	static void PING   (SerialTalks& talks, Deserializer& input, Serializer& output);
	//! Méthode pour la requête d'UUID.
	static void GETUUID(SerialTalks& talks, Deserializer& input, Serializer& output);
	//! Méthode pour la requête de changement d'UUID.
	static void SETUUID(SerialTalks& talks, Deserializer& input, Serializer& output);
	//! Méthode pour lire dans l'EEPROM.
	static void GETEEPROM(SerialTalks& talks, Deserializer& input, Serializer& output);
	//! Méthode pour écire dans l'EEPROM.
	static void SETEEPROM(SerialTalks& talks, Deserializer& input, Serializer& output);
	//! Méthode pour récuperer la taille du Buffer.
	static void GETBUFFERSIZE(SerialTalks& talks, Deserializer& input, Serializer& output);
};

extern SerialTalks talks;

#endif // __SERIALTALKS_H__
