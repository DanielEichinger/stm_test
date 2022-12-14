/*
 * usercode.c
 *
 *  Created on: Oct 13, 2022
 *      Author: daniel
 */

#include "usercode.h"

#include <stdio.h>
#include "tim.h"
#include "cmsis_os2.h"
#include "usbd_cdc_if.h"
#include <math.h>

uint32_t taskStatTicks;

uint32_t microtics;
timestamp timestamp1;
timestamp timestamp2;
uint32_t testResults[NUM_OF_TESTS];
uint8_t testsDone = 0;


int _write(int file, char *ptr, int len) {
  /* Implement your write code here, this is used by puts and printf for example */
  int i;
  for(i=0 ; i<len ; i++)
    ITM_SendChar((*ptr++));
  return len;
}

void setupTaskStatTimer() {

  taskStatTicks = 0;

  // Timer 6 starten
  HAL_TIM_Base_Start_IT(&htim6);
}

void beforeStart() {


  printf("starte...\n");


  // Compare Interrupt einschalten
  TIM3->DIER |= TIM_DIER_CC1IE;

  // Timer 3 starten
  HAL_TIM_OC_Start(&htim3, TIM_CHANNEL_1);

  TIM4->DIER |= TIM_DIER_CC1IE;
  HAL_TIM_OC_Start(&htim4, TIM_CHANNEL_1);
}

void ledTask1() {
  for(;;)
  {
    GPIOD->ODR ^= (1 << 15);

    osDelay(250);
  }
}

void ledTask2() {
  for(;;)
  {
    GPIOD->ODR ^= (1 << 14);

    osDelay(500);
  }
}

void sendTask() {

  for(;;)
  {
    // Warten bis Knopf gedrückt wurde
    osThreadFlagsWait(EVENT_BUTTON_PRESSED, osFlagsWaitAll, osWaitForever);

    // Signal an anderen Mikrocontroller senden
    GPIOD->BSRR = GPIO_BSRR_BS11;
    GPIOD->BSRR = GPIO_BSRR_BR11;
  }
}

void busyTask() {

  while (1) {

    //GPIOD->BSRR = GPIO_BSRR_BS8;
    //for (int i = 0; i < 0x2FFFFFF; i++){
    //}
    //GPIOD->BSRR = GPIO_BSRR_BR8;

    osDelay(2000);
  }
}

void usbSerialTask() {

  osThreadFlagsWait(EVENT_ALL_TESTS_DONE, osFlagsWaitAll, osWaitForever);

  for (int i = 0; i < NUM_OF_TESTS; i++) {
    printf("Test %d: %d\n", i, testResults[i]);
  }

  osThreadSuspend(UsbSerialTaskHandle);

  while (1) {
    osDelay(1000);
  }
}
extern osThreadId_t StartTaskHandle;
void startTask() {
  beforeStart();
  osThreadSuspend(StartTaskHandle);
}

void ext1Interrupt() {

  if (testsDone < NUM_OF_TESTS) {
    timestamp2 = getTimestamp();
    timestamp timestampDiff = timestampDifference(timestamp2, timestamp1);
    uint32_t timestampDiffUs = timestampToMicroSeconds(timestampDiff);

    testResults[testsDone] = timestampDiffUs;
    testsDone++;
  }
  if (testsDone == NUM_OF_TESTS) {
    osThreadFlagsSet(UsbSerialTaskHandle, EVENT_ALL_TESTS_DONE);
    testsDone++;
  }
}

void ext9Interrupt() {
  osThreadFlagsSet(SendTaskHandle, EVENT_BUTTON_PRESSED);
}

void ext15Interrupt(){
  timestamp1 = getTimestamp();
}

void tim3Interrupt() {
  microtics++;
}

void tim6Interrupt() {
  taskStatTicks++;
}

timestamp getTimestamp() {

  timestamp t;

  t.nanotics = TIM3->CNT;
  t.microtics = microtics;

  return t;
}

uint32_t timestampToMicroSeconds(timestamp t) {

  return t.microtics * MICROTICS_DURATION_US + t.nanotics * NANOTICS_DURATION_US;

}

timestamp timestampDifference(timestamp t1, timestamp t2) {
  // Sei t1 > t2

  timestamp result;

  if (t1.nanotics < t2.nanotics) {
    timestamp t1_tmp = t1;

    t1_tmp.microtics -= 1;
    t1_tmp.nanotics += TIM3->ARR+1;

    result.microtics = t1_tmp.microtics - t2.microtics;
    result.nanotics = t1_tmp.nanotics - t2.nanotics;

    return result;
  }

  result.microtics = t1.microtics - t2.microtics;
  result.nanotics = t1.nanotics - t2.nanotics;

  return result;
}
