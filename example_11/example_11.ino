/*
The queue is created to hold a maximum of five
int32_t values, even though the priorities of the tasks are set such that the queue will never
contain more than one item at a time
*/

#include <Arduino_FreeRTOS.h>
#include <queue.h>

typedef enum
{
  eSender_CAN,
  eSender_HMI
} DataSource_t;

typedef struct 
{
  uint8_t ucValue;
  DataSource_t eDataSrc;  
} Data_t;

// declare a variable of type QueueHandle_t . 
// this is used to store the handle to the queue that is accessed by all the three tasks
QueueHandle_t xQueue;

void setup() {
  Serial.begin(9600);
  while (!Serial) {;}

  // create the Queue to hold a maximum of 3 values of type Data_t 
  xQueue = xQueueCreate(3, sizeof(Data_t));

  if(xQueue != NULL)
  {
    xTaskCreate(vSenderTask_CAN, "Sender_CAN", 1000, NULL, 2, NULL);
    xTaskCreate(vSenderTask_HMI, "Sender_HMI", 1000, NULL, 2, NULL);

    xTaskCreate(vReceieverTask_Controller, "Receiever_Controller", 1000, NULL, 1, NULL);
    vTaskStartScheduler();
  }
}

void loop()
{
  // Empty. Things are done in Tasks.
  // should never reach here 
}

/*---------------------- Tasks ---------------------*/
static void vSenderTask_CAN(void* pvParameters)
{
  Data_t xStruct_CAN;
  xStruct_CAN.ucValue = 100;
  xStruct_CAN.eDataSrc = eSender_CAN;

  BaseType_t xStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

  for(;;)
  {
    xStatus = xQueueSendToBack(xQueue, (void*)&xStruct_CAN, xTicksToWait);

    if(xStatus != pdPASS)
    {
      Serial.println("Could not send to the queue");
    }

  }
}

static void vSenderTask_HMI(void* pvParameters)
{
  Data_t xStruct_HMI;
  xStruct_HMI.ucValue = 200;
  xStruct_HMI.eDataSrc = eSender_HMI;

  BaseType_t xStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

  for(;;)
  {
    xStatus = xQueueSendToBack(xQueue, (void*)&xStruct_HMI, xTicksToWait);

    if(xStatus != pdPASS)
    {
      Serial.println("Could not send to the queue");
    }

  }
}

static void vReceieverTask_Controller(void* pvParameters)
{
  Data_t xStruct_RECV;

  BaseType_t xStatus;

  for(;;)
  {

    if(uxQueueMessagesWaiting(xQueue) != 3)
    {
      Serial.println(" Queue should have been Full! \r\n");
    }

    xStatus = xQueueReceive(xQueue, (void*)&xStruct_RECV, 0);

    if(xStatus == pdPASS)
    {
      if(xStruct_RECV.eDataSrc == eSender_CAN)
      {
        Serial.print("received from CAN = ");
        Serial.println(xStruct_RECV.ucValue);
      }
      else if(xStruct_RECV.eDataSrc == eSender_HMI)
      {
        Serial.print("receieved from HMI = ");
        Serial.println(xStruct_RECV.ucValue);
      }
    }
    else
    {
      Serial.println("could not receive from the queue");
    }
  }
}