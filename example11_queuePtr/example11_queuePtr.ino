/*
this example shows how to pass data via a pointer of a buffer array using freertos queue
*/

#include <Arduino_FreeRTOS.h>
#include <queue.h>

#include <stdio.h>
#include <stdlib.h>

// declare a queue variable of type QueueHandle_t . 
QueueHandle_t xPointerQueue;

typedef enum
{
  eGPS,
  eCAN
}EDataSource;

typedef struct 
{
  //char* dataPtr;
  char data[32];
  EDataSource eDataSrc;
}SData;

void setup() {
  Serial.begin(9600);
  while (!Serial) {;}
  delay(1000);
  Serial.println("-------FreeRtos------");

  xPointerQueue = xQueueCreate(1, sizeof(SData));

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
  SData gps1Data;
  uint32_t thisVarWillNotIncrement = 575;

  gps1Data.eDataSrc = eGPS;
  //gps1Data.dataPtr = (char*) malloc(32*sizeof(char));
  memset(gps1Data.data, '\0', 32);
  snprintf(gps1Data.data, 32, "Dhaka, Bangladesh %d\r\n", thisVarWillNotIncrement++);

  for(;;)
  {    
    xStatus = xQueueSendToBack(xPointerQueue, &gps1Data, 0);
    //xStatus = xQueueSendToBack(xPointerQueue, &xDataCounter, portMAX_DELAY);

    if(xStatus == pdPASS)
    {
      Serial.println("data sent to queue");
    }
  }
}

static void vReceieverTask(void* pvParameters)
{
  BaseType_t xStatus;
  //const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  SData gpsRcvData;
  for(;;)
  {

    // if(uxQueueMessagesWaiting(xPointerQueue) != 0)
    // {
    //   Serial.println(" Queue should have been empty \r\n");
    // }

    xStatus = xQueueReceive(xPointerQueue, &gpsRcvData, portMAX_DELAY);

    if(xStatus == pdPASS)
    {
      Serial.println("receieved= ");
      Serial.println(gpsRcvData.data);
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