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

uint32_t microtics;
timestamp timestamp1;
timestamp timestamp2;


int _write(int file, char *ptr, int len) {
  /* Implement your write code here, this is used by puts and printf for example */
  int i;
  for(i=0 ; i<len ; i++)
    ITM_SendChar((*ptr++));
  return len;
}

void beforeStart() {
  printf("starte...\n");
  // Compare Interrupt einschalten
  TIM3->DIER |= TIM_DIER_CC1IE;

  // Timer 3 starten
  HAL_TIM_OC_Start(&htim3, TIM_CHANNEL_1);
}

void ledTask1() {
  for(;;)
  {
    GPIOD->ODR ^= (1 << 15);

    osDelay(500);
  }
}

void ledTask2() {
  for(;;)
    {
      GPIOD->ODR ^= (1 << 14);

      osDelay(1000);
    }
}

void sendTask() {

  // Warten bis Knopf gedrückt wurde
  osEventFlagsWait(interruptEventsHandle, EVENT_BUTTON_PRESSED, osFlagsWaitAll, osWaitForever);

  // Signal an anderen Mikrocontroller senden
  GPIOD->BSRR = GPIO_BSRR_BS8;

  for(;;)
  {

    osDelay(1000);
  }
}

void receiveTask() {

  while (1) {
    osDelay(1000);
  }
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
/*
    osEventFlagsWait(interruptEventsHandle, 0x1, osFlagsWaitAll, osWaitForever);

    receiveTimestamp = getTimestamp();
    printf("Signal im Task angekommen\n");

    timestamp timeDiff = timestampDifference(receiveTimestamp, sendTimestamp);

    printf("Time diff: %u us\n", timestampToMicroSeconds(timeDiff));

    while (1) {

      osDelay(10);
    }
    */
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
}

void ext1Interrupt() {
  timestamp2 = getTimestamp();

  printf("timestamp1: %u\n", timestampToMicroSeconds(timestamp1));
  printf("timestamp2: %u\n", timestampToMicroSeconds(timestamp2));
  printf("difference: %u\n", timestampDifference(timestamp2, timestamp1));
}

void ext9Interrupt() {
/*
  printf("Signal im Interrupt registriert\n");
  //interruptTickCount = osKernelGetTickCount();
  interruptTimestamp = getTimestamp();

  osEventFlagsSet(interruptEventsHandle, 0x1);
*/

  printf("HEY:D\n");
  osEventFlagsSet(interruptEventsHandle, EVENT_BUTTON_PRESSED);
}

void ext15Interrupt(){
  timestamp1 = getTimestamp();
  printf("angekommen\n");
}


void tim3Interrupt() {
  microtics++;
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


