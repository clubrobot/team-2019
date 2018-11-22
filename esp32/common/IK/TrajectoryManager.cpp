#include <Arduino.h>
#include "TrajectoryManager.h"
#include "Picker.h"
#include "ArmManager.h"

extern ArmManager arm_manager;

static TaskHandle_t path_handle;
static TaskHandle_t directly_handle;

template<typename T>
ostream& operator<< (ostream& out, const vector<T>& v) {
    out << "{";
    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) {
        out << v[i];
        if (i != last) 
            out << ", ";
    }
    out << "}";
    return out;
}


static bool equals(double* a, double* b, double epsilon = 0.3)
{
    return ((std::abs(a[0] - b[0]) < epsilon) && (std::abs(a[1] - b[1]) < epsilon) && (std::abs(a[2] - b[2]) < epsilon));
}
/* task withouth path */
static void task_directly(void * param) 
{
    
    trajectory_task_pv_t* task_param = (trajectory_task_pv_t*)param;
    static TickType_t xDelay;
    int element_number;
    joints_t joints;

    bool pos_reached = false;
    double pos[3];
    double wait_pos[3];

    std::cout << "GOTO DIRECTLY TASK CREATED" << std::endl;

    /* take semaphore */
    xSemaphoreTake(task_param->semaphore,  ( TickType_t ) -1  );

    /* compute path for trajectory */
    joints = arm_manager.m_arm.inverse_kinematics(task_param->end_coord);

    xSemaphoreGive(task_param->semaphore);

    pos[0] = AX12_COORDS(joints.th1);
    pos[1] = AX12_COORDS(joints.th2);
    pos[2] = AX12_COORDS(joints.th3);
        
    xSemaphoreTake(task_param->semaphore,  ( TickType_t ) -1);
    /* set pos and velocity to AX12 motor */
    arm_manager.m_AX1.move(pos[0]);
    arm_manager.m_AX2.move(pos[1]);
    arm_manager.m_AX3.move(pos[2]);

    xSemaphoreGive(task_param->semaphore);

    /* check error or if position is reached */
    while(!pos_reached)
    {
        xSemaphoreTake(task_param->semaphore,  ( TickType_t ) -1);

        wait_pos[0] = arm_manager.m_AX1.readPosition();
        wait_pos[1] = arm_manager.m_AX2.readPosition();
        wait_pos[2] = arm_manager.m_AX3.readPosition();

        xSemaphoreGive(task_param->semaphore);

        if(equals(pos, wait_pos))
        {
            /* switch to nex pos in path */
            pos_reached = true;
        }
        else
        {
            /* sleep */
            vTaskDelay(100/portTICK_PERIOD_MS);
        }
    }    

    std::cout << "th1 : " << AX12_COORDS(joints.th1) << std::endl;
    std::cout << "th2 : " << AX12_COORDS(joints.th2) << std::endl;
    std::cout << "th3 : " << AX12_COORDS(joints.th3) << std::endl;

    std::cout << "GOTO DIRECTLY TASK DELETED" << std::endl;

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

    std::cout << "PATH TASK CREATED" << std::endl;
    /* set default delay */
    xDelay= 10 / portTICK_PERIOD_MS;

    /* take semaphore */
    xSemaphoreTake(task_param->semaphore,  ( TickType_t ) -1  );

    /* compute path for trajectory */
    chemin = arm_manager.go_to(task_param->start_coord, task_param->vel , task_param->end_coord , task_param->vel);

    /* give semaphore */
    xSemaphoreGive(task_param->semaphore);

    /* get the size of path */
    element_number = chemin.path_th1.t.size();

    for(int i = 0; i < element_number ; i++)
    {
        pos_reached = false;

        pos[0] = AX12_COORDS(chemin.path_th1.pos[i]);
        vel[0] = AX12_SPEED(chemin.path_th1.vel[i]);

        pos[1] = AX12_COORDS(chemin.path_th2.pos[i]);
        vel[1] = AX12_SPEED(chemin.path_th2.vel[i]);

        pos[2] = AX12_COORDS(chemin.path_th3.pos[i]);
        vel[2] = AX12_SPEED(chemin.path_th3.vel[i]);
        
        xSemaphoreTake(task_param->semaphore,  ( TickType_t ) -1);
        /* set pos and velocity to AX12 motor */
        arm_manager.m_AX1.moveSpeed(pos[0], vel[0]);
        arm_manager.m_AX2.moveSpeed(pos[1], vel[1]);
        arm_manager.m_AX3.moveSpeed(pos[2], vel[2]);

        xSemaphoreGive(task_param->semaphore);

        /* check error or if position is reached */
        while(!pos_reached)
        {
            xSemaphoreTake(task_param->semaphore,  ( TickType_t ) -1);

            wait_pos[0] = arm_manager.m_AX1.readPosition();
            wait_pos[1] = arm_manager.m_AX2.readPosition();
            wait_pos[2] = arm_manager.m_AX3.readPosition();

            xSemaphoreGive(task_param->semaphore);

            if(equals(pos, wait_pos))
            {
                /* switch to nex pos in path */
                pos_reached = true;
            }
            else
            {
                /* get waiting time in ms*/
                xDelay = (chemin.path_th1.t[i] * 100)/portTICK_PERIOD_MS;
                /* sleep */
                vTaskDelay(xDelay);
            }
        }
    }

    std::cout << "t : " << chemin.path_th1.t << std::endl;
    std::cout << "pos : " << chemin.path_th1.pos << std::endl;
    std::cout << "vel : " << chemin.path_th1.vel << std::endl;
    std::cout << std::endl;

    std::cout << "t : " << chemin.path_th2.t << std::endl;
    std::cout << "pos : " << chemin.path_th2.pos << std::endl;
    std::cout << "vel : " << chemin.path_th2.vel << std::endl;
    std::cout << std::endl;

    std::cout << "t : " << chemin.path_th3.t << std::endl;
    std::cout << "pos : " << chemin.path_th3.pos << std::endl;
    std::cout << "vel : " << chemin.path_th3.vel << std::endl;
    std::cout << std::endl;
    
    task_param->is_arrived    = ARRIVED;

    std::cout << "PATH TASK DELETED" << std::endl;

    if( path_handle != NULL )
    {
        vTaskDelete(path_handle);
    }
}

TrajectoryManager::TrajectoryManager()
{
    m_task_parameters.semaphore = xSemaphoreCreateMutex();
}

/* go directly to pos */
double TrajectoryManager::goto_directly(double x, double y, double phi)
{
    double time_to_arrival;

    /* set_parameters */
    m_task_parameters.is_arrived    = ON_THE_ROAD;
    m_task_parameters.vel           = {0.0,0.0,0.0};
    m_task_parameters.end_coord.x   = x;
    m_task_parameters.end_coord.y   = y;
    m_task_parameters.end_coord.phi = phi;

    xSemaphoreTake(m_task_parameters.semaphore,  ( TickType_t ) -1  );

    m_task_parameters.start_coord = arm_manager.m_arm.get_tool();
    /* compute an estimation of trajectory time */
    time_to_arrival               = arm_manager.estimated_time_of_arrival(m_task_parameters.start_coord,\
                                                                          m_task_parameters.vel,\
                                                                          m_task_parameters.end_coord, \
                                                                          m_task_parameters.vel);
    xSemaphoreGive(m_task_parameters.semaphore);

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
    m_task_parameters.is_arrived    = ON_THE_ROAD;
    m_task_parameters.vel           = {0.0,0.0,0.0};
    m_task_parameters.end_coord.x   = x;
    m_task_parameters.end_coord.y   = y;
    m_task_parameters.end_coord.phi = phi;

    xSemaphoreTake(m_task_parameters.semaphore,  ( TickType_t ) -1  );

    m_task_parameters.start_coord = arm_manager.m_arm.get_tool();
    time_to_arrival               = arm_manager.estimated_time_of_arrival(m_task_parameters.start_coord,\
                                                                          m_task_parameters.vel,\
                                                                          m_task_parameters.end_coord,
                                                                          m_task_parameters.vel);
    xSemaphoreGive(m_task_parameters.semaphore);

    xTaskCreatePinnedToCore(
                    task_path,          /* Function to implement the task */
                    "goto_path",        /* Name of the task */
                    8196,               /* Stack size in words */
                    &m_task_parameters, /* Task input parameter */
                    0,                  /* Priority of the task */
                    &path_handle,       /* Task handle. */
                    RUNNING_CORE);      /* Core where the task should run */

    return time_to_arrival;
}