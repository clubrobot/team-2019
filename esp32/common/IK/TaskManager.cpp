#include "TaskManager.h"
#include "FreeRTOS.h"

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
    }
    else
    {
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