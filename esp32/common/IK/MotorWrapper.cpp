#include <EEPROM.h>
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
	
static bool float_equals(float a, float b, float epsilon = 0.001)
{
    return std::abs(a - b) < epsilon;
}


MotorWrapper::MotorWrapper()
{
	_mutex.acquire();

    _pos			= 0;
    _velInput		= 0;
	_posInput		= 0;
	_offset 	    = 0;

	_arrived = false;

	_mutex.release();
}

void MotorWrapper::setID(int id)
{
	_mutex.acquire();
	_id = id;
	_mutex.release();
}

void MotorWrapper::setOFFSET(float offset)
{
	_mutex.acquire();

	_offset = offset;

	_mutex.release();
}

void MotorWrapper::init()
{
	_mutex.acquire();

	_motor.attach(_id);

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

void MotorWrapper::setVelocityProfile(vector<float> vel)
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
	if(_step_counter < (_vel_profile.size()))
	{
		vel = _vel_profile[_step_counter++];
		vel = (vel * 180)/M_PI;
		vel = vel * RMP_TO_DEG_S;

		vel = saturate(vel,400,600);

		try
		{
			_motor.moveSpeed(_pos + _offset, vel);
		}
        catch(AX12Timeout const& e)
        {
            _state.id      = e.get_id();
			_state.timeout = 1;
        }
        catch(AX12error const& e)
        {
			_state.id       = e.get_id();
			_state.err_code = e.get_error_code();
        }

		/* check position to break loop if the joint is arrived before estimation */
		if(float_equals(_motor.readPosition(), (_pos + _offset), 1.0))
		{
			_step_counter = _vel_profile.size();
			_arrived = true;
		}
	}
	else
	{
		_arrived = true;
	}

	_mutex.release();
}

void MotorWrapper::load(int address)
{
	_mutex.acquire();
	EEPROM.get(address, _id); 			address += sizeof(_id);
	EEPROM.get(address, _offset);    	address += sizeof(_offset);
	_mutex.release();
}

void MotorWrapper::save(int address) const
{
	_mutex.acquire();
	EEPROM.put(address, _id); 			address += sizeof(_id);
	EEPROM.put(address, _offset);    	address += sizeof(_offset);
	EEPROM.commit();
	_mutex.release();
}

}