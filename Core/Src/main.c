/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "LP5036RJVR_led_driver.h"
#include "main_motor_driver.h"
#include "small_motor_DAC.h"
#include "string.h"
#include "tasks.h"
#include "stdio.h"
#include "stdbool.h"
#include "button.h"
#include "time_set.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

uint32_t My_Error_code = 0;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
Button_t btnHour;
Button_t btnMin;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//&&
//(vbatt_avg < 2669 	&& 	vbatt_avg 	> 4515)
//    	(usb_avg < 2669 	&& 	usb_avg 	> 3413) &&
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_I2C2_Init();
	MX_RTC_Init();
	MX_TIM17_Init();
	MX_USART1_UART_Init();
	MX_I2C1_Init();
	MX_CRC_Init();
	/* USER CODE BEGIN 2 */

	HAL_GPIO_WritePin(LEDS_ON_GPIO_Port, LEDS_ON_Pin, 0);
	HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, 1);
	HAL_GPIO_WritePin(STM_LED_GPIO_Port, STM_LED_Pin, 0);
	HAL_GPIO_WritePin(V3_BUCK_RST_GPIO_Port, V3_BUCK_RST_Pin, 0);

	HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1); //timer start for vaku led

	Task_Init();

//LED init
	if (led_driver_init() != HAL_OK) {
		write_bit(&My_Error_code, 0, 1);
		Error_Handler();
	}

	for (int i = 0; i < led_count; i++) {
		if (set_led(leds[i].channel, 0) != HAL_OK) {
			if (i == 6) {
				set_vaku_led(0);
			}
			write_bit(&My_Error_code, 1, 1);
			Error_Handler();
		}

	}

	log_reset_reason();
	show_error();
	HAL_Delay(1000);

	for (int i = 0; i < led_count; i++) {
		if (set_led(leds[i].channel, 0) != HAL_OK) {
			Error_Handler();
		}
	}

	set_vaku_led(0);

	set_background_brightness(100);

//  motor inits
	motor_init();

	Button_Init(&btnHour, MINUTE_GPIO_Port, MINUTE_Pin);
	Button_Init(&btnMin, HOUR_GPIO_Port, HOUR_Pin);

	TimeSet_Init(&btnHour, &btnMin);

	set_vaku_led(0);

	Task_Enable(task_Big_Motor);
	Task_Enable(task_Big_Motor);
	Task_Enable(task_left_blink);
	Task_Enable(task_right_blink);
	Task_Enable(task_smotor_right);
	Task_Enable(task_smotor_left);
	Task_Enable(task_adc_evaluation);

	HAL_Delay(1000);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		/*
		 if (!HAL_GPIO_ReadPin(HOUR_GPIO_Port, HOUR_Pin)) {
		 set_vaku_led(100);
		 } else {
		 set_vaku_led(0);
		 }
		 if (!HAL_GPIO_ReadPin(MINUTE_GPIO_Port, MINUTE_Pin)) {
		 set_led(MENETFENY, 100);
		 } else {
		 set_led(MENETFENY, 0);

		 }
		 */
		Task_Dispatch();

	}
	/* USER CODE END 3 */
}
/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Configure LSE Drive Capability
	 */
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI
			| RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
	RCC_OscInitStruct.PLL.PLLN = 8;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}

	/** Enables the Clock Security System
	 */
	HAL_RCC_EnableLSECSS();
}

/* USER CODE BEGIN 4 */

void log_reset_reason(void) {
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST)) { /* log "IWDG reset" */

		set_led(BIZTIOV, 20);
		write_bit(&My_Error_code, 14, 1);

	}
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST)) { /* log "WWDG reset" */

		set_led(IZZITO, 20);
		write_bit(&My_Error_code, 15, 1);

	}
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST)) { /* log "POR reset"  */

		set_led(NYITOTT_AJTO, 20);
		write_bit(&My_Error_code, 16, 1);

	}
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST)) { /* log "Software reset" */

		set_led(ABS, 20);
		write_bit(&My_Error_code, 17, 1);

	}
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_PWRRST)) { /* log "BOR reset" */

		set_led(KEZIFEK, 20);
		write_bit(&My_Error_code, 18, 1);

	}
	__HAL_RCC_CLEAR_RESET_FLAGS();

}

// Adott bit értékének kiolvasása (0 vagy 1)
uint8_t read_bit(uint32_t value, uint8_t bit_pos) {
	return (value >> bit_pos) & 0x1;
}

// Adott bit beállítása 0-ra vagy 1-re
void write_bit(uint32_t *value, uint8_t bit_pos, uint8_t bit_value) {
	if (bit_value)
		*value |= (1UL << bit_pos);  // bit beállítása 1-re
	else
		*value &= ~(1UL << bit_pos);  // bit törlése (0-ra)
}

void show_error() {

	for (int i = 2; i <= 24; i++) {
		if (read_bit(My_Error_code, i)) {

			switch (i) {
			case 2:
				set_led(UZEMANYAGSZINT, 100);
				break;

			case 3:
				set_led(DOBFEK, 100);
				break;

			case 4:
				set_led(HATSO_ABLAK_FUTES, 100);
				break;

			case 5:
				set_led(HUTOVIZ, 100);
				break;

			case 6:
				set_led(POTTY, 100);
				break;

			case 7:
				set_led(LEGZSAK_KI, 100);
				break;

			case 8:
				set_led(AKSI, 100);
				break;

			case 9:
				set_led(AKSI, 10);
				break;

			case 10:
				set_led(OLAJNYOMAS, 100);
				break;

			case 11:
				set_led(OLAJNYOMAS, 10);
				break;
			case 12:
				set_led(ABLAKOMOSO_SZINT, 100);
				break;

			case 13:
				set_led(ABLAKOMOSO_SZINT, 10);
				break;

			case 14:
				set_led(BIZTIOV, 100);
				break;

			case 15:
				set_led(IZZITO, 100);
				break;

			case 16:
				set_led(NYITOTT_AJTO, 100);
				break;

			case 17:
				set_led(ABS, 100);
				break;

			case 18:
				set_led(KEZIFEK, 100);
				break;

			case 19:
				set_led(MENETFENY, 100);
				break;

			case 20:
				set_led(ELSO_KOD, 100);
				break;

			case 21:
				set_led(HATSO_KOD, 100);
				break;

			case 22:
				set_led(INDEX_BALLRA, 100);
				break;

			case 23:
				set_led(INDEX_JOBBRA, 100);
				break;

			case 24:
				set_led(LEGZSAK_HIBA, 100);
				break;
			}
		}
	}
}

void HAL_SYSTICK_Callback(void) {

	ButtonEvent_t eH = Button_Update(&btnHour);
	ButtonEvent_t eM = Button_Update(&btnMin);

	if (eH != BTN_NO_EVENT) {
		printf("HOUR event = %d\n", eH);
	}
	if (eM != BTN_NO_EVENT) {
		printf("MIN event = %d\n", eM);
	}

	TimeSet_Tick();    // 1ms tick a beállító logikához
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
//__disable_irq();
	Task_Disable(task_Big_Motor);
	Task_Disable(task_left_blink);
	Task_Disable(task_right_blink);
//	Task_Disable(task_adc_evaluation);

	HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, 0);
	HAL_GPIO_WritePin(LEDS_ON_GPIO_Port, LEDS_ON_Pin, 0);

	uint8_t enable_led_driver_usage = 1;
	uint8_t vaku_led_brightness = 0;

	if ((read_bit(My_Error_code, 0)) == 1) { //led driver init hiba

		enable_led_driver_usage = 0;
		vaku_led_brightness = 5;

	} else {

		if (read_bit(My_Error_code, 1)) {
			vaku_led_brightness = 100;
		}

		for (int i = 0; i < led_count; i++) {
			set_led(leds[i].channel, 0);
		}

		set_background_brightness(20);
		set_vaku_led(0);

		//	set_led(My_Error_code, 100);
	}

	while (1) {

		if (enable_led_driver_usage == 1) { //not a led driver error init error

			set_led(CHECK_ENGINE, 50);
			set_vaku_led(vaku_led_brightness);

			HAL_Delay(1000);

			set_led(CHECK_ENGINE, 0);
			set_vaku_led(0);

			HAL_Delay(1000);

			show_error();

		} else { //led driver init error blink

			set_vaku_led(5);
			HAL_Delay(1000);
			set_vaku_led(0);
			HAL_Delay(1000);

		}

	}
	/* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
