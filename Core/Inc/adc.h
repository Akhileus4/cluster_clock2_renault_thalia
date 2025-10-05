/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */

#define ADC_BUFFER_LENGTH 4
extern uint16_t adc_buffer[ADC_BUFFER_LENGTH];  // PA0..PA3 értékei

#define AD_TASK_STACK_SIZE (4096)
#define AD_AVERAGE_QUEUE_SIZE (100)
#define AD_MOVIN_HYSTERESIS_DELTA (20)


#define BOOST_ADC_LIMIT_MIN 1128
#define BOOST_ADC_LIMIT_MAX 1580
#define USB_ADC_LIMIT_MIN 2234
#define USB_ADC_LIMIT_MAX 3413
#define BATTERY_ADC_LIMIT_MIN 2482
#define BATTERY_ADC_LIMIT_MAX 4095


/**
 * Moving hyst variables
 * */
typedef struct {
	uint16_t delta;
	uint16_t hyst_min;
	uint16_t hyst_max;

} moving_hysteresis_t;

/**
 * Moving average variables
 * */
typedef struct {
	uint16_t queue[AD_AVERAGE_QUEUE_SIZE];
	uint8_t index;

} moving_average_t;

/**
 * One adc with moving hyst, moving average, convertion variables and channel_id
 * */
typedef struct {

	uint16_t raw;
	uint16_t normalized;

	moving_hysteresis_t moving_hysteresis;
	moving_average_t moving_average;

	int ch_id;

} one_ad_channel_t;

typedef struct {

one_ad_channel_t my_adc_channels[ADC_BUFFER_LENGTH];
} adc_channels_t;

extern adc_channels_t my_adcs;

/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */

uint16_t get_moving_average(uint16_t raw, int ch);
uint16_t get_moving_hysteresis(uint16_t raw, int ch);


/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

