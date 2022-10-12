/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <tim.h>
extern timestamp interruptTimestamp;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
timestamp sendTimestamp;
timestamp receiveTimestamp;
extern timestamp interruptTimestamp;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LedTask1 */
osThreadId_t LedTask1Handle;
const osThreadAttr_t LedTask1_attributes = {
  .name = "LedTask1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for LedTask2 */
osThreadId_t LedTask2Handle;
const osThreadAttr_t LedTask2_attributes = {
  .name = "LedTask2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for SendTask */
osThreadId_t SendTaskHandle;
const osThreadAttr_t SendTask_attributes = {
  .name = "SendTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ReceiveTask */
osThreadId_t ReceiveTaskHandle;
const osThreadAttr_t ReceiveTask_attributes = {
  .name = "ReceiveTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for interruptEvents */
osEventFlagsId_t interruptEventsHandle;
const osEventFlagsAttr_t interruptEvents_attributes = {
  .name = "interruptEvents"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartLedTask1(void *argument);
void StartLedTask2(void *argument);
void StartSendTask(void *argument);
void StartReceiveTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of LedTask1 */
  LedTask1Handle = osThreadNew(StartLedTask1, NULL, &LedTask1_attributes);

  /* creation of LedTask2 */
  LedTask2Handle = osThreadNew(StartLedTask2, NULL, &LedTask2_attributes);

  /* creation of SendTask */
  SendTaskHandle = osThreadNew(StartSendTask, NULL, &SendTask_attributes);

  /* creation of ReceiveTask */
  ReceiveTaskHandle = osThreadNew(StartReceiveTask, NULL, &ReceiveTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of interruptEvents */
  interruptEventsHandle = osEventFlagsNew(&interruptEvents_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartLedTask1 */
/**
* @brief Function implementing the LedTask1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLedTask1 */
void StartLedTask1(void *argument)
{
  /* USER CODE BEGIN StartLedTask1 */
  /* Infinite loop */
  for(;;)
  {
    GPIOD->ODR ^= (1 << 15);

    osDelay(500);
  }
  /* USER CODE END StartLedTask1 */
}

/* USER CODE BEGIN Header_StartLedTask2 */
/**
* @brief Function implementing the LedTask2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLedTask2 */
void StartLedTask2(void *argument)
{
  /* USER CODE BEGIN StartLedTask2 */
  /* Infinite loop */
  for(;;)
  {
    GPIOD->ODR ^= (1 << 14);

    osDelay(1000);
  }
  /* USER CODE END StartLedTask2 */
}

/* USER CODE BEGIN Header_StartSendTask */
/**
* @brief Function implementing the SendTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSendTask */
void StartSendTask(void *argument)
{
  /* USER CODE BEGIN StartSendTask */

  osDelay(2000);

  printf("sende Signal\n");
  sendTimestamp = getTimestamp();
  GPIOD->BSRR = GPIO_BSRR_BS11;
  /* Infinite loop */
  for(;;)
  {

    osDelay(1000);
  }
  /* USER CODE END StartSendTask */
}

/* USER CODE BEGIN Header_StartReceiveTask */
/**
* @brief Function implementing the ReceiveTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartReceiveTask */
void StartReceiveTask(void *argument)
{
  /* USER CODE BEGIN StartReceiveTask */

  /*
  uint16_t signal;

  for(;;)
  {
    signal = (uint16_t)GPIOD->IDR;

    if (signal & GPIO_IDR_ID10) {
      printf("Signal da: %x\n");
    } else {
      printf("Signal nicht da: %x\n");
    }

    osDelay(500);
  }

  */

  osEventFlagsWait(interruptEventsHandle, 0x1, osFlagsWaitAll, osWaitForever);

  receiveTimestamp = getTimestamp();
  printf("Signal im Task angekommen\n");

  timestamp timeDiff = timestampDifference(receiveTimestamp, sendTimestamp);

  printf("Time diff: %u us\n", timestampToMicroSeconds(timeDiff));

  while (1) {

    osDelay(10);
  }
  /*
  osDelay(1000);

  printf("---Timer Test---\n");

  timestamp t, t_prev;
  t_prev.microtics = 0;
  t_prev.nanotics = 0;

  timestamp t_diff;

  while(1) {

    t = getTimestamp();
    printf("T - Microtics: %u, Nanotics: %u\n", t.microtics, t.nanotics);

    if (t_prev.microtics != 0) {
       t_diff = timestampDifference(t, t_prev);
       printf("Diff: Microtics: %u, Nanotics: %u (%u us)\n",
           t_diff.microtics, t_diff.nanotics, timestampToMicroSeconds(t_diff));
    }

    t_prev = t;



    osDelay(10);
  }
  */


  /* USER CODE END StartReceiveTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

