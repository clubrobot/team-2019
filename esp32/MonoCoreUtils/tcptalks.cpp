#include <Arduino.h>
#include "tcptalks.h"
#include "Pickle.h"
#include <WiFi/src/WiFi.h>


void SWITCH_LED(TCPTalks &inst, UnPickler& input, Pickler& output)
{
    bool var;

    pinMode(2, OUTPUT);

    var = input.load<bool>();
   
    if(var)
    {
        //Serial.println("ON");
        digitalWrite(2, HIGH);

    }
    else 
    {
        //Serial.println("false");
        digitalWrite(2, LOW);
    }


    //output.dump<bool>(var);
    //output.dump<long>(10);
    output.dump<double>(1.1);
    
    //output.dump<char>(0X02);
    output.dump<long>(11);

}

TCPTalks::TCPTalks(char* network_ssid, char* network_password, char* server_ip, int server_port)
{
	

    ip = server_ip;
    
	port =  server_port;

	password = "\n";

	m_connected = false;
	m_authentificated = false;

    ssid = network_ssid;
    pass = network_password;

}

void TCPTalks::connect(int timeout)
{
    //Serial.print("Connecting to ");
    //Serial.println(ssid);
    /* connect to your WiFi */
    WiFi.begin(ssid, pass);
    /* wait until ESP32 connect to WiFi*/

    long last_time = millis();
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(5);
        //Serial.print(".");
        long current_time = millis();
        if(current_time - last_time > timeout )
        {
            //Serial.println("Connexion Failed");
            return;
        }
    }

    //Serial.println("");
    //Serial.println("WiFi connected with IP address: ");
    //Serial.println(WiFi.localIP());
    //Serial.println("");

    last_time = millis();
    //Serial.print("wait for server...");

    while(!client.connect(ip, port))
    {
        //Serial.print(".");
        delay(5);
        long current_time = millis();
        if(current_time - last_time > timeout )
        {
            //Serial.println("Connexion Failed");
            return;
        }
       
    }

    m_connected = true;
    //Serial.println("connected");

    last_time = millis();
    //Serial.println("wait for authentification...");

    authentificate();

    while(!is_authentificated())
    {
        //Serial.print(".");
        delay(5);
        long current_time = millis();
        if(current_time - last_time > timeout )
        {
            //Serial.println("authentification Failed");
            return;
        }
       
    }

    //Serial.println("authentificated");
}

void TCPTalks::authentificate()
{
    sendback(AUTHENTIFICATION_OPCODE,NOT_RETCODE,(byte*)password);
}

void TCPTalks::disconnect()
{	
	client.stop();
    
	m_connected = false;
}

void TCPTalks::bind(uint8_t opcode, Instruction instruction)
{
	// Add a command to execute when receiving the specified opcode
	if (opcode < TCPTALKS_MAX_OPCODE)
		m_instructions[opcode] = instruction;
}

void TCPTalks::send(uint8_t opcode, uint8_t* args)
{
    long retcode = random(0, 0xFFFFFFFF);
}

bool TCPTalks::execinstruction(uint8_t* inputBuffer)
{
    UnPickler input(inputBuffer);
    Pickler   output(m_outputBuffer);

    long opcode = input.load<long>();

    //Serial.print("opcode : ");
    //Serial.println(opcode, HEX);

    long retcode = input.load<long>();

    //Serial.print("retcode : ");
    //Serial.println(retcode);

    if(input.is_tuple())
        input.remove_tuple_header();

    if (m_instructions[opcode] != 0)
    {
        m_instructions[opcode](*this, input, output);

        output.end_frame();

        sendback(NOT_OPCODE, retcode, m_outputBuffer);

        return true;
    }
    return false;
}

bool TCPTalks::execute()
{
    bool ret = false;

    int length = client.available();

    long currentTime = millis();
    if (m_state != TCPTALKS_WAITING_STATE && currentTime - m_lastTime > 100) // 0.1s timeout
    {
        // Abort previous communication
        m_state = TCPTALKS_WAITING_STATE;
    }

    for (int i = 0; i < length; i++)
    {
        // Read the incoming byte
        byte inc;

        client.read(&inc , 1);

        //Serial.println(inc,HEX);
        m_lastTime = currentTime;
        // Use a state machine to process the above byte
        switch (m_state)
        {
        // An instruction always begin with the Master byte
        case TCPTALKS_WAITING_STATE:

            if (inc == TCPTALKS_MASTER_BYTE)
            {
                m_state = TCPTALKS_INSTRUCTION_RECEIVING_STATE;
                m_bytesCounter = 0;
            }

            if (inc == AUTHENTIFICATION_OPCODE)
            {
                m_state = TCPTALKS_AUTHENTIFICATION_STATE;
                m_bytesCounter = 0;
            }
           
            //Serial.println("wait...");
            continue;

        case TCPTALKS_INSTRUCTION_RECEIVING_STATE:

            m_inputBuffer[m_bytesCounter] = inc;
            m_bytesCounter++;

            //Serial.println("exec");
            if(inc == '.')
            {
                //Serial.println();
                m_connected = true;
                ret |= execinstruction(m_inputBuffer);
                m_state = TCPTALKS_WAITING_STATE;
            }
            break;

        case TCPTALKS_AUTHENTIFICATION_STATE:

            m_inputBuffer[m_bytesCounter] = inc;
            m_bytesCounter++;

            if(inc == '.')
            {
                //Serial.print("is_authentificated : ");
                //Serial.println(m_authentificated);

                UnPickler input(m_inputBuffer);
                m_authentificated = input.load<bool>();
                m_state = TCPTALKS_WAITING_STATE;
                
                //Serial.print("is_authentificated : ");
                //Serial.println(m_authentificated);
            }
            break;
        }
    }
    return ret;
}

bool TCPTalks::is_connected()
{
    return m_connected;
}

bool TCPTalks::is_authentificated()
{
    return m_authentificated;
}

int TCPTalks::sendback(uint8_t opcode, long retcode, byte * args)
{
    int ptr = 0;
    size_t size;

    byte frame[TCPTALKS_OUTPUT_BUFFER_SIZE];

    /*start header frame*/
    frame[ptr] = PROTO; 
    ptr++;
    frame[ptr] = DEFAULT_PROTOCOL;
    ptr++;
    frame[ptr] = SHORT_BINBYTES;
    ptr++;
    frame[ptr] = 0X01;
    ptr++;
    frame[ptr] = TCPTALKS_SLAVE_BYTE;
    ptr++;
    frame[ptr] = BINPUT;
    ptr++;
    frame[ptr] = 0X00;
    ptr++;
    /* end header frame */

    /* ADD opcode if it defined */
    if(opcode != NOT_OPCODE)
    {
        frame[ptr] = BININT1;
        ptr++;
        frame[ptr] = opcode;
        ptr++;
    }
    else if(retcode != NOT_RETCODE)
    {
        frame[ptr] = (byte)LONG1;
        ptr++;
        frame[ptr] = (byte)0X05;
        ptr++;

        byte tab[4] = {0};
        /* add retcode */
        memcpy(tab,(byte*)&retcode, sizeof(retcode));

        frame[ptr] = tab[0];
        ptr++;
        frame[ptr] = tab[1];
        ptr++;
        frame[ptr] = tab[2];
        ptr++;
        frame[ptr] = tab[3];
        ptr++;
        frame[ptr] = 0X00;
        ptr++;

    }


    if(strlen((char*)args) <= 3)
    {
        frame[ptr] = NONE;
        ptr++;
        frame[ptr] = TUPLE1; 
        ptr++;
        frame[ptr] = BINPUT;
        ptr++;
        frame[ptr] = 0X01;
        ptr++;
    }
    else
    {
        /* get argument size */
        size = strlen((char*)args);
        //Serial.println(size);

        if(args[size - 3] == TUPLE)
        {
            //Serial.println("big tuple");

            uint8_t tmp[MAX_BUFFER_SIZE];
            
            tmp[0] = (uint8_t)MARK;
            memcpy(tmp+1, args, size);

            memcpy(args, tmp, (size+1));

            size = strlen((char*)args);
        }

        memcpy(frame+ptr, args, size);

        ptr += size;
    }


    /*ending frame*/
    frame[ptr] = TUPLE3;
    ptr++;
    frame[ptr] = BINPUT;
    ptr++;
    frame[ptr] = 0X02;
    ptr++;
    frame[ptr] = STOP;
    ptr++;

    client.write(frame, ptr);

    for(int i = 0; i<=ptr;i++)
    {
        //Serial.print(frame[i],HEX);
        //Serial.print(" ");
    }

    //Serial.println("");
    
}