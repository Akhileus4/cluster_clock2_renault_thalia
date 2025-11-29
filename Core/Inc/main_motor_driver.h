// main_motor_driver.h (unchanged, but ensure it's included)
#ifndef MAIN_MOTOR_DRIVER_H
#define MAIN_MOTOR_DRIVER_H

#include "stm32g0xx_hal.h"

void motor_init(void);
void motor_rotate(int direction, int steps);
void motor_move_to(int target_pos);
void motor_move_to_kmh(int kmh);

extern int motor_position;

// Lookup table a leggyakoribb értékekhez
typedef struct {
	int kmh;
	int position;
} kmh_position_map_t;

static const kmh_position_map_t position_map[] = { { 0, 0 }, { 10, 22 }, //updated
		{ 20, 40 },		//updated
		{ 30, 58 }, 	//updated
		{ 40, 75 }, 	//updated
		{ 50, 92 },		//updated
		{ 60, 108 },		//updated
		{ 70, 125 }, 	//updated
		{ 80, 140 }, 	//updated
		{ 90, 157 }, 	//updated
		{ 100, 172 }, 	//updated
		{ 110, 189 }, 	//updated
		{ 120, 205 }, 	//updated
		{ 130, 210 }, 	//updated
		{ 140, 230 }, 	//updated
		{ 150, 238 }, 	//updated
		{ 160, 253 }, 	//updated
		{ 170, 270 }, 	//updated
		{ 180, 285 }, 	//updated
		{ 190, 305 }, 	//updated
		{ 200, 320 }, 	//updated
		{ 210, 339 } }; //updated

#endif /* MAIN_MOTOR_DRIVER_H */
