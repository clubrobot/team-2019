#include <Arduino.h>

void task(void * sema) {
    while(true)
    {
        xSemaphoreTake( *((SemaphoreHandle_t * )  (sema)),  ( TickType_t ) -1  );
        Serial.println("Je suis le thread fils et je mange");
        delay(1);
        Serial.println("Je suis le thread fils et j'ai fini de manger");
        xSemaphoreGive(*((SemaphoreHandle_t * )  (sema)));
        delay(1);
    }
}

SemaphoreHandle_t semaph;

void setup()
{
	Serial.begin(9600);
    int returnValue;
    semaph = xSemaphoreCreateMutex();

   if( semaph == NULL )
   {
       Serial.println("An error has occurred with semaphore");
   }
  
    xTaskCreatePinnedToCore(
                    task,   /* Function to implement the task */
                    "test", /* Name of the task */
                    10000,      /* Stack size in words */
                    &semaph,       /* Task input parameter */
                    0,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    1);  /* Core where the task should run */
   


    }

// Loop

void loop()
{	
    xSemaphoreTake( semaph,  ( TickType_t ) -1  );
     Serial.println("Je suis le thread pere et je mange");
    delay(1);
    Serial.println("Je suis le thread pere et j'ai fini de manger");
    xSemaphoreGive(semaph);
    delay(5);

}
