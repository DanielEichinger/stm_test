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
#define EVENT_ALL_TESTS_DONE 0x2

#define RESULT_BUFFER_LENGTH 400

#define DEBOUNCE_MS 100

#define NUM_OF_TESTS 50

extern uint8_t testsDone;
extern uint16_t counter;

extern uint8_t resultBuffer[RESULT_BUFFER_LENGTH];
extern uint32_t bufferPosition;

int _write(int file, char *ptr, int len);
extern uint32_t taskStatTicks;
void setupTaskStatTimer();
void beforeStart();

void ledTask1();
void ledTask2();
void sendTask();
void busyTask();
void usbSerialTask();
void startTask();

void ext1Interrupt();
void ext9Interrupt();
void ext15Interrupt();

extern uint32_t microtics;
void tim3Interrupt();
void tim6Interrupt();


#define MICROTICS_DURATION_US 25000
#define NANOTICS_DURATION_US  1/2

typedef struct timestamp {
  uint32_t nanotics;
  uint32_t microtics;
} timestamp;

extern timestamp timestamp1;
extern timestamp timestamp2;

extern osThreadId_t SendTaskHandle;
extern osThreadId_t UsbSerialTaskHandle;
extern osEventFlagsId_t interruptEventsHandle; //freertos.c (generiert)

timestamp getTimestamp();
uint32_t timestampToMicroSeconds(timestamp);
timestamp timestampDifference(timestamp, timestamp);



#endif /* INC_USERCODE_H_ */
