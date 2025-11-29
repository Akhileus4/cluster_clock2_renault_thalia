#include "tasks.h"
#include "main.h"
#include "adc.h"
#include "small_motor_DAC.h"
#include "LP5036RJVR_led_driver.h"
#include "main_motor_driver.h"
#include "rtc.h"

#define MAX_TASKS 100
Task_t tasks[MAX_TASKS];

//commented old //void task_Big_Motor() task, kep for history
//void task_Big_Motor() {
//
//	static int wait_time = 3000;
//
//	static uint8_t state = 0;
//	static uint32_t last = 0;
//	uint32_t now = HAL_GetTick();
//
//	switch (state) {
//
//	case 0:
//
//		if (now - last >= wait_time) {
//
//			motor_move_to_kmh(0);
//
//			//		set_led(INDEX_BALLRA, 0);
//			//		set_led(INDEX_JOBBRA, 0);
//
//			//		Task_Enable(task_left_blink);
//			//		Task_Disable(task_right_blink);
//
//			last = now;
//			state = 1;
//		}
//		break;
//
//	case 1:
//
//		if (now - last >= wait_time) {
//
//			motor_move_to_kmh(50);
//
//			//		set_led(INDEX_BALLRA, 0);
//			//		set_led(INDEX_JOBBRA, 0);
//
//			//		Task_Enable(task_left_blink);
//			//		Task_Disable(task_right_blink);
//
//			last = now;
//			state = 2;
//		}
//
//		break;
//
//	case 2:
//
//		if (now - last >= wait_time) {
//
//			motor_move_to_kmh(140);
//
//			//		set_led(INDEX_BALLRA, 0);
//			//		set_led(INDEX_JOBBRA, 0);
//
//			//		Task_Enable(task_right_blink);
//			//		Task_Disable(task_left_blink);
//
//			last = now;
//			state = 3;
//		}
//
//		break;
//
//	case 3:
//
//		if (now - last >= wait_time) {
//
//			motor_move_to_kmh(180);
//
//			//		set_led(INDEX_BALLRA, 0);
//			//		set_led(INDEX_JOBBRA, 0);
//
//			//		Task_Enable(task_left_blink);
//			//		Task_Disable(task_right_blink);
//
//			last = now;
//			state = 4;
//		}
//
//		break;
//	case 4:
//
//		if (now - last >= wait_time) {
//
//			motor_move_to_kmh(20);
//
//			//		set_led(INDEX_BALLRA, 0);
//			//		set_led(INDEX_JOBBRA, 0);
//
//			//		Task_Enable(task_left_blink);
//			//		Task_Disable(task_right_blink);
//
//			last = now;
//			state = 5;
//		}
//
//		break;
//	case 5:
//
//		if (now - last >= wait_time) {
//
//			motor_move_to_kmh(80);
//
//			//		set_led(INDEX_BALLRA, 0);
//			//		set_led(INDEX_JOBBRA, 0);
//
//			//		Task_Enable(task_left_blink);
//			//		Task_Disable(task_right_blink);
//
//			last = now;
//			state = 0;
//		}
//		break;
//
//	}
//
//	/*
//	 motor_rotate(1,200);
//	 set_vaku_led(1);
//
//
//	 motor_rotate(-1, 200);
//	 set_vaku_led(0);
//	 */
//}

void task_Big_Motor() {

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	switch (sTime.Hours) {
	case 0:

		set_led(UZEMANYAGSZINT, 0);
		set_led(DOBFEK, 0);
		set_led(HATSO_ABLAK_FUTES, 0);
		set_led( HATSO_KOD, 0);
		set_led(ELSO_KOD, 0);
		set_led(LEGZSAK_HIBA, 0);
		set_led(HUTOVIZ, 0);
		set_led(POTTY, 0);
		set_led(LEGZSAK_KI, 0);
		set_led(BIZTIOV, 0);
		set_led(IZZITO, 0);
		set_led(NYITOTT_AJTO, 0);
		set_led(ABS, 0);
		set_led(KEZIFEK, 0);

		break;

	case 1:

		set_led(
		UZEMANYAGSZINT, 100);

		motor_move_to(22);

		break;

	case 2:

		set_led(
		DOBFEK, 100);

		motor_move_to(40);

		break;

	case 3:

		set_led(
		HATSO_ABLAK_FUTES, 100);

		motor_move_to(58);
		break;

	case 4:

		set_led(
		HATSO_KOD, 100);
		motor_move_to(75);

		break;

	case 5:

		set_led(
		ELSO_KOD, 100);
		motor_move_to(92);
		break;

	case 6:
		set_led(
		LEGZSAK_HIBA, 100);
		motor_move_to(108);
		break;

	case 7:
		set_led(
		HUTOVIZ, 100);
		motor_move_to(125);
		break;

	case 8:
		set_led(
		POTTY, 100);
		motor_move_to(140);
		break;

	case 9:
		set_led(
		LEGZSAK_KI, 100);
		motor_move_to(158);
		break;

	case 10:
		set_led(
		BIZTIOV, 100);
		motor_move_to(175);
		break;

	case 11:
		set_led(
		IZZITO, 100);
		motor_move_to(191);
		break;

	case 12:
		set_led(
		NYITOTT_AJTO, 100);
		motor_move_to(209);
		break;

	case 13:
		set_led(
		ABS, 100);
		motor_move_to(225);
		break;

	case 14:
		set_led(KEZIFEK, 0);
		motor_move_to(235);
		break;

	default:
		break;
	}

}

void task_smotor_left() {

	static uint8_t state = 0;
	static uint32_t last = 0;
	uint32_t now = HAL_GetTick();
	static int wait_time = 2000;

	switch (state) {
	case 0:

		small_motor_set_dac(left_motor.l1_dac_addr, 0);
		small_motor_set_dac(left_motor.l2_dac_addr, 4095);
		small_motor_set_dac(left_motor.l3_dac_addr, 1100);

		set_led(UZEMANYAGSZINT, 0);
		set_led(DOBFEK, 0);
		set_led(HATSO_ABLAK_FUTES, 0);

		last = now;
		state = 1;
		break;

	case 1: //refill végállás
		if (now - last >= wait_time) {

			small_motor_set_dac(left_motor.l1_dac_addr, 0);
			small_motor_set_dac(left_motor.l2_dac_addr, 4095);
			small_motor_set_dac(left_motor.l3_dac_addr, 1100);

			set_led(UZEMANYAGSZINT, 100);
			set_led(DOBFEK, 0);
			set_led(HATSO_ABLAK_FUTES, 0);

			last = now;
			state = 2;
		}
		break;

	case 2: // középső állás 1/2
		if (now - last >= wait_time) {

			small_motor_set_dac(left_motor.l1_dac_addr, 0);
			small_motor_set_dac(left_motor.l2_dac_addr, 500);
			small_motor_set_dac(left_motor.l3_dac_addr, 4095);

			set_led(UZEMANYAGSZINT, 0);
			set_led(DOBFEK, 100);
			set_led(HATSO_ABLAK_FUTES, 0);

			last = now;
			state = 3;
		}
		break;

	case 3: //full végállás
		if (now - last >= wait_time) {

			small_motor_set_dac(left_motor.l1_dac_addr, 2300);
			small_motor_set_dac(left_motor.l2_dac_addr, 0);
			small_motor_set_dac(left_motor.l3_dac_addr, 4095);

			set_led(UZEMANYAGSZINT, 0);
			set_led(DOBFEK, 0);
			set_led(HATSO_ABLAK_FUTES, 100);

			last = now;
			state = 1;
		}

		break;

	}

}

void task_smotor_right() {

	static uint8_t state = 0;
	static uint32_t last = 0;
	uint32_t now = HAL_GetTick();
	static int wait_time = 1000;

	switch (state) {
	case 0:

		small_motor_set_dac(right_motor.l1_dac_addr, 0);
		small_motor_set_dac(right_motor.l2_dac_addr, 4095);
		small_motor_set_dac(right_motor.l3_dac_addr, 2200);

		set_led_by_name("HUTOVIZ", 0);
		set_led_by_name("POTTY", 0);
		set_led_by_name("LEGZSAK_KI", 0);

		last = now;
		state = 1;
		break;

	case 1: //refill végállás
		if (now - last >= wait_time) {

			small_motor_set_dac(right_motor.l1_dac_addr, 0);
			small_motor_set_dac(right_motor.l2_dac_addr, 4095);
			small_motor_set_dac(right_motor.l3_dac_addr, 2200);

			set_led_by_name("HUTOVIZ", 100);
			set_led_by_name("POTTY", 0);
			set_led_by_name("LEGZSAK_KI", 0);

			last = now;
			state = 2;
		}
		break;

	case 2: // középső állás
		if (now - last >= wait_time) {

			small_motor_set_dac(right_motor.l1_dac_addr, 0);
			small_motor_set_dac(right_motor.l2_dac_addr, 0);
			small_motor_set_dac(right_motor.l3_dac_addr, 0);

			set_led_by_name("HUTOVIZ", 0);
			set_led_by_name("POTTY", 100);
			set_led_by_name("LEGZSAK_KI", 0);

			last = now;
			state = 3;
		}
		break;

	case 3: //full végállás
		if (now - last >= wait_time) {

			small_motor_set_dac(right_motor.l1_dac_addr, 0);
			small_motor_set_dac(right_motor.l2_dac_addr, 0);
			small_motor_set_dac(right_motor.l3_dac_addr, 0);

			set_led_by_name("HUTOVIZ", 0);
			set_led_by_name("POTTY", 0);
			set_led_by_name("LEGZSAK_KI", 100);

			last = now;
			state = 1;
		}

		break;

	}

}

void task_left_blink() {

	static int wait_time = 333;

	static uint8_t state = 0;
	static uint32_t last = 0;
	uint32_t now = HAL_GetTick();

	switch (state) {

	case 0: //not used

		set_led(INDEX_BALLRA, 0);

		last = now;
		state = 1;
		break;

	case 1:
		if (now - last >= wait_time) {

			set_led(INDEX_BALLRA, 100);
			last = now;
			state = 2;
		}
		break;

	case 2:

		if (now - last >= wait_time) {

			set_led(INDEX_BALLRA, 0);

			last = now;
			state = 1;
		}
		break;
	}

	/*
	 index_blink("bal", 20);
	 HAL_Delay(1000);
	 index_blink("bal", 0);

	 HAL_Delay(1000);
	 */
}

void task_right_blink() {

	static int wait_time = 333;

	static uint8_t state = 0;
	static uint32_t last = 0;
	uint32_t now = HAL_GetTick();

	switch (state) {

	case 0: //not used

		set_led(INDEX_JOBBRA, 0);

		last = now;
		state = 1;
		break;

	case 1:
		if (now - last >= wait_time) {

			set_led(INDEX_JOBBRA, 100);

			last = now;
			state = 2;
		}
		break;

	case 2:

		if (now - last >= wait_time) {

			set_led(INDEX_JOBBRA, 0);

			last = now;
			state = 1;
		}
		break;
	}

	/*
	 index_blink("jobb", 0);
	 HAL_Delay(1000);
	 index_blink("jobb", 20);
	 HAL_Delay(1000);
	 */

}

void task_adc_evaluation() {

	for (int i = 0; i < ADC_BUFFER_LENGTH; i++) {

		my_adcs.my_adc_channels[i].raw = adc_buffer[i];
		my_adcs.my_adc_channels[i].normalized = get_moving_average(
				get_moving_hysteresis(my_adcs.my_adc_channels[i].raw, i), i);
	}

	// Futóátlag számítása
	uint16_t boost_avg = my_adcs.my_adc_channels[0].normalized;
	uint16_t usb_avg = my_adcs.my_adc_channels[1].normalized;
	//uint16_t vbatt_avg = my_adcs.my_adc_channels[2].normalized;
	uint16_t light_avg = my_adcs.my_adc_channels[3].normalized;

	// fényérték, 255ig skálázva
	global_offset = (((light_avg * 255) / 4095));

	if (((boost_avg > BOOST_ADC_LIMIT_MIN) && (boost_avg < BOOST_ADC_LIMIT_MAX))) {

		set_led(AKSI, 0);
	} else {
		set_led(AKSI, 100);

	}

	if (((usb_avg > USB_ADC_LIMIT_MIN) && (usb_avg < USB_ADC_LIMIT_MAX))) {

		set_led(ABLAKOMOSO_SZINT, 0);
	} else {
		set_led(ABLAKOMOSO_SZINT, 100);
	}
	/*
	 if (((vbatt_avg > BATTERY_ADC_LIMIT_MIN)
	 && (vbatt_avg < BATTERY_ADC_LIMIT_MAX))) {

	 set_led(LEGZSAK_HIBA, 0);
	 } else {
	 set_led(LEGZSAK_HIBA, 100);
	 */
}

void Task_Init(void) {
	for (int i = 0; i < MAX_TASKS; i++) {
		tasks[i].taskFunc = NULL;
		tasks[i].enabled = 0;
		tasks[i].period = 0;
		tasks[i].lastRun = 0;
	}

	if (small_motor_dac_init() != HAL_OK) {

		Error_Handler();
	}

	if (Task_Add(task_Big_Motor, 100) != HAL_OK) {
		write_bit(&My_Error_code, 19, 1);
		Error_Handler();
	}

	if (Task_Add(task_smotor_left, 100) != HAL_OK) {
		write_bit(&My_Error_code, 20, 1);
		Error_Handler();
	}

	if (Task_Add(task_smotor_right, 100) != HAL_OK) {
		write_bit(&My_Error_code, 21, 1);
		Error_Handler();
	}

	if (Task_Add(task_left_blink, 100) != HAL_OK) {
		write_bit(&My_Error_code, 22, 1);
		Error_Handler();
	}

	if (Task_Add(task_right_blink, 100) != HAL_OK) {
		write_bit(&My_Error_code, 23, 1);
		Error_Handler();

	}

	if (Task_Add(task_adc_evaluation, 10) != HAL_OK) {
		write_bit(&My_Error_code, 25, 1);
		Error_Handler();
	}
}

int Task_Add(void (*func)(void), uint32_t period) {

	for (int i = 0; i < MAX_TASKS; i++) {
		if (tasks[i].taskFunc == NULL) {
			tasks[i].taskFunc = func;
			tasks[i].period = period;
			tasks[i].lastRun = HAL_GetTick();
			tasks[i].enabled = 0;
			return HAL_OK; // visszaadjuk a task ID-t
		}
	}
	return HAL_ERROR; // nincs hely
}

void Task_Enable(void (*func)(void)) {

	for (int i = 0; i < MAX_TASKS; i++) {
		if (tasks[i].taskFunc == func) {
			tasks[i].enabled = 1;
			tasks[i].lastRun = HAL_GetTick();
		}
	}

	/*
	 if (id >= 0 && id < MAX_TASKS) {
	 tasks[id].enabled = 1;
	 tasks[id].lastRun = HAL_GetTick();
	 }
	 */
}

void Task_Disable(void (*func)(void)) {

	for (int i = 0; i < MAX_TASKS; i++) {
		if (tasks[i].taskFunc == func) {
			tasks[i].enabled = 0;
			tasks[i].lastRun = HAL_GetTick();
		}
	}

	/*
	 if (id >= 0 && id < MAX_TASKS) {
	 tasks[id].enabled = 0;
	 }
	 */
}

void Task_Dispatch(void) {
	uint32_t now = HAL_GetTick();
	for (int i = 0; i < MAX_TASKS; i++) {
		if (tasks[i].enabled && tasks[i].taskFunc != NULL) {
			if ((now - tasks[i].lastRun) >= tasks[i].period) {
				tasks[i].lastRun = now;
				tasks[i].taskFunc();
			}
		}
	}
}
