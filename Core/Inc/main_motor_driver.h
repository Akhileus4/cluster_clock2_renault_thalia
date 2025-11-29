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

static const kmh_position_map_t position_map[] = { { 0, 0 }, { 10, 18 }, //updated
		{ 20, 19 },		//updated
		{ 30, 38 }, 	//updated
		{ 40, 51 }, 	//updated
		{ 50, 73 },		//updated
		{ 60, 90 },		//updated
		{ 70, 106 }, 	//updated
		{ 80, 122 }, 	//updated
		{ 90, 137 }, 	//updated
		{ 100, 153 }, 	//updated
		{ 110, 169 }, 	//updated
		{ 120, 185 }, 	//updated
		{ 130, 202 }, 	//updated
		{ 140, 218 }, 	//updated
		{ 150, 238 }, 	//updated
		{ 160, 253 }, 	//updated
		{ 170, 270 }, 	//updated
		{ 180, 285 }, 	//updated
		{ 190, 305 }, 	//updated
		{ 200, 320 }, 	//updated
		{ 210, 339 } }; //updated

#endif /* MAIN_MOTOR_DRIVER_H */
