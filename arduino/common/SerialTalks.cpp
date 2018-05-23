#include "SerialTalks.h"
#include "Clock.h"
#include <EEPROM.h>


// Global instance

SerialTalks talks;


// Built-in instructions

void SerialTalks::PING(SerialTalks& talks, Deserializer& input, Serializer& output)
{
	output << true;
}

void SerialTalks::GETUUID(SerialTalks& inst, Deserializer& input, Serializer& output)
{
	char uuid[SERIALTALKS_UUID_LENGTH];
	talks.getUUID(uuid);
	output << uuid;
}

void SerialTalks::SETUUID(SerialTalks& inst, Deserializer& input, Serializer& output)
{	
	char uuid[SERIALTALKS_UUID_LENGTH];
	input >> uuid;
	talks.setUUID(uuid);
}

void SerialTalks::GETEEPROM(SerialTalks& inst, Deserializer& input, Serializer& output)
{
	int addr = input.read<int>();
	output.write<byte>(EEPROM.read(addr));
}

void SerialTalks::SETEEPROM(SerialTalks& inst, Deserializer& input, Serializer& output)
{
	int addr = input.read<int>();
	byte value = input.read<byte>();
	EEPROM.update(addr,value);
}

// Built-in Processing 
void SerialTalks::LAUNCHWARNING(unsigned char * message)
{
	Serializer output = getSerializer();
	for(int i = 0;i<3;i++)
	{
		output.write(*(message+i));	
	}
	send(SERIALTALKS_WARNING_OPCODE, output);
}


// SerialTalks::ostream

void SerialTalks::ostream::begin(SerialTalks& parent, long retcode)
{
	m_parent = &parent;
	m_retcode = retcode;
}

size_t SerialTalks::ostream::write(uint8_t c)
{
	uint8_t buffer[2] = {c, '\0'};
	return write(buffer, 2);
}

size_t SerialTalks::ostream::write(const uint8_t *buffer, size_t size)
{
	return m_parent->sendback(m_retcode, buffer, size + 1);
}


// SerialTalks

void SerialTalks::begin(Stream& stream)
{
	// Initialize attributes
	m_stream = &stream;
	m_connected = false;
	out.begin(*this, SERIALTALKS_STDOUT_RETCODE);
	err.begin(*this, SERIALTALKS_STDERR_RETCODE);

	// Initialize UUID stuff
#ifdef BOARD_UUID
	setUUID(BOARD_UUID);
#else
	char uuid[SERIALTALKS_UUID_LENGTH];
	if (!getUUID(uuid) || uuid[0] == '\0')
	{
		generateRandomUUID(uuid, SERIALTALKS_DEFAULT_UUID_LENGTH);
		setUUID(uuid);
	}
#endif // BOARD_UUID

	// Add UUID accessors
	bind(SERIALTALKS_PING_OPCODE,     SerialTalks::PING);
	bind(SERIALTALKS_GETUUID_OPCODE,  SerialTalks::GETUUID);
	bind(SERIALTALKS_SETUUID_OPCODE,  SerialTalks::SETUUID);
	bind(SERIALTALKS_GETEEPROM_OPCODE,SerialTalks::GETEEPROM);
	bind(SERIALTALKS_SETEEPROM_OPCODE,SerialTalks::SETEEPROM);
}

int SerialTalks::send(byte opcode,Serializer output)
{
	int count = 0;
	unsigned long retcode = opcode;
	if (m_stream != 0 && isConnected())
	{
		count += m_stream->write(SERIALTALKS_MASTER_BYTE);
		count += m_stream->write( sizeof(retcode) + output.buffer-m_outputBuffer+sizeof(byte) );

		/*******************************crc computation********************************/
		memcpy(m_crc_tmp,(byte*)&opcode, sizeof(opcode));
		memcpy(m_crc_tmp + sizeof(opcode), (byte*) &retcode, sizeof(retcode));
		memcpy(m_crc_tmp + sizeof(retcode) + sizeof(opcode) , m_outputBuffer, output.buffer-m_outputBuffer);

		uint16_t crc = m_crc.CRCprocessBuffer(m_crc_tmp, (output.buffer-m_outputBuffer) + sizeof(retcode) + sizeof(opcode));

		count += m_stream->write((byte*)(&crc), sizeof(crc));
		/*****************************************************************************/

		count += m_stream->write(opcode);
		count += m_stream->write((byte*)(&retcode), sizeof(retcode));
		count += m_stream->write(m_outputBuffer, output.buffer-m_outputBuffer);	
	}
	return count;
}


int SerialTalks::sendback(long retcode, const byte* buffer, int size)
{
	int count = 0;
	if (m_stream != 0 && isConnected())
	{
		count += m_stream->write(SERIALTALKS_SLAVE_BYTE);
		count += m_stream->write(byte(sizeof(retcode) + size));

		/*******************************crc computation********************************/ 
		memcpy(m_crc_tmp,(byte*) &retcode, sizeof(retcode));
		memcpy(m_crc_tmp + sizeof(retcode), buffer, size);
		
		uint16_t crc = m_crc.CRCprocessBuffer(m_crc_tmp, size + sizeof(retcode));

		count += m_stream->write((byte*)(&crc), sizeof(crc));
		/*****************************************************************************/

		count += m_stream->write((byte*)(&retcode), sizeof(retcode));
		count += m_stream->write(buffer, size);	
	}
	return count;
}

void SerialTalks::bind(byte opcode, Instruction instruction)
{
	// Add a command to execute when receiving the specified opcode
	if (opcode < SERIALTALKS_MAX_OPCODE)
		m_instructions[opcode] = instruction;
}


void SerialTalks::attach(byte opcode, Processing processing)
{
	if (opcode < SERIALTALKS_MAX_PROCESSING)
		m_processings[opcode] = processing;
}


bool SerialTalks::receive(byte* inputBuffer)
{
	Deserializer input (inputBuffer);
	byte retcode = (byte) input.read<long>();
	if(m_processings[retcode]!=0)
	{
		m_processings[retcode](*this, input);
		return true;
	}
	return false;

}

bool SerialTalks::execinstruction(byte* inputBuffer)
{
	Deserializer input (inputBuffer);
	Serializer   output(m_outputBuffer);
	byte opcode = input.read<byte>();
	long retcode = input.read<long>();
	if (m_instructions[opcode] != 0)
	{
		m_instructions[opcode](*this, input, output);
		if (output.buffer > m_outputBuffer)
			sendback(retcode, m_outputBuffer, output.buffer - m_outputBuffer);
		return true;
	}
	return false;
}

bool SerialTalks::execute()
{
	bool ret = false;
	int length = m_stream->available();

	long currentTime = millis();
	if (m_state != SERIALTALKS_WAITING_STATE && currentTime - m_lastTime > 100) // 0.1s timeout
	{
		// Abort previous communication
		m_state = SERIALTALKS_WAITING_STATE;
	}

	for (int i = 0; i < length; i++)
	{
		// Read the incoming byte
		byte inc = byte(m_stream->read());
		m_lastTime = currentTime;
		
		
		// Use a state machine to process the above byte
		switch (m_state)
		{
		// An instruction always begin with the Master byte
		case SERIALTALKS_WAITING_STATE:
			if (inc == SERIALTALKS_MASTER_BYTE || inc==SERIALTALKS_SLAVE_BYTE)
				m_state = SERIALTALKS_INSTRUCTION_STARTING_STATE;
				m_order = (inc == SERIALTALKS_MASTER_BYTE) ?
					SERIALTALKS_ORDER :
					SERIALTALKS_RETURN;
			continue;

		// The second byte is the instruction size (for example: 'R', '\x02', '\x03', '\x31')
		case SERIALTALKS_INSTRUCTION_STARTING_STATE:
			m_bytesNumber  = inc;
			m_bytesCounter = 0;
			m_crcBytesCounter = 0;
			m_state = (m_bytesNumber <= SERIALTALKS_INPUT_BUFFER_SIZE) ?
				SERIALTALKS_CRC_RECIEVING_STATE :
				SERIALTALKS_WAITING_STATE;
			continue;
		
		case SERIALTALKS_CRC_RECIEVING_STATE : 
			m_crc_tab[m_crcBytesCounter++] = inc;
			if(m_crcBytesCounter >= SERIALTALKS_CRC_SIZE)
			{
				Deserializer incrc(m_crc_tab);
			 	received_crc_value = incrc.read<uint16_t>();
			 	m_state = SERIALTALKS_INSTRUCTION_RECEIVING_STATE;
			}
			continue;

		// The first instruction byte is the opcode and the others the parameters
		case SERIALTALKS_INSTRUCTION_RECEIVING_STATE:
			m_inputBuffer[m_bytesCounter++] = inc;
			if (m_bytesCounter >= m_bytesNumber)
			{
				//Checking frame integrity(CRC)
				if(m_crc.CRCcheck(m_inputBuffer,m_bytesNumber,received_crc_value))
				{
        			m_connected = true;
					if(m_order==SERIALTALKS_ORDER) ret |= execinstruction(m_inputBuffer);
					else if (m_order==SERIALTALKS_RETURN) ret |= receive(m_inputBuffer);
					m_state = SERIALTALKS_WAITING_STATE;
				}
				else
				{
					unsigned char message_warning[3];
					message_warning[0] = (unsigned char) (received_crc_value);
					message_warning[1] = (unsigned char) (received_crc_value>>8);
					message_warning[2] = (unsigned char) 0;
					LAUNCHWARNING(message_warning);
					m_state = SERIALTALKS_WAITING_STATE;
				}	
			}
			continue;
		}
	}
	return ret;
}

bool SerialTalks::waitUntilConnected(float timeout)
{
	Clock clock;
	clock.restart();
	while (!isConnected() || (timeout > 0 && clock.getElapsedTime() < timeout))
		execute();
	return isConnected();
}

bool SerialTalks::getUUID(char* uuid)
{
	for (int i = 0; i < int(EEPROM.length()); i++)
	{
		uuid[i] = EEPROM.read(SERIALTALKS_UUID_ADDRESS + i);
		switch(byte(uuid[i]))
		{
		case '\0': return true;
		case 0xFF: return false;
		default  : continue;
		}
	}
	return false;
}

void SerialTalks::setUUID(const char* uuid)
{
	int i = 0;
	do
		EEPROM.write(SERIALTALKS_UUID_ADDRESS + i, uuid[i]);
	while(uuid[i++] != '\0');
}

void SerialTalks::generateRandomUUID(char* uuid, int length)
{
	// Initialize the random number generator
	randomSeed(analogRead(0));

	// Generate the UUID from a list of random hexadecimal numbers
	for (int i = 0; i < length; i++)
	{
		if (i % 5 == 4)
			uuid[i] = '-';
		else
		{
			long digit = random(16);
			if (digit < 10)
				uuid[i] = char('0' + digit);
			else
				uuid[i] = char('a' + digit - 10);
		}
	}
	uuid[length] = '\0';
}
