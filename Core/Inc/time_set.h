#ifndef TIME_SET_H
#define TIME_SET_H

#include <stdint.h>
#include "button.h"

#ifdef __cplusplus
extern "C" {
#endif

// Button IDs
#define TS_BTN_HOUR    0
#define TS_BTN_MIN     1

// Global vars
extern volatile uint8_t time_minute;   // 0–59
extern volatile uint8_t my_hour;       // 0–23

// Initialization
void TimeSet_Init(Button_t *hourBtn, Button_t *minBtn);

// Tick (call every 1 ms)
void TimeSet_Tick(void);

// Check if mode active
uint8_t TimeSet_IsActive(void);

// Force exit
void TimeSet_ForceExit(void);

void TimeSet_InternalTick(void);

#ifdef __cplusplus
}
#endif

#endif
