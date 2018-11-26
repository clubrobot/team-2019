#ifndef __TRAJECTORYMANAGER_H
#define __TRAJECTORYMANAGER_H

#include <Arduino.h>
#include "Picker.h"
#include "ArmManager.h"

#define RUNNING_CORE 0

typedef enum{
    ARRIVED     = 0X00,
    ON_THE_ROAD = 0X01,
    ERROR       = 0X02,
}status_t;

typedef struct
{
    SemaphoreHandle_t semaphore;

    coords_t start_coord;
    coords_t vel;
    coords_t end_coord;

}trajectory_task_pv_t;

class TrajectoryManager : public ArmManager
{

    public :
        TrajectoryManager(double dt = 0.2) : ArmManager(dt) {m_task_parameters.semaphore = xSemaphoreCreateMutex();}
        /* go directly to pos */
        double goto_directly(double x, double y, double phi);
        /* go to pos with path */
        double goto_path(double x, double y, double phi);

        double goto_home();

        void set_status(status_t status){m_status = status;}

        status_t get_status(){return m_status;}

    private :

        status_t m_status;

        trajectory_task_pv_t m_task_parameters;
       
        
};

#endif /* __TRAJECTORYMANAGER_H */