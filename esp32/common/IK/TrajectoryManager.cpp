#include <Arduino.h>
#include <EEPROM.h>
#include "TrajectoryManager.h"
#include "Picker.h"
#include "ArmManager.h"

using namespace IK;
using namespace std;

#ifdef IK_LOG
    #define LOG_TRAJ(arg) cout << __TIME__<<" (TRAJ MANAGER)("<< __func__ << " , " << __LINE__ << ")\t\t: "<< arg <<endl;
#else
    #define LOG_TRAJ(arg) 
#endif

namespace IK
{
void TrajectoryManager::set_armManager(ArmManager& manager)
{
    _mutex.acquire();
    _manager = &manager;
    _mutex.release();
}

void TrajectoryManager::set_Motors(MotorWrapper& motor1, MotorWrapper& motor2, MotorWrapper& motor3)
{
    _mutex.acquire();
    _motor1 = &motor1;
    _motor2 = &motor2;
    _motor3 = &motor3;
    _mutex.release();
}

void TrajectoryManager::set_timestep(float timestep)
{
    _mutex.acquire();
    _timestep = timestep;
    _mutex.release();
}

void TrajectoryManager::init()
{
    _mutex.acquire();
    PeriodicProcess::setTimestep(_timestep);
    _mutex.release();
}

float TrajectoryManager::get_timestep() const 
{
    float ret;

    _mutex.acquire();
    ret =  _timestep;
    _mutex.release();

    return ret;
}

void TrajectoryManager::move_directly(Coords pos)
{
    _mutex.acquire();

    MoveBatch mb;

    /* set_parameters */
    Coords end_coord     = pos;
    Coords start_coord   = _manager->get_tool();

    LOG_TRAJ("start : " << start_coord);
    LOG_TRAJ("end : " << end_coord );

    try
    {
        mb = _manager->go_to(start_coord, end_coord);
        addMoveBatch(mb);
    }
    catch(const string& e)
    {
        cout << e << endl;
    }
    _mutex.release();
}

bool TrajectoryManager::is_arrived() const
{
    bool ret;

    _mutex.acquire();
    ret = _arrived;
    _mutex.release();
    
    return ret;
}

void TrajectoryManager::addMoveBatch(MoveBatch mb)
{
    LOG_TRAJ("ADD BATCH");
    _batchQueue.push(mb);
    _batchQueue.setResetPoint();
}

// NOTE: for debugging purposes.
MoveBatch TrajectoryManager::popMoveBatch()
{
    return _batchQueue.pop();
}

// NOTE: for debugging purposes.
MoveBatch TrajectoryManager::peekMoveBatch()
{
    return _batchQueue.peek();
}

void TrajectoryManager::process(float timestep)
{
    _mutex.acquire();

    static float th1, th2, th3, th1_inter, th2_inter, th3_inter;

    if(!_isExecutingBatch)
    {
        if (_batchQueue.peek().is_active())
        {
            MoveBatch mb = _batchQueue.pop();

            if(mb.is_active())
            {
                th1 = convert_deg(mb.batch[0].position);
                th2 = convert_deg(mb.batch[1].position);
                th3 = convert_deg(mb.batch[2].position);

                _motor1->setVelocityProfile(mb.batch[0].vel);
                _motor2->setVelocityProfile(mb.batch[1].vel);
                _motor3->setVelocityProfile(mb.batch[2].vel);

                _motor1->setGoalPos(th1);
                _motor2->setGoalPos(th2);
                _motor3->setGoalPos(th3);

            }
            _isExecutingBatch = true;
            _arrived          = false;
        }
        else
        {
            _arrived          = true;
            disable();
        }
    }
    else
    {
        _motor1->process(timestep);
        _motor2->process(timestep);
        _motor3->process(timestep);

        if(_motor1->arrived() && _motor2->arrived() && _motor3->arrived())
        {
            _arrived          = true;
            _isExecutingBatch = false;
        }
    }
    
    _mutex.release();
}

float TrajectoryManager::convert_deg(float theta)
{
    float th;

    th =  theta * (180.0 / M_PI);

    return th;
}
float TrajectoryManager::convert_speed(float theta_speed)
{
    float speed;

    speed = theta_speed * ( 60.0 / (2.0 * M_PI));

    return speed;
}

void TrajectoryManager::load(int address)
{
	_mutex.acquire();
	EEPROM.get(address, _timestep);        address += sizeof(_timestep);
	_mutex.release();
}

void TrajectoryManager::save(int address) const
{
	_mutex.acquire();
	EEPROM.put(address, _timestep); address += sizeof(_timestep);
	EEPROM.commit();
	_mutex.release();
}

} /* end of namespace */