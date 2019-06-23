/*
this example shows the use of queue as a mailbox
create the queue as usual 
write to the queue using xQueueOverwrite(QueueHandle_t xQueue, const void* pvItemToQueue)
read from the queue using xQueuePeek(QueueHandle_t xQueue, void* const pvBuffer, TickType_t xTicksToWait)
*/

#include <Arduino_FreeRTOS.h>
#include <queue.h>

#include <stdio.h>
#include <stdlib.h>

/* declare a queue variable of type QueueHandle_t */
QueueHandle_t xMailBox;

typedef enum
{
  E_GPS,
  E_CAN
}EDataSource;

typedef struct 
{
  EDataSource eDataSrc;
  TickType_t xTimeStamp; 
  char cDataBuf[32];
}SData;

void setup() {
  Serial.begin(9600);
  while (!Serial) {;}
  delay(1000);
  Serial.println("-------FreeRtos------");

  xMailBox = xQueueCreate(1, sizeof(SData));

  if(xMailBox != NULL)
  {
    xTaskCreate(prvSenderTask,    "Sender",    1000, NULL, 1, NULL);
    xTaskCreate(prvReceieverTask, "Receiever", 1000, NULL, 1, NULL);
    vTaskStartScheduler();
  }
}

void loop()
{
  /* Empty. Things are done in Tasks */
  /* should never reach here */
}

/*---------------------- Tasks ---------------------*/
static void prvSenderTask(void* pvParameters)
{
  BaseType_t xStatus;
  SData xCanData;
  xCanData.eDataSrc = E_CAN;
  uint32_t ulSpeed = 0;

  for(;;)
  { 
    xCanData.xTimeStamp = xTaskGetTickCount();
    memset(xCanData.cDataBuf, '\0', 32);
    snprintf(xCanData.cDataBuf, 32, "speed:%ld time:%ld", ulSpeed++, xCanData.xTimeStamp);

    xStatus = xQueueOverwrite(xMailBox, &xCanData);
  }
}

static void prvReceieverTask(void* pvParameters)
{
  BaseType_t xStatus;
  SData xCanRcvData;
  TickType_t xPreviousTimeStamp = xTaskGetTickCount();

  for(;;)
  {
    xStatus = xQueuePeek(xMailBox, &xCanRcvData, portMAX_DELAY);

    if(xCanRcvData.xTimeStamp > xPreviousTimeStamp)
    {
      xPreviousTimeStamp = xCanRcvData.xTimeStamp;
      Serial.print("\n\rToyota Axio= ");
      Serial.println(xCanRcvData.cDataBuf);
    }
  }
}