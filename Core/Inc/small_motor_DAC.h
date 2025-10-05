/*
 * small_motor_DAC.h
 *
 *  Created on: Sep 18, 2025
 *      Author: admin
 */

#ifndef INC_SMALL_MOTOR_DAC_H_
#define INC_SMALL_MOTOR_DAC_H_

#include "main.h"   // hogy legyen GPIO és I2C definiálva




typedef struct {
    const char *name;
    int position;
    uint8_t l1_dac_addr;
    uint8_t l2_dac_addr;
    uint8_t l3_dac_addr;
} s_motors_t;


int small_motor_dac_init(void);
int small_motor_set_dac(uint8_t dac_addr, uint16_t value);
int small_motor_set_position(s_motors_t *s_motor,uint8_t dac, uint8_t pos_percent);

extern s_motors_t left_motor;
extern s_motors_t right_motor;

#endif /* INC_SMALL_MOTOR_DAC_H_ */
