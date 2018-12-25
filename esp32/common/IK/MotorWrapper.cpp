#include "MotorWrapper.h"
#include <iostream>
using namespace std;
#ifdef IK_LOG
    #define LOG_MOTOR(arg) cout << __TIME__<<" (MOTOR WRAPPER)("<< __func__ << " , " << __LINE__ << ")\t\t\t: "<< arg <<endl;
#else
    #define LOG_MOTOR(arg) 
#endif

namespace IK
{

MotorWrapper::MotorWrapper()
{
    m_th1 = 0;
    m_th2 = 0;
    m_th3 = 0;

    m_offset1 = 0;
    m_offset2 = 0;
    m_offset3 = 0;
}
        
void MotorWrapper::attach(int id1, int id2, int id3)
{
    m_AX1.attach(id1);
    m_AX2.attach(id2);
    m_AX3.attach(id3);

    m_AX_Broadcast.attach(254);
}

void MotorWrapper::detach()
{
    m_AX1.detach();
    m_AX2.detach();
    m_AX3.detach();

    m_AX_Broadcast.detach();
}

void MotorWrapper::init()
{
    AX12::SerialBegin(1000000, 5);

    //m_AX_Broadcast.setRDT(0x250);
    m_AX_Broadcast.setVoltageLimit(0,250);

    m_AX_Broadcast.setLEDAlarm(32); // max torque only

    m_AX_Broadcast.setSRL(2); 

    m_AX_Broadcast.setShutdownAlarm(32); // max torque only

    m_AX_Broadcast.setMaxTorque(1024);
    m_AX_Broadcast.setMaxTorqueRAM(1024);

    m_AX_Broadcast.setPunch(0X20);

    m_AX_Broadcast.setEndlessMode(OFF);

    m_AX_Broadcast.setCMargin(0X00,0X00); //Compliance margin
	m_AX_Broadcast.setCSlope(0X10,0X10);  //compliance slope

    m_AX_Broadcast.hold(OFF);

    LOG_MOTOR("AX12 INIT");
}

void MotorWrapper::init_offsets(double offset1, double offset2, double offset3)
{
    m_offset1 = offset1;
    m_offset2 = offset2;
    m_offset3 = offset3;
    LOG_MOTOR("OFFSET INIT: ("<<m_offset1<<","<<m_offset2<<","<<m_offset3<<")");     
}

void MotorWrapper::move(double th1, double th2, double th3)
{
    m_th1 = th1 + m_offset1;
    m_th2 = th2 + m_offset2;
    m_th3 = th3 + m_offset3;

    delta_pos1 = delta_pos2 = delta_pos3 = 0;

    LOG_MOTOR("th1 : "<<m_th1<<"\t|th2 : "<<m_th2<<"\t|th3 : "<<m_th3);

    m_AX1.move(m_th1);
    m_AX2.move(m_th2);
    m_AX3.move(m_th3);

    m_cur_pos1 = m_AX1.readPosition();
    m_cur_pos2 = m_AX2.readPosition();
    m_cur_pos3 = m_AX3.readPosition();

}

void MotorWrapper::moveSpeed(double th1, double th1_speed, double th2, double th2_speed, double th3, double th3_speed)
{
    m_th1 = th1 + m_offset1;
    m_th2 = th2 + m_offset2;
    m_th3 = th3 + m_offset3;

    m_AX1.moveSpeed(m_th1, th1_speed);
    m_AX2.moveSpeed(m_th2, th2_speed);
    m_AX3.moveSpeed(m_th3, th3_speed);
}

bool MotorWrapper::equals(double a, double b, double epsilon)
{ 
    return ((std::abs(a - b) < epsilon));
}

bool MotorWrapper::position_reached()
{

}
bool MotorWrapper::converge_to_pos()
{
    bool ret1, ret2, ret3 = false;
    static double off_th1, off_th2, off_th3;

    if(!equals(m_cur_pos1,m_th1))
    {
        m_cur_pos1 = m_AX1.readPosition();

        delta_pos1 += (m_th1 - m_cur_pos1);

        if(delta_pos1 < 5)
        {
            off_th1 = (0.01 * delta_pos1);
        }
        m_AX1.move(m_th1 + off_th1);

    }
    else
    {
        ret1 = true;
    }
    

    if(!equals(m_cur_pos2,m_th2))
    {
        m_cur_pos2 = m_AX2.readPosition();

        delta_pos2 += (m_th2 - m_cur_pos2);

        if(delta_pos2 < 5)
        {
            off_th2 = (0.01 * delta_pos2);
        }
        m_AX2.move(m_th2 + off_th2);

    }
    else
    {
        ret2 = true;
    }

    if(!equals(m_cur_pos3,m_th3))
    {
        m_cur_pos3 = m_AX3.readPosition();

        delta_pos3 += (m_th3 - m_cur_pos3);

        if(delta_pos3 < 5)
        {
            off_th3 = (0.01 * delta_pos3);
        }
        m_AX3.move(m_th3 + off_th3);

    }
    else
    {
        ret3 = true;
    }
    delay(15);
    return (ret1 & ret2 & ret3);
}

void MotorWrapper::motor_read()
{
    std::cout << m_AX1.readPosition() << std::endl;
    std::cout << m_AX2.readPosition() << std::endl;
    std::cout << m_AX3.readPosition() << std::endl;
    std::cout << std::endl;
}

}