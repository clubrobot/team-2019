#include "MotorWrapper.h"
#include "mathutils.h"
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
    m_pos			= 0;
    m_velInput		= 0;
	m_posInput		= 0;
	m_offset 	    = 0;

	m_arrived = false;
}

void MotorWrapper::process(float timestep)
{
	static float vel = 0;
	/* TODO : add asservissement */
	if(m_step_counter < (m_vel_profile.size()))
	{
		vel = m_vel_profile[m_step_counter++];
		vel = (vel * 180)/M_PI;
		vel = vel * RMP_TO_DEG_S;

		vel = saturate(vel,1,1024);

		cout << "Pos(deg) : " << m_pos << " | vel(rpm) : " << vel << endl;
		try
		{
			m_motor->move(m_pos + m_offset);
		}
		catch(...)
		{
			cout<<"err"<<endl;
		}
	}
	else
	{
		m_arrived = true;
	}

	
}

}