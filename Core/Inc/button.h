#ifndef BUTTON_H
#define BUTTON_H

#include "stm32g0xx_hal.h"
#include <stdint.h>

typedef enum {
	BTN_NO_EVENT = 0,
	BTN_SHORT_PRESSED,
	BTN_SHORT_RELEASED,
	BTN_LONG_PRESSED,
	BTN_LONG_RELEASED
} ButtonEvent_t;

typedef struct {
	GPIO_TypeDef *PORT;
	uint16_t PIN;

	uint8_t lastState;
	uint8_t currentState;

	uint32_t pressTime;
	uint8_t longReported;
} Button_t;

#define LONG_PRESS_TIME_MS   5000   // 5 seconds

extern Button_t btnHour;
extern Button_t btnMin;

void Button_Init(Button_t *btn, GPIO_TypeDef *PORT, uint16_t PIN);
ButtonEvent_t Button_Update(Button_t *btn);

#endif
