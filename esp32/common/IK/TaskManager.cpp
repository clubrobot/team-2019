#include "TaskManager.h"
#include "FreeRTOS.h"
#include <iostream>

using namespace std;

#ifdef IK_LOG
    #define LOG_TASK(arg) cout << __TIME__<<" (TASK MANAGER)("<< __func__ << " , " << __LINE__ << ")\t\t: "<< arg <<endl;
#else
    #define LOG_TASK(arg) 
#endif

bool TaskManager::create_task(TaskFunction_t TaskCode, void * const Parameters)
{
    bool ret;

    m_mutex.acquire();

    if(!_task_is_runnig)
    {
        _taskStatus = xTaskCreatePinnedToCore(
                        TaskCode,           /* Function to implement the task */
                        "Task_Manager",     /* Name of the task */
                        _stackDepth,        /* Stack size in words */
                        Parameters,         /* Task input parameter */
                        _taskPriority,      /* Priority of the task */
                        &_taskHandler,      /* Task handle. */
                        _coreID);           /* Core where the task should run */

        /* TODO : Handle task creation status */

        _task_is_runnig = true;
        ret = true;
        LOG_TASK("TASK CREATION SUCCESS");
    }
    else
    {
        LOG_TASK("TASK ALREADY RUNNING");
        ret = false;
    }

    m_mutex.release();

    return ret;                                        
}

void TaskManager::delete_task()
{
    m_mutex.acquire();

    if(_taskHandler != NULL)
    {
        LOG_TASK("TASK DELETE SUCCESS");
        _task_is_runnig = false;
        m_mutex.release();
        vTaskDelete(_taskHandler);
    }
    else
    {
        m_mutex.release();
    }
}

bool TaskManager::task_is_running()
{
    bool ret;

    m_mutex.acquire();

    ret = _task_is_runnig;

    m_mutex.release();

    return ret;
}