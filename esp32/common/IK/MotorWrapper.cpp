#include "MotorWrapper.h"
#include <iostream>

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
    AX12::SerialBegin(1000000, 2);

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

}

void MotorWrapper::init_offsets(double offset1, double offset2, double offset3)
{
    m_offset1 = offset1;
    m_offset2 = offset2;
    m_offset3 = offset3;     
}

void MotorWrapper::move(double th1, double th2, double th3)
{
    
    m_th1 = th1 + m_offset1;
    m_th2 = th2 + m_offset2;
    m_th3 = th3 + m_offset3;

    std::cout << "th1 : " << th1 << std::endl;
    std::cout << "th2 : " << th2 << std::endl;
    std::cout << "th3 : " << th3 << std::endl;

    std::cout << "th1 : " << m_th1 << std::endl;
    std::cout << "th2 : " << m_th2 << std::endl;
    std::cout << "th3 : " << m_th3 << std::endl;
    std::cout << std::endl;

    m_AX1.move(m_th1);
    m_AX2.move(m_th2);
    m_AX3.move(m_th3);
}

void MotorWrapper::moveSpeed(double th1, double th1_speed, double th2, double th2_speed, double th3, double th3_speed)
{
     

    m_th1 = th1 + m_offset1;
    m_th2 = th2 + m_offset2;
    m_th3 = th3 + m_offset3;
    
    std::cout << std::endl;
    std::cout << "th1 : " << m_th1 << std::endl;
    std::cout << "th2 : " << m_th2 << std::endl;
    std::cout << "th3 : " << m_th3 << std::endl;
    std::cout << std::endl;

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
    double a1 = m_AX1.readPosition();
    double a2 = m_AX2.readPosition();
    double a3 = m_AX3.readPosition();

    // std::cout << a1 << std::endl;
    // std::cout << a2 << std::endl;
    // std::cout << a3 << std::endl;
    // std::cout << std::endl;
    
    if(equals(a1,m_th1) && equals(a2,m_th2) && equals(a3,m_th3))
    {
         
        return true;
    }
     
    return false;
}

void MotorWrapper::motor_read()
{
    std::cout << m_AX1.readPosition() << std::endl;
    std::cout << m_AX2.readPosition() << std::endl;
    std::cout << m_AX3.readPosition() << std::endl;
    std::cout << std::endl;
}

}