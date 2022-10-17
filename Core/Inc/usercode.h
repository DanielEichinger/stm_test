/*
 * usercode.h
 *
 *  Created on: Oct 13, 2022
 *      Author: daniel
 */

#ifndef INC_USERCODE_H_
#define INC_USERCODE_H_

#include "stdint.h"
#include "cmsis_os2.h"

#define EVENT_BUTTON_PRESSED 0x1

int _write(int file, char *ptr, int len);
void beforeStart();

void ledTask1();
void ledTask2();
void sendTask();
void busyTask();

void ext1Interrupt();
void ext9Interrupt();
void ext15Interrupt();

extern uint32_t microtics;
void tim3Interrupt();


#define MICROTICS_DURATION_US 25000
#define NANOTICS_DURATION_US  1/2

typedef struct timestamp {
  uint32_t nanotics;
  uint32_t microtics;
} timestamp;

extern timestamp timestamp1;
extern timestamp timestamp2;


extern osEventFlagsId_t interruptEventsHandle; //freertos.c (generiert)

timestamp getTimestamp();
uint32_t timestampToMicroSeconds(timestamp);
timestamp timestampDifference(timestamp, timestamp);



#endif /* INC_USERCODE_H_ */
