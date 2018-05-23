#include <Arduino.h>
#include "../common/PeriodicProcess.h"
#include "configuration.h"

#ifndef IPDISPLAY_H
#define IPDISPLAY_H

class Ipdisplay : public PeriodicProcess
{

  public:
    void attach(byte dataPin, byte clockPin, byte latchPin);
    void clearDisplay();
    void computeBuffer(char buffer[], byte nbDigits);

  private:
    char m_toSend[4][3];
    byte m_DATA;
    byte m_CLOCK;
    byte m_LATCH;
    byte m_disp_number; // Number of the effective 7 seg display

    virtual void process(float timestep);
    void display();
};

#endif