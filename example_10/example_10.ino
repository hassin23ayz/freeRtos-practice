/*
The queue is created to hold a maximum of five
int32_t values, even though the priorities of the tasks are set such that the queue will never
contain more than one item at a time
*/

#include <Arduino_FreeRTOS.h>
#include <queue.h>

// declare a variable of type QueueHandle_t . 
// this is used to store the handle to the queue that is accessed by all the three tasks
QueueHandle_t xQueue;

void setup() {
  Serial.begin(9600);
  while (!Serial) {;}

  // create the Queue to hold a maximum of 5 values of type int32_t 
  xQueue = xQueueCreate(5, sizeof(int32_t));

  if(xQueue != NULL)
  {
    xTaskCreate(vSenderTask, "Sender1", 1000, (void*)100, 1, NULL);
    xTaskCreate(vSenderTask, "Sender2", 1000, (void*)200, 1, NULL);

    xTaskCreate(vReceieverTask, "Receiever", 1000, NULL, 2, NULL);
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
  int32_t lValueToSend;
  BaseType_t xStatus; 

  lValueToSend = (int32_t)pvParameters;

  for(;;)
  {
    xStatus = xQueueSendToBack(xQueue, &lValueToSend, 0);

    if(xStatus != pdPASS)
    {
      Serial.println("Could not send to the queue");
    }

  }
}

static void vReceieverTask(void* pvParameters)
{
  BaseType_t xStatus;
  int32_t lReceivedValue;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

  for(;;)
  {

    if(uxQueueMessagesWaiting(xQueue) != 0)
    {
      Serial.println(" Queue should have been empty \r\n");
    }

    xStatus = xQueueReceive(xQueue, (void*)&lReceivedValue, xTicksToWait);
    //xStatus = xQueueReceive(xQueue, (void*)&lReceivedValue, portMAX_DELAY); //works too 

    if(xStatus == pdPASS)
    {
      Serial.print("receieved= ");
      Serial.println(lReceivedValue);
    }
    else
    {
      Serial.println("could not receive from the queue , timeout");
    }
  }
}