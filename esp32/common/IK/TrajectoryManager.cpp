#include <Arduino.h>
#include "TrajectoryManager.h"
#include "Picker.h"
#include "ArmManager.h"

using namespace IK;
using namespace std;

/* task withouth path */
static void task_directly(void * param) 
{
    /* cast input parameter */
    TrajectoryManager* traj_manager = (TrajectoryManager*)param;

    while(!traj_manager->move_directly());

    /* delete task at the end */
    traj_manager->delete_task();
}

namespace IK
{

void TrajectoryManager::attach(int id_1, int id_2, int id_3) throw()
{
    MotorWrapper::attach(id_1, id_2, id_3);
    MotorWrapper::init();
    MotorWrapper::init_offsets(LINK1_OFFSET, LINK2_OFFSET, LINK3_OFFSET);
}

void TrajectoryManager::begin(coords_t initial_pos)
{
    joints_t joints = Picker::inverse_kinematics(initial_pos);
    
    MotorWrapper::move(convert_deg(joints.th1), convert_deg(joints.th2), convert_deg(joints.th3));
}

/* go directly to pos */
double TrajectoryManager::goto_directly(coords_t pos)
{
    double time_to_arrival;

    m_mutex.acquire();

    /* set_parameters */
    m_end_coord     = pos;
  
    m_start_coord   = get_tool();

    std::cout << "start : " << m_start_coord   << std::endl;
    std::cout << "end : " << m_end_coord << std::endl;

    /* compute an estimation of trajectory time */
    time_to_arrival = estimated_time_of_arrival(m_start_coord, NULL_VEL, m_end_coord, NULL_VEL);

    m_current_traj_time = time_to_arrival;

    m_mutex.release();

    if(create_task(task_directly, this))
    {
        set_status(ON_THE_ROAD);
    }
    else
    {
        set_status(ERROR); 
        time_to_arrival = -1;
    }

    return time_to_arrival;
}

void TrajectoryManager::set_status(status_t status) throw()
{
    m_status = status;
}

status_t TrajectoryManager::get_status() const throw()
{
    status_t ret;

    ret = m_status;

    return ret;
}

bool TrajectoryManager::move_directly()
{
    joints_t joints;
    int timeout;

    m_mutex.acquire();

    joints = go_to(m_start_coord, m_end_coord);
    /* set pos and velocity to AX12 motor */
    if(joints.intermediary_pos == true)
    {
        move(convert_deg(joints.th1_int), convert_deg(joints.th2_int), convert_deg(joints.th3_int));
        delay(500);
        // while(!position_reached())
        // {
        //     /* sleep */
        //     delay(100);
        // }  
    }

    move(convert_deg(joints.th1), convert_deg(joints.th2), convert_deg(joints.th3));
    

    /* check error or if position is reached */
    // while(!position_reached())
    // {
    //     /* sleep */
    //     delay(100);
    // }    

    set_status(ARRIVED);

    m_mutex.release();

    return true;
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