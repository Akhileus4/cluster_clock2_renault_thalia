/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

extern uint32_t My_Error_code;

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

void log_reset_reason(void);

uint8_t read_bit(uint32_t value, uint8_t bit_pos); // Adott bit értékének kiolvasása (0 vagy 1)

void write_bit(uint32_t *value, uint8_t bit_pos, uint8_t bit_value); // Adott bit beállítása 0-ra vagy 1-re

void show_error();

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LD_EN_Pin GPIO_PIN_11
#define LD_EN_GPIO_Port GPIOC
#define STM_LED_Pin GPIO_PIN_12
#define STM_LED_GPIO_Port GPIOC
#define V3_BUCK_RST_Pin GPIO_PIN_13
#define V3_BUCK_RST_GPIO_Port GPIOC
#define SERVO_ENL2_Pin GPIO_PIN_0
#define SERVO_ENL2_GPIO_Port GPIOF
#define BOOST_ADC_Pin GPIO_PIN_0
#define BOOST_ADC_GPIO_Port GPIOA
#define USB_ADC_Pin GPIO_PIN_1
#define USB_ADC_GPIO_Port GPIOA
#define VBATT_ADC_Pin GPIO_PIN_2
#define VBATT_ADC_GPIO_Port GPIOA
#define LIGH_SENS_ADC_Pin GPIO_PIN_3
#define LIGH_SENS_ADC_GPIO_Port GPIOA
#define LED_I2C_CLK_Pin GPIO_PIN_10
#define LED_I2C_CLK_GPIO_Port GPIOB
#define LED_IC2_DATA_Pin GPIO_PIN_11
#define LED_IC2_DATA_GPIO_Port GPIOB
#define MINUTE_Pin GPIO_PIN_12
#define MINUTE_GPIO_Port GPIOB
#define HOUR_Pin GPIO_PIN_13
#define HOUR_GPIO_Port GPIOB
#define SERVO1_Pin GPIO_PIN_8
#define SERVO1_GPIO_Port GPIOA
#define JBM_I2C_CLK_Pin GPIO_PIN_9
#define JBM_I2C_CLK_GPIO_Port GPIOA
#define SERVO4_Pin GPIO_PIN_11
#define SERVO4_GPIO_Port GPIOA
#define VAKU_Pin GPIO_PIN_1
#define VAKU_GPIO_Port GPIOD
#define SERVO2_Pin GPIO_PIN_4
#define SERVO2_GPIO_Port GPIOB
#define SERVO3_Pin GPIO_PIN_6
#define SERVO3_GPIO_Port GPIOB
#define JBM_I2C_DATA_Pin GPIO_PIN_7
#define JBM_I2C_DATA_GPIO_Port GPIOB
#define BOOST_EN_Pin GPIO_PIN_8
#define BOOST_EN_GPIO_Port GPIOB
#define SERVO_ENL1_Pin GPIO_PIN_9
#define SERVO_ENL1_GPIO_Port GPIOB
#define LEDS_ON_Pin GPIO_PIN_10
#define LEDS_ON_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
