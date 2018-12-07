#include "TaskManager.h"
#include "FreeRTOS.h"

bool TaskManager::create_task(TaskFunction_t TaskCode, void * const Parameters)
{
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
        return true;
    }
    else
    {
        return false;
    }

                                               
}

bool TaskManager::delete_task()
{
    if(_taskHandler != NULL)
    {
        vTaskDelete(_taskHandler);
        _task_is_runnig = false;
        return true;
    }
    else
    {
        return false;
    }
    
}