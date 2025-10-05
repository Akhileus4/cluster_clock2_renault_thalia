#include "tasks.h"
#include "main.h"



#define MAX_TASKS 1000
Task_t tasks[MAX_TASKS];

void Task_Init(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].taskFunc = NULL;
        tasks[i].enabled  = 0;
        tasks[i].period   = 0;
        tasks[i].lastRun  = 0;
    }
}

int Task_Add(void (*func)(void), uint32_t period) {


    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].taskFunc == NULL) {
            tasks[i].taskFunc = func;
            tasks[i].period   = period;
            tasks[i].lastRun  = HAL_GetTick();
            tasks[i].enabled  = 1;
            return HAL_OK; // visszaadjuk a task ID-t
        }
    }
    return HAL_ERROR; // nincs hely
}

void Task_Enable(void (*func)(void)) {


	for (int i = 0; i < MAX_TASKS; i++) {
		if (tasks[i].taskFunc == func) {
			tasks[i].enabled  = 1;
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
			tasks[i].enabled  = 0;
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
