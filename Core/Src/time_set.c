#include "time_set.h"
#include "LP5036RJVR_led_driver.h"
#include "button.h"

#define TS_LONG_PRESS_MS   5000
#define TS_TIMEOUT_MS      10000
#define TS_BLINK_MS        333

volatile uint8_t time_minute = 0;
volatile uint8_t my_hour = 0;

typedef enum {
	TS_MODE_NORMAL = 0, TS_MODE_SET_MINUTE, TS_MODE_SET_HOUR
} ts_mode_t;

static ts_mode_t mode = TS_MODE_NORMAL;

static Button_t *btnHour = 0;
static Button_t *btnMin = 0;

static uint32_t inactivityTimer = 0;
static uint32_t blinkTimer = 0;
static uint8_t blinkState = 0;

// ----------------------------
// Internal helpers
// ----------------------------

static void enter_minute_mode(void) {
	mode = TS_MODE_SET_MINUTE;
	inactivityTimer = 0;
	blinkTimer = 0;
	blinkState = 0;
	set_led(MENETFENY, 0);
}

static void enter_hour_mode(void) {
	mode = TS_MODE_SET_HOUR;
	inactivityTimer = 0;
	blinkTimer = 0;
	blinkState = 0;
	set_vaku_led(0);
}

static void exit_mode(void) {
	if (mode == TS_MODE_SET_MINUTE)
		set_led(MENETFENY, 0);

	if (mode == TS_MODE_SET_HOUR)
		set_vaku_led(0);

	mode = TS_MODE_NORMAL;
	inactivityTimer = 0;
	blinkTimer = 0;
	blinkState = 0;
}

// ----------------------------
// Event handler
// ----------------------------

static void handle_event(uint8_t id, ButtonEvent_t ev) {
	// Reset inactivity when active
	if (mode != TS_MODE_NORMAL)
		inactivityTimer = 0;

	// ENTER MODES
	if (id == TS_BTN_MIN && ev == BTN_LONG_PRESSED) {
		enter_minute_mode();
		return;
	}

	if (id == TS_BTN_HOUR && ev == BTN_LONG_PRESSED) {
		enter_hour_mode();
		return;
	}

	// Setting minute
	if (mode == TS_MODE_SET_MINUTE) {
		if (id == TS_BTN_MIN && ev == BTN_SHORT_RELEASED) {
			time_minute++;
			if (time_minute >= 60)
				time_minute = 0;
		}
		return;
	}

	// Setting hour
	if (mode == TS_MODE_SET_HOUR) {
		if (id == TS_BTN_HOUR && ev == BTN_SHORT_RELEASED) {
			my_hour++;
			if (my_hour >= 24)
				my_hour = 0;
		}
		return;
	}
}

// ----------------------------
// Public API
// ----------------------------

void TimeSet_Init(Button_t *hourBtn, Button_t *minBtn) {
	btnHour = hourBtn;
	btnMin = minBtn;
	mode = TS_MODE_NORMAL;
}

void TimeSet_Tick(void) {
	static uint32_t minutePressTimer = 0;
	static uint8_t minutePressed = 0;

	// --- GOMB ÁLLAPOT BEOLVASÁSA ---
	uint8_t min_is_pressed = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)
			== GPIO_PIN_RESET);

	// 1) Gomb Lenyomás -> azonnali LED jelez
	if (min_is_pressed && !minutePressed) {
		minutePressed = 1;
		minutePressTimer = 0;
		set_led(MENETFENY, 100);   // jelzés: hosszú nyomás indul
	}

	// 2) Ha tartod: számláljuk a hosszú nyomás időt
	if (min_is_pressed && minutePressed) {
		minutePressTimer++;

		// 5 másodperc után -> belép módba
		if (minutePressTimer >= 5000) {
			// átvált villogó módba
			enter_minute_mode();       // a TimeSet rendszer funkciója
			minutePressed = 0;         // reseteljük a logikai flag-et
			return;                    // innentől a villogó logika viszi tovább
		}
	}

	// 3) Felengedés az 5s előtt → visszajelző LED kikapcs
	if (!min_is_pressed && minutePressed) {
		minutePressed = 0;
		set_led(MENETFENY, 0);         // megszakadt a long press
	}

	// 4) Ha már beállító módban vagyunk → eredeti TimeSet logika
	if (TimeSet_IsActive()) {
		// ezt meghagyjuk az eddigi hosszú kódból:
		// - villogás
		// - minute++
		// - hour++
		// - 10s timeout
		TimeSet_InternalTick();  // (ezt mutatom lent!)
	}
}

void TimeSet_InternalTick(void) {
	// Inaktivitás számláló
	inactivityTimer++;
	if (inactivityTimer >= TS_TIMEOUT_MS) {
		exit_mode();
		return;
	}

	// Villogás
	blinkTimer++;
	if (blinkTimer >= TS_BLINK_MS) {
		blinkTimer = 0;
		blinkState = !blinkState;

		if (mode == TS_MODE_SET_MINUTE)
			set_led(MENETFENY, blinkState ? 100 : 0);

		if (mode == TS_MODE_SET_HOUR)
			set_vaku_led(blinkState ? 100 : 0);
	}
}

void TimeSet_ForceExit(void) {
	exit_mode();
}

uint8_t TimeSet_IsActive(void) {
	return (mode != TS_MODE_NORMAL);
}
