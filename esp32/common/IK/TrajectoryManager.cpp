#include <Arduino.h>
#include "TrajectoryManager.h"
#include "Picker.h"
#include "ArmManager.h"

using namespace IK;

/* task withouth path */
static void task_directly(void * param) 
{
    /* cast input parameter */
    TrajectoryManager* traj_manager = (TrajectoryManager*)param;

    while(!traj_manager->move_directly());

    /* delete task at the end */
    traj_manager->delete_task();
}

static void task_path(void * param) 
{
    /* cast input parameter */
    TrajectoryManager* traj_manager = (TrajectoryManager*)param;

    while(!traj_manager->move_directly());

    /* delete task at the end */
    traj_manager->delete_task();
}

static void task_home(void * param) 
{
    /* cast input parameter */
    TrajectoryManager* traj_manager = (TrajectoryManager*)param;

    while(!traj_manager->move_path());

    /* delete task at the end */
    traj_manager->delete_task();
}

namespace IK
{

/* go directly to pos */
double TrajectoryManager::goto_directly(double x, double y, double phi)
{
    double time_to_arrival;

    m_mutex.acquire();

    /* set_parameters */
    m_start_vel     = NULL_VEL;
    m_end_coord.x   = x;
    m_end_coord.y   = y;
    m_end_coord.phi = phi;
  
    m_start_coord   = get_tool();

    std::cout << "start : " << m_start_coord   << std::endl;
    std::cout << "end : " << m_end_coord << std::endl;

    /* compute an estimation of trajectory time */
    time_to_arrival = estimated_time_of_arrival(m_start_coord, m_start_vel, m_end_coord, m_end_vel);

    if(create_task(task_directly, this))
    {
        set_status(ON_THE_ROAD);
    }
    else
    {
        set_status(ERROR); 
        time_to_arrival = -1;
    }

    m_mutex.release();

    return time_to_arrival;
}

/* go to pos with path */
double TrajectoryManager::goto_path(double x, double y, double phi)
{
    double time_to_arrival;

    m_mutex.acquire();

    /* set_parameters */
    m_start_vel     = NULL_VEL;
    m_end_coord.x   = x;
    m_end_coord.y   = y;
    m_end_coord.phi = phi;

    m_start_coord   = get_tool();

    std::cout << "start : " << m_start_coord   << std::endl;
    std::cout << "end : " << m_end_coord << std::endl;

    /* compute an estimation of trajectory time */
    time_to_arrival = estimated_time_of_arrival(m_start_coord, m_start_vel, m_end_coord, m_end_vel);

    if(create_task(task_path, this))
    {
        set_status(ON_THE_ROAD);
    }
    else
    {
        set_status(ERROR); 
        time_to_arrival = -1;
    }

    m_mutex.release();

    return time_to_arrival;
}

double TrajectoryManager::goto_home()
{
    double time_to_arrival;

    m_mutex.acquire();
    
    /* set_parameters */
    m_start_vel     = NULL_VEL;
    m_start_coord   = get_tool();

    std::cout << "start : " << m_start_coord   << std::endl;
    std::cout << "end : " << m_end_coord << std::endl;

    if(create_task(task_home, this))
    {
        set_status(ON_THE_ROAD); 
    }
    else
    {
        set_status(ERROR); 
        time_to_arrival = -1;
    }

    m_mutex.release();

    return time_to_arrival;
}

void TrajectoryManager::set_status(status_t status) throw()
{
    m_mutex.acquire();
    m_status = status;
    m_mutex.release();
}

status_t TrajectoryManager::get_status() const throw()
{
    status_t ret;

    m_mutex.acquire();

    ret = m_status;

    m_mutex.release();

    return ret;
}

bool TrajectoryManager::move_directly()
{
    m_mutex.acquire();

    m_mutex.release();
}

bool TrajectoryManager::move_path()
{
    m_mutex.acquire();

    m_mutex.release();
}

bool TrajectoryManager::move_home()
{
    m_mutex.acquire();

    m_mutex.release();
}

double TrajectoryManager::convert_deg(double theta)
{
    double th;

    th =  theta * (180.0 / M_PI);

    return th;
}
double TrajectoryManager::convert_speed(double theta_speed)
{
    double speed;

    speed = theta_speed * ( 60.0 / (2.0 * M_PI));

    return speed;
}

} /* end of namespace */