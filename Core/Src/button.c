#include "button.h"

void Button_Init(Button_t *btn, GPIO_TypeDef *PORT, uint16_t PIN) {
	btn->PORT = PORT;
	btn->PIN = PIN;
	btn->lastState = 0;
	btn->currentState = 0;
	btn->pressTime = 0;
	btn->longReported = 0;
}

ButtonEvent_t Button_Update(Button_t *btn) {
	btn->currentState =
			(HAL_GPIO_ReadPin(btn->PORT, btn->PIN) == GPIO_PIN_RESET);

	// New press
	if (btn->currentState && !btn->lastState) {
		btn->pressTime = 0;
		btn->longReported = 0;
		btn->lastState = 1;
		return BTN_SHORT_PRESSED;
	}

	// Held
	if (btn->currentState && btn->lastState) {
		btn->pressTime++;

		if (btn->pressTime >= LONG_PRESS_TIME_MS && !btn->longReported) {
			btn->longReported = 1;
			return BTN_LONG_PRESSED;
		}

		return BTN_NO_EVENT;
	}

	// Released
	if (!btn->currentState && btn->lastState) {
		btn->lastState = 0;

		if (btn->pressTime >= LONG_PRESS_TIME_MS)
			return BTN_LONG_RELEASED;
		else
			return BTN_SHORT_RELEASED;
	}

	return BTN_NO_EVENT;
}
