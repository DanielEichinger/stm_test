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
uint8_t resultBuffer[RESULT_BUFFER_LENGTH];
uint32_t bufferPosition = 0;


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
}

void ledTask1() {
  for(;;)
  {
    GPIOD->ODR ^= (1 << 15);

    osDelay(500);
  }
}

void ledTask2() {

  int i = 0;

  for(;;)
    {
      i++;
      GPIOD->ODR ^= (1 << 14);

      osDelay(1000);

      if(i == 10) {
        CDC_Transmit_FS(resultBuffer, strlen(resultBuffer));
      }
    }
}

void sendTask() {

  for(;;)
  {
    // Warten bis Knopf gedrückt wurde
    osThreadFlagsWait(EVENT_BUTTON_PRESSED, osFlagsWaitAll, osWaitForever);

    // Signal an anderen Mikrocontroller senden
    GPIOD->BSRR = GPIO_BSRR_BS11;
    osDelay(100);
    GPIOD->BSRR = GPIO_BSRR_BR11;

    osDelay(1000);
  }
}

void busyTask() {

  while (1) {

    GPIOD->BSRR = GPIO_BSRR_BS8;
    for (int i = 0; i < 0x2FFFFFF; i++){
    }
    GPIOD->BSRR = GPIO_BSRR_BR8;

    osDelay(2000);
  }
}

void ext1Interrupt() {
  timestamp2 = getTimestamp();
  timestamp timestampDiff = timestampDifference(timestamp2, timestamp1);

  printf("timestamp1: %u\n", timestampToMicroSeconds(timestamp1));
  printf("timestamp2: %u\n", timestampToMicroSeconds(timestamp2));
  printf("difference: %u\n", timestampToMicroSeconds(timestampDiff));

  // schreibe Zeitunterschied in Mikrosekunden in Buffer
  sprintf(resultBuffer+bufferPosition, "%u", timestampToMicroSeconds(timestampDifference(timestamp2, timestamp1)));

  // zähle Position um Anzahl der Stellen der Zahl weiter
  bufferPosition+= ((uint32_t)(log10(timestampToMicroSeconds(timestampDiff))))+1;

  // füge ";" als Trennzeichen ein
  resultBuffer[bufferPosition] = ';';
  bufferPosition++;
}

void ext9Interrupt() {
  printf("Knopf gedrueckt\n");
  osThreadFlagsSet(SendTaskHandle, EVENT_BUTTON_PRESSED);
}

void ext15Interrupt(){
  timestamp1 = getTimestamp();
  printf("angekommen\n");
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
