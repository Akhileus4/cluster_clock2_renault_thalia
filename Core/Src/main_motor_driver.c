// main_motor_driver.c - Fixed for bidirectional movement (CW and CCW)
#include "main_motor_driver.h"
#include "main.h"
#include "stdlib.h"

// Step delay in ms (your working 1 ms)
#define STEP_DELAY_MS 2
int motor_position = 0;  // 0 az abszolút nullpont

// Single full-step sequence (standard bipolar for CW)
static const uint8_t sequence[4][4] = { { 1, 0, 0, 1 },  // Phase 0: A+ B-
		{ 0, 1, 0, 1 },  // Phase 1: A- B-
		{ 0, 1, 1, 0 },  // Phase 2: A- B+
		{ 1, 0, 1, 0 }   // Phase 3: A+ B+
};

void motor_init(void) {
	// Enable drivers
	HAL_GPIO_WritePin(SERVO_ENL1_GPIO_Port, SERVO_ENL1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SERVO_ENL2_GPIO_Port, SERVO_ENL2_Pin, GPIO_PIN_SET);

	// Initial coast (all low)
	HAL_GPIO_WritePin(SERVO1_GPIO_Port, SERVO1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SERVO2_GPIO_Port, SERVO2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SERVO3_GPIO_Port, SERVO3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SERVO4_GPIO_Port, SERVO4_Pin, GPIO_PIN_RESET);

	motor_rotate(-1, 500);
}

static void set_phase(int phase) {
	// Set all pins simultaneously
	HAL_GPIO_WritePin(SERVO1_GPIO_Port, SERVO1_Pin,
			(GPIO_PinState) sequence[phase][0]);
	HAL_GPIO_WritePin(SERVO2_GPIO_Port, SERVO2_Pin,
			(GPIO_PinState) sequence[phase][1]);
	HAL_GPIO_WritePin(SERVO3_GPIO_Port, SERVO3_Pin,
			(GPIO_PinState) sequence[phase][2]);
	HAL_GPIO_WritePin(SERVO4_GPIO_Port, SERVO4_Pin,
			(GPIO_PinState) sequence[phase][3]);
}

void motor_rotate(int direction, int steps) {
	if (steps <= 0)
		return;

	// Standard bidirectional: One sequence, reverse stepping for CCW
	int start_phase = (direction > 0) ? 0 : 3;  // CW start at 0, CCW at 3
	int phase_step = (direction > 0) ? 1 : -1;   // CW +1, CCW -1
	int num_phases = 4;
	int current_phase = start_phase;

	for (int i = 0; i < steps; ++i) {
		set_phase(current_phase);
		HAL_Delay(STEP_DELAY_MS);

		// Advance phase (modulo handles wrap-around)
		current_phase = (current_phase + phase_step + num_phases) % num_phases;
	}
	HAL_Delay(1);
	// Coast: all inputs low
	HAL_GPIO_WritePin(SERVO1_GPIO_Port, SERVO1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SERVO2_GPIO_Port, SERVO2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SERVO3_GPIO_Port, SERVO3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SERVO4_GPIO_Port, SERVO4_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
}

// Globális változó a motor aktuális abszolút pozíciójához

// Abszolút pozícióra léptető függvény
// target_pos: 0-hoz képest kívánt lépés
void motor_move_to(int target_pos) {
	int delta = target_pos - motor_position; // hány lépést kell lépni

	if (delta == 0)
		return; // nincs lépés

	int direction = (delta > 0) ? 1 : -1; // pozitív = jobbra(CW), negatív = balra(CCW)
	int steps = abs(delta);

	motor_rotate(direction, steps); // hívjuk a meglévő függvényt
	motor_position = target_pos;    // frissítjük az aktuális pozíciót
}

void motor_move_to_kmh(int kmh) {
	// Korlátozzuk a kmh értéket
	if (kmh < 10)
		kmh = 10;

	if (kmh > 210)
		kmh = 210;

	int target_position = 0;
	int map_size = sizeof(position_map) / sizeof(position_map[0]);

	// Keressük meg a pontos egyezést vagy interpoláljunk
	for (int i = 0; i < map_size; i++) {
		if (position_map[i].kmh == kmh) {
			target_position = position_map[i].position;
			break;
		}
		// Ha a kmh érték a jelenlegi és következő pont között van
		if (i < map_size - 1 && kmh > position_map[i].kmh
				&& kmh < position_map[i + 1].kmh) {
			// Lineáris interpoláció a két pont között

			/*
			 int kmh1 = position_map[i].kmh;
			 int pos1 = position_map[i].position;
			 int kmh2 = position_map[i + 1].kmh;
			 int pos2 = position_map[i + 1].position;

			 target_position = pos1 + (kmh - kmh1) * (pos2 - pos1) / (kmh2 - kmh1);

			 */

			target_position = position_map[i].position
					+ (kmh - position_map[i].kmh)
							* (position_map[i + 1].position
									- position_map[i].position)
							/ (position_map[i + 1].kmh - position_map[i].kmh);
			break;
		}
	}

	motor_move_to(target_position);
}

