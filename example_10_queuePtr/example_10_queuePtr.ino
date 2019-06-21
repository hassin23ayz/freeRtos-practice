/*
this example shows how to pass data via a pointer of a buffer array using freertos queue
*/

#include <Arduino_FreeRTOS.h>
#include <queue.h>

// declare a queue variable of type QueueHandle_t . 
QueueHandle_t xPointerQueue;

void setup() {
  Serial.begin(9600);
  while (!Serial) {;}
  delay(1000);
  Serial.println("-------FreeRtos------");
  // create the Queue to hold a maximum of 5 items of type char* 
  //xPointerQueue = xQueueCreate(5, sizeof(char*));
  xPointerQueue = xQueueCreate(1, sizeof(uint32_t));

  if(xPointerQueue != NULL)
  {
    xTaskCreate(vSenderTask,    "Sender",    1000, NULL, 1, NULL);
    xTaskCreate(vReceieverTask, "Receiever", 1000, NULL, 2, NULL);
    //xTaskCreate(vPrintRamUsage, "Debug",     1000, NULL, 0, NULL);
    vTaskStartScheduler();
  }
}

void loop()
{
  // Empty. Things are done in Tasks.
  // should never reach here 
}

/*---------------------- Tasks ---------------------*/
static void vSenderTask(void* pvParameters)
{
  BaseType_t xStatus;
  char* dataToSend;
  //BaseType_t xDataCounter = 0;
  uint32_t xDataCounter = 0;

  for(;;)
  {
    //dataToSend = (char*) malloc(64 * sizeof(char));
    //snprintf(dataToSend, 64, "Data-ID %d = Hello RTOS/r/n", xDataCounter);
    //xStatus = xQueueSendToBack(xPointerQueue, &dataToSend, 0);
    xStatus = xQueueSendToBack(xPointerQueue, &xDataCounter, portMAX_DELAY);

    if(xStatus == pdPASS)
    {
      Serial.print("data sent to queue = ");
      Serial.println(xDataCounter);
      xDataCounter++;
    }
  }
}

static void vReceieverTask(void* pvParameters)
{
  BaseType_t xStatus;
  //char* dataToRecv;
  //const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

  uint32_t dataToRecv;
  for(;;)
  {

    // if(uxQueueMessagesWaiting(xPointerQueue) != 0)
    // {
    //   Serial.println(" Queue should have been empty \r\n");
    // }

    xStatus = xQueueReceive(xPointerQueue, (void*)&dataToRecv, portMAX_DELAY);

    if(xStatus == pdPASS)
    {
      Serial.print("receieved= ");
      Serial.println(dataToRecv);

    //  free(dataToRecv);
    }
    else
    {
      Serial.println("could not receive from the queue , timeout");
    }
  }
}

/*
static void vPrintRamUsage(void* pvParameters)
{
  for(;;)
  {
    if((millis() - timeNow) > 2000)
    {
      timeNow = millis();
      Serial.println("---------------------");
      Serial.print("RAM left: ");
      Serial.println(getFreeRam());
      Serial.println("---------------------");
    }
  }
}

int getFreeRam()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
*/