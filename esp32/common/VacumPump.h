#ifndef __VACUMPUMP_H
#define __VACUMPUMP_H

#include <Arduino.h>
#include "thread_tools.h"

class VacumPump
{
    public :
        VacumPump(int vacumPin, int sluicePin): m_vacumPin(vacumPin), m_sluicePin(sluicePin){pinMode(m_vacumPin, OUTPUT); pinMode(m_sluicePin, OUTPUT);}

        void startPump();
        void stopPump();

        void startSluice();
        void stopSluice();

    private:
        int m_vacumPin;
        int m_sluicePin;

        Mutex m_mutex;
};


#endif /* __VACUMPUMP_H */