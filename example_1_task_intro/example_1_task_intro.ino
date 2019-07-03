#include <Arduino_FreeRTOS.h>

void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  // Now set up two tasks to run independently.
  xTaskCreate(
    vTask1
    ,  (const portCHAR *)"Task1-do this"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority
    ,  NULL );

  xTaskCreate(
    vTask2
    ,  (const portCHAR *) "Task2-do that"
    ,  128  // Stack size
    ,  NULL
    //,  1 // same priority as vTask1
    ,  2   // higher Priority than vTask1 
    ,  NULL );

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void vTask1(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  volatile uint32_t ul;

  const char* taskMsg = "Task 1 is running\r\n";
  const TickType_t xDelay250ms = pdMS_TO_TICKS(250);

  for (;;) // A Task shall never return or exit.
  {
    Serial.println(taskMsg);
    // for(ul = 0; ul< 1000000; ul++)
    // {}
    vTaskDelay(xDelay250ms);
  }
  vTaskDelete(NULL);
}

void vTask2(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  volatile uint32_t ul;

  const char* taskMsg = "Task 2 is running\r\n";
  const TickType_t xDelay250ms = pdMS_TO_TICKS(250); 

  for (;;)
  {
    Serial.println(taskMsg);
    // for(ul = 0; ul< 1000000; ul++)
    // {}
    vTaskDelay(xDelay250ms);
  }
  vTaskDelete(NULL);
}
