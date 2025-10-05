/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    adc.c
 * @brief   This file provides code for the configuration
 *          of the ADC instances.
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
#include "adc.h"

/* USER CODE BEGIN 0 */

uint16_t adc_buffer[ADC_BUFFER_LENGTH];  // PA0..PA3 értékei
adc_channels_t my_adcs;

uint16_t get_moving_hysteresis(uint16_t raw, int ch) {

	if (raw > my_adcs.my_adc_channels[ch].moving_hysteresis.hyst_max) {

		my_adcs.my_adc_channels[ch].moving_hysteresis.hyst_max +=
				my_adcs.my_adc_channels[ch].moving_hysteresis.delta;
		my_adcs.my_adc_channels[ch].moving_hysteresis.hyst_min +=
				my_adcs.my_adc_channels[ch].moving_hysteresis.delta;

		return my_adcs.my_adc_channels[ch].moving_hysteresis.hyst_max;
	}

	if (raw < my_adcs.my_adc_channels[ch].moving_hysteresis.hyst_min) {

		my_adcs.my_adc_channels[ch].moving_hysteresis.hyst_max -=
				my_adcs.my_adc_channels[ch].moving_hysteresis.delta;
		my_adcs.my_adc_channels[ch].moving_hysteresis.hyst_min -=
				my_adcs.my_adc_channels[ch].moving_hysteresis.delta;

		if ((my_adcs.my_adc_channels[ch].moving_hysteresis.hyst_min < 10)
				|| (my_adcs.my_adc_channels[ch].moving_hysteresis.hyst_max < 10)) {
			my_adcs.my_adc_channels[ch].moving_hysteresis.hyst_min = 0;
			my_adcs.my_adc_channels[ch].moving_hysteresis.hyst_max = 0;
		}

		return my_adcs.my_adc_channels[ch].moving_hysteresis.hyst_min;
	}

	return raw;
}

uint16_t get_moving_average(uint16_t raw, int ch) {

	my_adcs.my_adc_channels[ch].moving_average.queue[my_adcs.my_adc_channels[ch].moving_average.index] =
			raw;

	my_adcs.my_adc_channels[ch].moving_average.index =
			(my_adcs.my_adc_channels[ch].moving_average.index + 1)
					% AD_AVERAGE_QUEUE_SIZE;

	uint32_t sum = 0;

	for (int i = 0; i < AD_AVERAGE_QUEUE_SIZE; ++i) {
		sum += my_adcs.my_adc_channels[ch].moving_average.queue[i];
	}
	return sum / AD_AVERAGE_QUEUE_SIZE;
}

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 4;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_160CYCLES_5;
  hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

	bzero(&my_adcs, sizeof(my_adcs));

	for (int i = 0; i < ADC_BUFFER_LENGTH; i++) {
		my_adcs.my_adc_channels[i].ch_id = i;

		my_adcs.my_adc_channels[i].raw = 0;
		my_adcs.my_adc_channels[i].normalized = 0;
		my_adcs.my_adc_channels[i].moving_hysteresis.delta =
				AD_MOVIN_HYSTERESIS_DELTA;
		my_adcs.my_adc_channels[i].moving_hysteresis.hyst_max =
				my_adcs.my_adc_channels[i].raw + AD_MOVIN_HYSTERESIS_DELTA;
		my_adcs.my_adc_channels[i].moving_hysteresis.hyst_min =
				my_adcs.my_adc_channels[i].raw - AD_MOVIN_HYSTERESIS_DELTA;

	}

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_buffer, ADC_BUFFER_LENGTH);

  /* USER CODE END ADC1_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA0     ------> ADC1_IN0
    PA1     ------> ADC1_IN1
    PA2     ------> ADC1_IN2
    PA3     ------> ADC1_IN3
    */
    GPIO_InitStruct.Pin = BOOST_ADC_Pin|USB_ADC_Pin|VBATT_ADC_Pin|LIGH_SENS_ADC_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Request = DMA_REQUEST_ADC1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA0     ------> ADC1_IN0
    PA1     ------> ADC1_IN1
    PA2     ------> ADC1_IN2
    PA3     ------> ADC1_IN3
    */
    HAL_GPIO_DeInit(GPIOA, BOOST_ADC_Pin|USB_ADC_Pin|VBATT_ADC_Pin|LIGH_SENS_ADC_Pin);

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
