// main_motor_driver.h (unchanged, but ensure it's included)
#ifndef MAIN_MOTOR_DRIVER_H
#define MAIN_MOTOR_DRIVER_H

#include "stm32g0xx_hal.h"

void motor_init(void);
void motor_rotate(int direction, int steps);
void motor_move_to(int target_pos);
void motor_move_to_kmh(int kmh);

extern int motor_position;

#endif /* MAIN_MOTOR_DRIVER_H */
