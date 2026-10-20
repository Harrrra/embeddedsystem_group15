#ifndef KEYPAD_H
#define KEYPAD_H

#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    KEYPAD_EVENT_PRESSED,
    KEYPAD_EVENT_HELD,
    KEYPAD_EVENT_RELEASED
} KeypadEventType;

typedef struct
{
    KeypadEventType type;
    char key;
} KeypadEvent;

/* Start scanning with a timer configured for a 10 ms update period. */
HAL_StatusTypeDef Keypad_Init(TIM_HandleTypeDef *timer);

/* Call from HAL_TIM_PeriodElapsedCallback(). */
void Keypad_OnTimerElapsed(TIM_HandleTypeDef *timer);

/* Get the next queued event; returns false when the queue is empty. */
bool Keypad_GetEvent(KeypadEvent *event);

uint32_t Keypad_GetDroppedEventCount(void);

#endif
