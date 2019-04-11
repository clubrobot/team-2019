#include <Arduino.h>
#include "../common/PeriodicProcess.h"
#include "configuration.h"

#ifndef IPDISPLAY_H
#define IPDISPLAY_H

#define IP_MAX_NB 10
#define IP_MAX_SIZE 20
#define IP_CHANGE_TIME 2

class Ipdisplay : public PeriodicProcess
{

  public:
    Ipdisplay() : PeriodicProcess(), m_nb_ips(0) {}
    void attach(byte dataPin, byte clockPin, byte latchPin);
    void addIP(char* buffer, byte nbDigits);
    void clearIPs();
    void clearDisplay();
    void computeBuffer(char buffer[], byte nbDigits);

  private:
    char m_toSend[4][3];
    byte m_DATA;
    byte m_CLOCK;
    byte m_LATCH;
    byte m_disp_number; // Number of the effective 7 seg display
    char m_ips[IP_MAX_NB][IP_MAX_SIZE];
    float m_acc_time;
    byte m_nb_ips;
    byte m_current_ip;
    byte m_nb_digits[IP_MAX_NB];

    virtual void process(float timestep);
    void display();
};

#endif