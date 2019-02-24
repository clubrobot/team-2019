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
	m_mutex.acquire();

    m_pos			= 0;
    m_velInput		= 0;
	m_posInput		= 0;
	m_offset 	    = 0;

	m_arrived = false;
	m_error_occur = false;

	m_mutex.release();
}

void MotorWrapper::process(float timestep)
{
	m_mutex.acquire();

	static float vel = 0;
	/* TODO : add asservissement and control reached position*/
	if(m_step_counter < (m_vel_profile.size()) && !m_error_occur)
	{
		vel = m_vel_profile[m_step_counter++];
		vel = (vel * 180)/M_PI;
		vel = vel * RMP_TO_DEG_S;

		vel = saturate(vel,400,600);

		try
		{
			m_motor->moveSpeed(m_pos + m_offset, vel);
		}
        catch(const AX12Timeout& e)
        {
            m_state.id      = e.get_id();
			m_state.timeout = 1;
			//m_error_occur   = true;
        }
        catch(const AX12error& e)
        {
			m_state.id       = e.get_id();
			m_state.err_code = e.get_error_code();
			//m_error_occur   = true;
        }
	}
	else if(!m_error_occur)
	{
		m_arrived = true;
	}

	m_mutex.release();
}

}