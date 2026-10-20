# Keypad 4x4 Timer Driver

This STM32F103C8T6 project contains a standalone 4x4 matrix keypad driver.
The driver scans the matrix from a TIM2 update interrupt every 10 ms, filters
contact bounce, and provides debounced pressed, held, and released events.

## Keypad wiring

| Signal | STM32 pin |
| --- | --- |
| Column C1 | PA8 |
| Column C2 | PB13 |
| Column C3 | PB14 |
| Column C4 | PB15 |
| Row R1 | PA12 |
| Row R2 | PA11 |
| Row R3 | PA10 |
| Row R4 | PA9 |

Rows use internal pull-ups. Columns are driven high when idle and selected
one at a time by driving the active column low.

## Driver behavior

- One scan is performed on each TIM2 update interrupt (10 ms).
- A key state is accepted after three consecutive matching scans (about 30 ms).
- `KEYPAD_EVENT_PRESSED` is emitted once after a stable press.
- `KEYPAD_EVENT_HELD` is emitted once after a stable press has lasted about
  800 ms.
- `KEYPAD_EVENT_RELEASED` is emitted once after a stable release.
- The scanner assumes one key is pressed at a time; simultaneous-key rollover
  and ghost-key suppression are not implemented.
- The event queue holds up to seven pending events. Check
  `Keypad_GetDroppedEventCount()` if the application might not drain it promptly.

## API

Include `keypad.h` and initialize the driver with the periodic timer:

```c
if (Keypad_Init(&htim2) != HAL_OK)
{
    Error_Handler();
}
```

Forward the timer callback to the driver:

```c
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    Keypad_OnTimerElapsed(htim);
}
```

Poll events from the main loop; do not call LCD, UART, or other blocking
application code from the timer interrupt:

```c
KeypadEvent event;
while (Keypad_GetEvent(&event))
{
    if (event.type == KEYPAD_EVENT_PRESSED)
    {
        /* Process event.key */
    }
}
```

`main.c` is a minimal integration example. It records the most recent event,
key, and event count in debugger-visible variables. The keypad driver itself
is in `Core/Src/keypad.c` and `Core/Inc/keypad.h`.

## TIM2 setup

The project clock is 72 MHz. APB1 runs at 36 MHz with the timer clock doubled
to 72 MHz. TIM2 uses prescaler 7199 and auto-reload value 99:

```text
72 MHz / (7199 + 1) / (99 + 1) = 100 Hz = 10 ms period
```

TIM2 update interrupt is enabled in NVIC. TIM3, I2C, SPI, LCD, RFID, password,
servo, and door-lock application modules are not part of this keypad-only
project.
