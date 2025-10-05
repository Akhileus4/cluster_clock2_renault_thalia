#include "LP5036RJVR_led_driver.h"
#include <string.h>   // strcmp
#include "tim.h"

#define LP5036_I2C_ADDR   (0x38)   // 7-bit cím
#define REG_DEVICE_CONFIG0  0x00
#define REG_DEVICE_CONFIG1  0x01
#define MAX_DUTY 100 // maximum duty cycle of the LEDS
#define MIN_DUTY 10

extern I2C_HandleTypeDef hi2c2;   // a CubeMX által generált I2C2

uint32_t My_LED_Error_code = 0;

// LED

int Vaku_led_offset = 0;
uint8_t global_offset = 0; // 0 - 255
uint8_t data[2];

Led_t leds[] = { { "ABLAKOMOSO_SZINT", 2, 0, 0x16, 0, },   //0
		{ "AKSI", 3, 0, 0x17, 0, },   //1
		{ "ELSO_KOD", 4, 0, 0x18, 0, },   //2
		{ "HATSO_KOD", 5, 0, 0x19, 0, },   //3
		{ "LEGZSAK_HIBA", 6, 0, 0x1A, 0, },   //4
		{ "MENETFENY", 7, 0, 0x1B, 0, },   //5
		{ "LD_BG_1", 8, 0, 0x1C, 0, },   //6
		{ "HATSO_ABLAK_FUTES", 9, 0, 0x1D, 0, },   //7
		{ "DOBFEK", 10, 0, 0x1E, 0, },   //8
		{ "UZEMANYAGSZINT", 11, 0, 0x1F, 0, },   //9
		{ "INDEX_BALLRA", 12, 0, 0x20, 0, },   //10
		{ "LD_BG_2", 13, 0, 0x21, 0, },   //11
		{ "LD_BG_3", 14, 0, 0x22, 0, },   //12
		{ "LD_BG_4", 15, 0, 0x23, 0, },   //13
		{ "INDEX_JOBBRA", 24, 0, 0x2C, 0, },   //14
		{ "HUTOVIZ", 25, 0, 0x2D, 0, },   //15
		{ "POTTY", 26, 0, 0x2E, 0, },   //16
		{ "LEGZSAK_KI", 27, 0, 0x2F, 0, },   //17
		{ "LD_BG_5", 28, 0, 0x30, 0, },   //18
		{ "BIZTIOV", 29, 0, 0x31, 0, },   //19
		{ "KEZIFEK", 30, 0, 0x32, 0, },   //20
		{ "ABS", 31, 0, 0x33, 0, },   //21
		{ "NYITOTT_AJTO", 32, 0, 0x34, 0, },   //22
		{ "IZZITO", 33, 0, 0x35, 0, },   //23
		{ "OLAJNYOMAS", 34, 0, 0x36, 0, },   //24
		{ "CHECK_ENGINE", 35, 0, 0x37, 0, },   //25
		};

const uint8_t banks_reg[] = { 0x05, 0x06, 0x07 };
const uint8_t led_brightness_reg[] = { 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
		0x0F, 0x10, 0x11, 0x12, 0x13 };

const int led_count = sizeof(leds) / sizeof(Led_t);
const int banks_reg_count = sizeof(banks_reg) / sizeof(uint8_t);
const int led_br_count = sizeof(led_brightness_reg) / sizeof(uint8_t);

/// --- LED Driver INIT ---
int led_driver_init(void) {
	HAL_StatusTypeDef ret;
	uint8_t data[2];

	// 1. Enable pin high
	HAL_GPIO_WritePin(LD_EN_GPIO_Port, LD_EN_Pin, 0);
	HAL_Delay(100); // kis várakozás
	HAL_GPIO_WritePin(LD_EN_GPIO_Port, LD_EN_Pin, 1);
	HAL_Delay(100); // kis várakozás
	HAL_GPIO_WritePin(LD_EN_GPIO_Port, LD_EN_Pin, 0);
	HAL_Delay(100); // kis várakozás
	HAL_GPIO_WritePin(LD_EN_GPIO_Port, LD_EN_Pin, 1);
	HAL_Delay(100); // kis várakozás

	// 1.1 reset
	data[0] = 0x38;
	data[1] = 0x00;
	ret = HAL_I2C_Master_Transmit(&hi2c2, LP5036_I2C_ADDR, data, 2,
	HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		write_bit(&My_LED_Error_code, 26, 1);
		return -1;
	}

	// 2. CONFIG0 = CHIP_EN + AUTO_INC (0xC0)
	data[0] = REG_DEVICE_CONFIG0;
	data[1] = 0xFF;
	ret = HAL_I2C_Master_Transmit(&hi2c2, LP5036_I2C_ADDR, data, 2,
	HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		write_bit(&My_LED_Error_code, 27, 1);
		return -1;
	}

	// 3. CONFIG1 = LOGIC_EN + PWM_EN (0x03)
	data[0] = REG_DEVICE_CONFIG1;
	data[1] = 0x22;
	ret = HAL_I2C_Master_Transmit(&hi2c2, LP5036_I2C_ADDR, data, 2,
	HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		write_bit(&My_LED_Error_code, 28, 1);
		return -1;
	}

	//LED_CONFIG0 (Address = 2h) [reset = 00h]
	data[0] = 0x02;
	data[1] = 0x00;
	ret = HAL_I2C_Master_Transmit(&hi2c2, LP5036_I2C_ADDR, data, 2,
	HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		write_bit(&My_LED_Error_code, 27, 1);
		return -1;
	}

	//LED_CONFIG1 (Address = 3h) [reset = 00h]
	data[0] = 0x03;
	data[1] = 0x00;
	ret = HAL_I2C_Master_Transmit(&hi2c2, LP5036_I2C_ADDR, data, 2,
	HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		write_bit(&My_LED_Error_code, 28, 1);
		return -1;
	}

	//BANK_BRIGHTNESS (Address = 4h) [reset = FFh]
	data[0] = 0x04;
	data[1] = 0xFF;
	ret = HAL_I2C_Master_Transmit(&hi2c2, LP5036_I2C_ADDR, data, 2,
	HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		write_bit(&My_LED_Error_code, 29, 1);
		return -1;
	}

	//BANK_ABC_COLOR (Address = 5h 6h 7h) [reset = 00h
	data[1] = 0xff;
	for (int i = 0; i < banks_reg_count; i++) {
		data[0] = banks_reg[i];
		ret = HAL_I2C_Master_Transmit(&hi2c2, LP5036_I2C_ADDR, data, 2,
		HAL_MAX_DELAY);
		if (ret != HAL_OK) {
			write_bit(&My_LED_Error_code, 30, 1);
			return -1;
		}
	}

	data[1] = 0xff;
	for (int i = 0; i < led_br_count; i++) {
		data[0] = led_brightness_reg[i];
		ret = HAL_I2C_Master_Transmit(&hi2c2, LP5036_I2C_ADDR, data, 2,
		HAL_MAX_DELAY);
		if (ret != HAL_OK) {
			write_bit(&My_LED_Error_code, 31, 1);
			return -1;
		}

	}

	//startup play

	for (int i = 0; i < led_count; i++) {

		for (int j = 0; j < 20; j++) {

			if (i == 6) {
				set_vaku_led(j);
			}

			set_led(leds[i].channel, j);

			HAL_Delay(1);

		}

	}

	return 0; // OK
}

int set_background_brightness(uint8_t duty_percent) {
	if (set_led(LD_BG_1, duty_percent) != HAL_OK) {
		return -1;
	}
	//  HAL_Delay(10);
	if (set_led(LD_BG_2, duty_percent) != HAL_OK) {
		return -1;
	}
	//  HAL_Delay(10);
	if (set_led(LD_BG_3, duty_percent) != HAL_OK) {
		return -1;
	}
	//  HAL_Delay(10);
	if (set_led(LD_BG_4, duty_percent) != HAL_OK) {
		return -1;
	}
	//  HAL_Delay(10);
	if (set_led(LD_BG_5, duty_percent) != HAL_OK) {
		return -1;
	}
	//  HAL_Delay(10);

	return HAL_OK;
}

/// --- LED beállítás csatorna alapján ---
int set_led(uint8_t channel, uint8_t duty_percent) {

	data[0] = 0;
	data[1] = 0;

	if (channel > 35)
		return -1;   // nincs ilyen csatorna

	int led_id_count = 0;
	for (led_id_count = 0; led_id_count < led_count; led_id_count++) {
		if (leds[led_id_count].channel == channel) {

			if (duty_percent > 0) {
				if (duty_percent > MAX_DUTY) {
					duty_percent = MAX_DUTY; // max definiált aktuális 20%
				}
			}

			// Átszámítás % -> 0-255
			duty_percent = (duty_percent * 255) / 100;

			//duty cycle offset és global offset bevétele a beállításba
			if (duty_percent != 0) {

				if (leds[led_id_count].duty_offset < duty_percent) {
					duty_percent -= leds[led_id_count].duty_offset;

					if (global_offset < duty_percent) {
						duty_percent -= global_offset;
					}

				} else {
					if (duty_percent < MIN_DUTY) {
						duty_percent = MIN_DUTY;
					}
				}
			}

			leds[led_id_count].duty = duty_percent;

			data[0] = leds[led_id_count].reg_addr;
			data[1] = leds[led_id_count].duty;
			break;
		}
	}

	// Írás I2C-n
	if (HAL_I2C_Master_Transmit(&hi2c2, LP5036_I2C_ADDR, data, 2, HAL_MAX_DELAY)
			!= HAL_OK) {
		write_bit(&My_LED_Error_code, led_id_count, 1);
		write_bit(&My_Error_code, 1, 1);
		return HAL_ERROR;  // hiba
	}

	return HAL_OK; // siker
}

/// --- LED beállítás név alapján ---
int set_led_by_name(const char *name, uint8_t duty) {
	for (int i = 0; i < led_count; i++) {
		if (strcmp(leds[i].name, name) == 0) {
			return set_led(leds[i].channel, duty);
		}
	}
	return -1;
}

int set_vaku_led(uint8_t percent) {

	if (percent > MAX_DUTY)
		percent = MAX_DUTY; // limitáljuk 100%-ra

	uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim17);

	uint32_t ccr = ((uint32_t) percent * arr) / 100;

	if (percent > 0) {

		if ((ccr - global_offset) > 0) {
			ccr -= global_offset;
		}
	}

	__HAL_TIM_SET_COMPARE(&htim17, TIM_CHANNEL_1, ccr);

	return 1;
}

int index_blink(const char *name, uint8_t duty) {

	if (strcmp(name, "jobb") == 0) {
		set_led(INDEX_JOBBRA, duty);
	}

	if (strcmp(name, "bal") == 0) {
		set_led(INDEX_BALLRA, duty);
	}

	return 1;

}

