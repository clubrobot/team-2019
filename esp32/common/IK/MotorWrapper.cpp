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
	_mutex.acquire();

    _pos			= 0;
    _velInput		= 0;
	_posInput		= 0;
	_offset 	    = 0;

	_arrived = false;
	_error_occur = false;

	_mutex.release();
}

void MotorWrapper::attach(int id)
{
	_mutex.acquire();

	_motor.attach(id);

	_mutex.release();
}

void MotorWrapper::setOFFSET(float offset)
{
	_mutex.acquire();

	_offset = offset;

	_mutex.release();
}

void MotorWrapper::setGoalPos(float pos)
{
	_mutex.acquire();

	_pos = pos;
	_step_counter = 0;
	_arrived = false;

	_mutex.release();
}

void MotorWrapper::setVelocityProfile(vector<double> vel)
{
	_mutex.acquire();
	_vel_profile = vel;
	_mutex.release();
}


void MotorWrapper::process(float timestep)
{
	_mutex.acquire();

	static float vel = 0;
	/* TODO : add asservissement and control reached position*/
	if(_step_counter < (_vel_profile.size()) && !_error_occur)
	{
		vel = _vel_profile[_step_counter++];
		vel = (vel * 180)/M_PI;
		vel = vel * RMP_TO_DEG_S;

		vel = saturate(vel,400,600);

		try
		{
			_motor.moveSpeed(_pos + _offset, vel);
		}
        catch(const AX12Timeout& e)
        {
            _state.id      = e.get_id();
			_state.timeout = 1;
			//_error_occur   = true;
        }
        catch(const AX12error& e)
        {
			_state.id       = e.get_id();
			_state.err_code = e.get_error_code();
			//_error_occur   = true;
        }
	}
	else if(!_error_occur)
	{
		_arrived = true;
	}

	_mutex.release();
}

}