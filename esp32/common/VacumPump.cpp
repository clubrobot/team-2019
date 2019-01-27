#include "VacumPump.h"

void VacumPump::startPump()
{
    m_mutex.acquire();
    digitalWrite(m_vacumPin, HIGH);
    m_mutex.release();
}
void VacumPump::stopPump()
{
    m_mutex.acquire();
    digitalWrite(m_vacumPin, LOW);
    m_mutex.release();
}
void VacumPump::startSluice()
{
    m_mutex.acquire();
    digitalWrite(m_sluicePin, HIGH);
    m_mutex.release();
}
void VacumPump::stopSluice()
{
    m_mutex.acquire();
    digitalWrite(m_sluicePin, LOW);
    m_mutex.release();
}