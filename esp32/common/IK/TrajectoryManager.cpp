#include <Arduino.h>
#include "TrajectoryManager.h"
#include "Picker.h"
#include "ArmManager.h"

using namespace IK;

extern TrajectoryManager traj_manager;

static TaskHandle_t path_handle;
static TaskHandle_t directly_handle;
static TaskHandle_t home_handle;

static bool equals(double* a, double* b, double epsilon = 0.3)
{
    return ((std::abs(a[0] - b[0]) < epsilon) && (std::abs(a[1] - b[1]) < epsilon) && (std::abs(a[2] - b[2]) < epsilon));
}

/* task withouth path */
static void task_directly(void * param) 
{
    trajectory_task_pv_t* task_param = (trajectory_task_pv_t*)param;

    joints_t joints;

    /* compute path for trajectory */
    joints = traj_manager.inverse_kinematics(task_param->end_coord);
   
    /* set pos and velocity to AX12 motor */
    traj_manager.move(CONVERT_DEG(joints.th1), CONVERT_DEG(joints.th2), CONVERT_DEG(joints.th3));

    /* check error or if position is reached */
    while(!traj_manager.position_reached())
    {
        /* sleep */
        delay(100);
    }    

    traj_manager.set_status(ARRIVED);
    
    if( directly_handle != NULL )
    {
        vTaskDelete(directly_handle);
    }
}

static void task_path(void * param) 
{
    /* cast input parameter */
    trajectory_task_pv_t* task_param = (trajectory_task_pv_t*)param;
    static TickType_t xDelay;
    int element_number;
    path_t chemin;

    bool pos_reached = false;
    double pos[3];
    double vel[3];
    double wait_pos[3];

    /* set default delay */
    xDelay= 10 / portTICK_PERIOD_MS;

    /* compute path for trajectory */
    chemin = traj_manager.go_to(task_param->start_coord, task_param->vel , task_param->end_coord , task_param->vel);

    cout << chemin << endl;
    /* get the size of path */
    element_number = chemin.path_th1.t.size();

    // for(int i = 0; i < element_number ; i++)
    // {
    //     pos_reached = false;

    //     pos[0] = CONVERT_DEG(chemin.path_th1.pos[i]);
    //     vel[0] = AX12_SPEED(chemin.path_th1.vel[i]);

    //     pos[1] = CONVERT_DEG(chemin.path_th2.pos[i]);
    //     vel[1] = AX12_SPEED(chemin.path_th2.vel[i]);

    //     pos[2] = CONVERT_DEG(chemin.path_th3.pos[i]);
    //     vel[2] = AX12_SPEED(chemin.path_th3.vel[i]);
        

    //     traj_manager.moveSpeed(pos[0], 1000, pos[1], 1000, pos[2], 1000);

    //     /* check error or if position is reached */
    //     while(!traj_manager.position_reached())
    //     {
    //         /* get waiting time in ms*/
    //         xDelay = (chemin.path_th1.t[i] * 100)/portTICK_PERIOD_MS;
    //         /* sleep */
    //         vTaskDelay(xDelay);
    //     }    
    // }

    traj_manager.set_status(ARRIVED);

    if( path_handle != NULL )
    {
        vTaskDelete(path_handle);
    }
}

static void task_home(void * param) 
{
    /* cast input parameter */
    trajectory_task_pv_t* task_param = (trajectory_task_pv_t*)param;
    static TickType_t xDelay;
    int element_number;
    path_t chemin;

    bool pos_reached = false;
    double pos[3];
    double vel[3];
    double wait_pos[3];

    /* set default delay */
    xDelay= 10 / portTICK_PERIOD_MS;

    /* compute path for trajectory */
    chemin = traj_manager.go_home(task_param->start_coord, task_param->vel);

     /* get the size of path */
    element_number = chemin.path_th1.t.size();

    for(int i = 0; i < element_number ; i++)
    {
        pos_reached = false;

        pos[0] = CONVERT_DEG(chemin.path_th1.pos[i]);
        vel[0] = AX12_SPEED(chemin.path_th1.vel[i]);

        pos[1] = CONVERT_DEG(chemin.path_th2.pos[i]);
        vel[1] = AX12_SPEED(chemin.path_th2.vel[i]);

        pos[2] = CONVERT_DEG(chemin.path_th3.pos[i]);
        vel[2] = AX12_SPEED(chemin.path_th3.vel[i]);
        

        traj_manager.moveSpeed(pos[0], 1000, pos[1], 1000, pos[2], 1000);

        /* check error or if position is reached */
        while(!traj_manager.position_reached())
        {
            /* get waiting time in ms*/
            xDelay = (chemin.path_th1.t[i] * 100)/portTICK_PERIOD_MS;
            /* sleep */
            vTaskDelay(xDelay);
        }    
    }

    traj_manager.set_status(ARRIVED);
    
    if( home_handle != NULL )
    {
        vTaskDelete(home_handle);
    }
}

namespace IK
{

/* go directly to pos */
double TrajectoryManager::goto_directly(double x, double y, double phi)
{
    double time_to_arrival;

    /* set_parameters */
    set_status(ON_THE_ROAD);

    m_task_parameters.vel           = {0.0,0.0,0.0};
    m_task_parameters.end_coord.x   = x;
    m_task_parameters.end_coord.y   = y;
    m_task_parameters.end_coord.phi = phi;

     
    m_task_parameters.start_coord = traj_manager.get_tool();

    std::cout << "start : " << m_task_parameters.start_coord.x << std::endl;
    std::cout << "start : " << m_task_parameters.start_coord.y << std::endl;
    std::cout << "start : " << m_task_parameters.start_coord.phi << std::endl;
    /* compute an estimation of trajectory time */
    time_to_arrival               = traj_manager.estimated_time_of_arrival(m_task_parameters.start_coord,\
                                                                          m_task_parameters.vel,\
                                                                          m_task_parameters.end_coord, \
                                                                          m_task_parameters.vel);

     

    xTaskCreatePinnedToCore(
                    task_directly,      /* Function to implement the task */
                    "goto_directly",    /* Name of the task */
                    8196,               /* Stack size in words */
                    &m_task_parameters, /* Task input parameter */
                    0,                  /* Priority of the task */
                    &directly_handle,   /* Task handle. */
                    RUNNING_CORE);      /* Core where the task should run */

    return time_to_arrival;

}

/* go to pos with path */
double TrajectoryManager::goto_path(double x, double y, double phi)
{
    double time_to_arrival;

    /* set_parameters */
    set_status(ON_THE_ROAD);

    m_task_parameters.vel           = {0.0,0.0,0.0};
    m_task_parameters.end_coord.x   = x;
    m_task_parameters.end_coord.y   = y;
    m_task_parameters.end_coord.phi = phi;

     

    m_task_parameters.start_coord = traj_manager.get_tool();
    time_to_arrival               = traj_manager.estimated_time_of_arrival(m_task_parameters.start_coord,\
                                                                          m_task_parameters.vel,\
                                                                          m_task_parameters.end_coord,
                                                                          m_task_parameters.vel);
     

    xTaskCreatePinnedToCore(
                    task_path,          /* Function to implement the task */
                    "goto_path",        /* Name of the task */
                    25000,               /* Stack size in words */
                    &m_task_parameters, /* Task input parameter */
                    0,                  /* Priority of the task */
                    &path_handle,       /* Task handle. */
                    RUNNING_CORE);      /* Core where the task should run */

    return time_to_arrival;
}

double TrajectoryManager::goto_home()
{
    double time_to_arrival;

    /* set_parameters */
    set_status(ON_THE_ROAD);
    m_task_parameters.vel           = {0.0,0.0,0.0};

     

    m_task_parameters.start_coord = traj_manager.get_tool();

     

    xTaskCreatePinnedToCore(
                    task_home,          /* Function to implement the task */
                    "goto_home",        /* Name of the task */
                    8196,               /* Stack size in words */
                    &m_task_parameters, /* Task input parameter */
                    0,                  /* Priority of the task */
                    &home_handle,       /* Task handle. */
                    RUNNING_CORE);      /* Core where the task should run */

    return time_to_arrival;
}

void TrajectoryManager::set_status(status_t status) throw()
{
     
    m_status = status;
     
}

status_t TrajectoryManager::get_status() throw()
{
     
    status_t ret = m_status;
     
    return ret;
}

} /* end of namespace */