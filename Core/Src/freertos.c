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
#include <usercode.h>
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
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for BusyTask */
osThreadId_t BusyTaskHandle;
const osThreadAttr_t BusyTask_attributes = {
  .name = "BusyTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for UsbSerialTask */
osThreadId_t UsbSerialTaskHandle;
const osThreadAttr_t UsbSerialTask_attributes = {
  .name = "UsbSerialTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for StartTask */
osThreadId_t StartTaskHandle;
const osThreadAttr_t StartTask_attributes = {
  .name = "StartTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartLedTask1(void *argument);
void StartLedTask2(void *argument);
void StartSendTask(void *argument);
void StartBusyTask(void *argument);
void StartUsbSerialTask(void *argument);
void StartStartTask(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{
  setupTaskStatTimer();
}

__weak unsigned long getRunTimeCounterValue(void)
{
return taskStatTicks;
}
/* USER CODE END 1 */

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

  /* creation of BusyTask */
  BusyTaskHandle = osThreadNew(StartBusyTask, NULL, &BusyTask_attributes);

  /* creation of UsbSerialTask */
  UsbSerialTaskHandle = osThreadNew(StartUsbSerialTask, NULL, &UsbSerialTask_attributes);

  /* creation of StartTask */
  StartTaskHandle = osThreadNew(StartStartTask, NULL, &StartTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

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
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  osThreadSuspend(defaultTaskHandle)
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
  ledTask1();
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
  ledTask2();
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
  sendTask();
  /* USER CODE END StartSendTask */
}

/* USER CODE BEGIN Header_StartBusyTask */
/**
* @brief Function implementing the BusyTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBusyTask */
void StartBusyTask(void *argument)
{
  /* USER CODE BEGIN StartBusyTask */
  busyTask();
  /* USER CODE END StartBusyTask */
}

/* USER CODE BEGIN Header_StartUsbSerialTask */
/**
* @brief Function implementing the UsbSerialTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUsbSerialTask */
void StartUsbSerialTask(void *argument)
{
  /* USER CODE BEGIN StartUsbSerialTask */
  /* Infinite loop */
  usbSerialTask();
  /* USER CODE END StartUsbSerialTask */
}

/* USER CODE BEGIN Header_StartStartTask */
/**
* @brief Function implementing the StartTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartStartTask */
void StartStartTask(void *argument)
{
  /* USER CODE BEGIN StartStartTask */
  /* Infinite loop */
  startTask();
  /* USER CODE END StartStartTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

