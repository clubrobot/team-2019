#ifndef __TCPTALKS_H__
#define __TCPTALKS_H__

#include <Arduino.h>
#include "Pickle.h"
#include <WiFi/src/WiFi.h>

#ifndef TCPTALKS_INPUT_BUFFER_SIZE
#define TCPTALKS_INPUT_BUFFER_SIZE 64
#endif

#ifndef TCPTALKS_OUTPUT_BUFFER_SIZE
#define TCPTALKS_OUTPUT_BUFFER_SIZE 64
#endif

#ifndef TCPTALKS_MAX_OPCODE
#define TCPTALKS_MAX_OPCODE 0x20
#endif

#define TCPTALKS_MASTER_BYTE (uint8_t)'R'
#define TCPTALKS_SLAVE_BYTE  (uint8_t)'A'

#define AUTHENTIFICATION_OPCODE 0xAA

#define NOT_OPCODE  0XFF
#define NOT_RETCODE 0XFE

#define TCPTALKS_BAUDRATE 115200

typedef uint8_t byte;


class TCPTalks
{
	public: // Public API

	TCPTalks(char* network_ssid, char* network_password,char* server_ip, int server_port);

	typedef void (*Instruction)(TCPTalks& inst, UnPickler& input, Pickler& output);

	void connect(int timeout);

	void authentificate();
		
	void disconnect();
		
	void bind(uint8_t opcode, Instruction instruction);
		
	void send(uint8_t opcode, uint8_t* args);

	void sendback(int retcode, uint8_t* args);

	void process(uint8_t* message);
		
	bool execinstruction(uint8_t* args);
		
	bool execute();
	
	void sleep_until_disconnected();

	int sendback(uint8_t opcode, long retcode, byte * args);

	bool is_connected();

	bool is_authentificated();

		
	protected: // Protected methods

	char* ip;

	int port;

	char* password = "";

	bool m_connected;
	bool m_authentificated;

	char* ssid = "";
	char* pass = "";

	WiFiClient client;

	Instruction	m_instructions[TCPTALKS_MAX_OPCODE];

	enum //     m_state
	{
		TCPTALKS_WAITING_STATE,
		TCPTALKS_INSTRUCTION_STARTING_STATE,
		TCPTALKS_INSTRUCTION_RECEIVING_STATE,
		TCPTALKS_AUTHENTIFICATION_STATE,
	}m_state;


	byte        m_bytesNumber;
	byte        m_bytesCounter;
	long        m_lastTime;

	byte  m_inputBuffer [TCPTALKS_INPUT_BUFFER_SIZE];
	byte  m_outputBuffer[TCPTALKS_OUTPUT_BUFFER_SIZE];



};
void SWITCH_LED(TCPTalks& inst, UnPickler& input, Pickler& output);
#endif // __TCPTALKS_H__
