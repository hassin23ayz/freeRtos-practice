#include <Arduino_FreeRTOS.h>

void vTaskFunction( void *pvParameters );

static const char* pcTextForTask1 = "Task 1 is running\r\n";
static const char* pcTextForTask2 = "Task 2 is running\r\n";

void setup() {
  Serial.begin(9600);
  while (!Serial) {;}

  // Now set up two tasks to run independently.
  xTaskCreate(
    vTaskFunction              // pointer to the function that implements the task
    ,  "Task1"                 // Text Name for the task 
    ,  128                     // Stack Depth 
    ,  (void*)pcTextForTask1   // pass the text to be printed into the task using the task parameter 
    ,  1                       // Priority
    ,  NULL );                 // Task handler , not used 

  // Now set up two tasks to run independently.
  xTaskCreate(
    vTaskFunction              // pointer to the function that implements the task
    ,  "Task2"                 // Text Name for the task 
    ,  128                     // Stack Depth 
    ,  (void*)pcTextForTask2   // pass the text to be printed into the task using the task parameter 
    ,  2                       // Priority
    ,  NULL );                 // Task handler , not used 

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
  vTaskStartScheduler();
}

void loop()
{
  // Empty. Things are done in Tasks.
  // should never reach here 
}

/*---------------------- Tasks ---------------------*/
void vTaskFunction(void *pvParameters)  // This is a task.
{
  char* pcTaskName;
  volatile uint32_t ul;
  TickType_t xLastWakeTime;

  pcTaskName = (char*) pvParameters;
  xLastWakeTime = xTaskGetTickCount();

  for (;;)                    // A Task shall never return or exit.
  {
    Serial.println(pcTaskName);
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(250));
  }
  vTaskDelete(NULL);
}
