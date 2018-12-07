#include <Arduino.h>
#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
#include "../common/SerialTalks.h"
#include "../common/IK/Joint.h"
#include "../common/IK/Picker.h"
#include "../common/IK/ArmManager.h"
#include "../common/IK/TrajectoryManager.h"
#include "../common/IK/TaskManager.h"
#include "arm_config.h"

using namespace IK;
using namespace std;

TaskManager taskManager;

double id;

static void task(void * param) 
{
   for(;;)
   {
        Serial.println("Hello task");
        delay(500);
        Serial.println(taskManager.task_is_running());
        delay(5000);
        taskManager.delete_task();
   }
}

static void task2(void * param) 
{
   for(;;)
   {
        Serial.println("Hello task 22222");
        delay(500);

   }
}

void setup()
{
    /* init Serial talks */
    Serial.begin(SERIALTALKS_BAUDRATE);
    talks.begin(Serial);

    taskManager.create_task(task, &id);
  
}

void loop()
{  
    Serial.println("Hello main");
    Serial.println(taskManager.task_is_running());
    delay(1000);
    taskManager.create_task(task2, &id);
}
