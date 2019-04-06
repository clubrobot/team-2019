#include <Arduino.h>
#include "IPDisplay.h"
#include "configuration.h"
#include "../common/SerialTalks.h"
#include <EEPROM.h>

extern SerialTalks talks;

void Ipdisplay::attach(byte dataPin, byte clockPin, byte latchPin)
{
	clearDisplay();
	m_DATA = dataPin;
	m_CLOCK = clockPin;
	m_LATCH = latchPin;
	m_disp_number = 0;

	// sets the pins as output
	pinMode(m_DATA, OUTPUT);
	pinMode(m_CLOCK, OUTPUT);
	pinMode(m_LATCH, OUTPUT);

	for (int i = 0; i < DISP_NUMBER; i++)
	{
		pinMode(DISP_PIN[i], OUTPUT);
		digitalWrite(DISP_PIN[i], LOW);
	}
}

void Ipdisplay::process(float timestep)
{
	m_acc_time += timestep;
	if(m_acc_time > IP_CHANGE_TIME)
	{
		if (m_nb_ips == 0)
		{
			this->clearDisplay();
		}
		else
		{
			m_acc_time = 0;
			m_current_ip++;
			m_current_ip %= m_nb_ips;
			this->computeBuffer(m_ips[m_current_ip], m_nb_digits[m_current_ip]);
		}
		
	}

	digitalWrite(DISP_PIN[m_disp_number], LOW); // Turn the off the previous 7 seg display
	m_disp_number++;
	if (m_disp_number >= 3)
		m_disp_number = 0;

	digitalWrite(LATCH_IPDISPLAY, LOW);
	shiftOut(DATA_IPDISPLAY, CLOCK_IPDISPLAY, MSBFIRST, m_toSend[2][m_disp_number]);
	shiftOut(DATA_IPDISPLAY, CLOCK_IPDISPLAY, MSBFIRST, m_toSend[3][m_disp_number]);
	shiftOut(DATA_IPDISPLAY, CLOCK_IPDISPLAY, MSBFIRST, m_toSend[0][m_disp_number]);
	shiftOut(DATA_IPDISPLAY, CLOCK_IPDISPLAY, MSBFIRST, m_toSend[1][m_disp_number]);
	digitalWrite(LATCH_IPDISPLAY, HIGH);
	digitalWrite(DISP_PIN[m_disp_number], HIGH); // Turn on the next 7 seg display
}

void Ipdisplay::clearIPs()
{
	m_nb_ips = 0;
    m_acc_time = IP_CHANGE_TIME+1;
}

void Ipdisplay::addIP(char* buffer, byte nbDigits)
{
	if (m_nb_ips < IP_MAX_NB && strlen(buffer) <= IP_MAX_SIZE) {
        m_current_ip = 0;
        strcpy(m_ips[m_nb_ips], buffer);
        m_nb_digits[m_nb_ips++] = nbDigits;
    }
    m_acc_time = IP_CHANGE_TIME+1;
}

void Ipdisplay::clearDisplay()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m_toSend[i][j] = 0;
		}
	}
}

void Ipdisplay::computeBuffer(char buffer[], byte nbDigits)
{
	int cpt = 0;
	int shift = (12 - nbDigits) / 2;
	clearDisplay();
	for (int display = 0; display < 4; display++)
	{
		for (int i = 0; i < 3; i++)
		{
			m_toSend[display][i] = 0;
			if ((display)*3 + i + 1 > shift)
			{
				if ((buffer[cpt] >= START_CHAR) && (buffer[cpt] <= END_CHAR) && (display*3+i < shift + nbDigits))
				{
					for (int j = 0; j < 8; j++)
					{
						m_toSend[display][i] += (((EEPROM.read(EEPROM_IPDISPLAY_START_ADDRESS+buffer[cpt]-START_CHAR)) & (0x01 << j)) / (0x01 << j)) * DISP[display][7 - j];
					}
				}

				cpt++;

				if (buffer[cpt] == '.' && buffer[cpt-1] != '.' && (display*3+i < shift + nbDigits))
				{
					m_toSend[display][i] += DISP[display][7];
					cpt++;
				}
			}
		}
	}
}
