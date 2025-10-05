#include "small_motor_DAC.h"
#include "LP5036RJVR_led_driver.h"

extern I2C_HandleTypeDef hi2c1;   // CubeMX által generált I2C1

// motor + DAC címek
uint8_t dac_addresses[] = { 0b0001000, 0b0001001, 0b0001010, 0b1001100,
		0b1001101, 0b1001110 };

s_motors_t left_motor = { "left", 0, 0b0001000, 0b0001001, 0b0001010 };
s_motors_t right_motor = { "right", 0, 0b1001100, 0b1001101, 0b1001110 };

const uint8_t broadcast_address = 0b1001000;

/**
 * Alap inicializálás: minden DAC kimenet 0-ra állítása
 */
int small_motor_dac_init(void) {

	left_motor.position = 0;
	right_motor.position = 0;

	return HAL_OK;
}

int small_motor_set_dac(uint8_t dac_addr, uint16_t value) {
	if (value > 4095)
		value = 4095;

	uint8_t data[2];
	uint8_t pd = 0; // normal operation
	data[0] = (uint8_t) (((pd & 0x03) << 4) | ((value >> 8) & 0x0F)); // upper byte: 00 PD1 PD0 D11..D8
	data[1] = (uint8_t) (value & 0xFF); // lower byte: D7..D0

	if (HAL_I2C_Master_Transmit(&hi2c1, dac_addr << 1, data, 2,
	HAL_MAX_DELAY) != HAL_OK) {

		for (uint8_t i = 0; i < 6; i++) {
			if (dac_addresses[i] == dac_addr) {
				write_bit(&My_Error_code, i + 2, 1);
				break;
			}

		}

		return HAL_ERROR;
	}

	return HAL_OK;

}

/**
 * Pozíció %-ban (0–100), motor 3 DAC közül az elsőt állítjuk példának
 */

/*int small_motor_set_position(s_motors_t *s_motor, uint8_t pos_percent) {
 if (pos_percent > 100) pos_percent = 100;

 uint16_t value = (pos_percent * 4095) / 100;
 s_motor->position = value;

 if (value > 4095) value = 4095;

 uint8_t data[2];
 data[0] = (value >> 4) & 0xFF;       // D11..D4
 data[1] = (value & 0x0F) << 4;       // D3..D0 + padding

 HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(
 &hi2c1,
 dac << 1,   // 7 bites cím balra shiftelve
 data,
 2,
 HAL_MAX_DELAY
 );

 return (ret == HAL_OK) ? HAL_OK : -1;
 }
 */
