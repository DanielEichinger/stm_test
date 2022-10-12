/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim3;

/* USER CODE BEGIN Private defines */

#define MICROTICS_DURATION_US 25000
#define NANOTICS_DURATION_US  1/2

/* USER CODE END Private defines */

void MX_TIM3_Init(void);

/* USER CODE BEGIN Prototypes */
extern uint32_t microtics;

typedef struct timestamp {
  uint32_t nanotics;
  uint32_t microtics;
} timestamp;

timestamp getTimestamp();
uint32_t timestampToMicroSeconds(timeStamp);
timestamp timestampDifference(timestamp, timestamp);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

