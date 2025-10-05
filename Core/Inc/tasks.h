/*
 * tasks.h
 *
 *  Created on: Sep 19, 2025
 *      Author: admin
 */

#ifndef INC_TASKS_H_
#define INC_TASKS_H_

#include "main.h"

typedef struct {
    void (*taskFunc)(void);   // futtatandó függvény pointer
    uint32_t period;          // periódus [ms]
    uint32_t lastRun;         // utolsó futtatás ideje
    uint8_t enabled;          // engedélyezve van-e
} Task_t;


void Task_Init(void) ;
int Task_Add(void (*func)(void), uint32_t period) ;
void Task_Enable(void (*func)(void)) ;
void Task_Disable(void (*func)(void));
void Task_Dispatch(void);



//tasks to be define here
void task_Big_Motor();
void task_left_blink();
void task_right_blink();
void task_display_led_update();
void task_smotor_right();
void task_smotor_left();

/*task definiciós template
 *
 * void someTask(void) {
 *
    static uint8_t state = 0;
    static uint32_t last = 0;
    uint32_t now = HAL_GetTick();
    static int wait_time  = 100;

    switch (state) {
    case 0:
        // Első akció
        do_something();
        last = now;
        state = 1;
        break;

    case 1:
        if (now - last >= wait_time) {
            // Második akció
            do_something_else();

            last = now;
            state = 2;
        }
        break;

    case 2:
        if (now - last >= WAIT_TIME_2) {
            // Harmadik akció vagy vissza a kezdéshez
            do_third_action();

            last = now;
            state = 0; // vagy folytathatod 3,4,5 állapotokkal

        }
        break;
    }
}
 *
 *
 *
 *
 *
 *
 */



#endif /* INC_TASKS_H_ */
