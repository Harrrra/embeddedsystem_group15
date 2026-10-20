#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define KP_C1_Pin GPIO_PIN_8
#define KP_C1_GPIO_Port GPIOA
#define KP_C2_Pin GPIO_PIN_13
#define KP_C2_GPIO_Port GPIOB
#define KP_C3_Pin GPIO_PIN_14
#define KP_C3_GPIO_Port GPIOB
#define KP_C4_Pin GPIO_PIN_15
#define KP_C4_GPIO_Port GPIOB

#define KP_R1_Pin GPIO_PIN_12
#define KP_R1_GPIO_Port GPIOA
#define KP_R2_Pin GPIO_PIN_11
#define KP_R2_GPIO_Port GPIOA
#define KP_R3_Pin GPIO_PIN_10
#define KP_R3_GPIO_Port GPIOA
#define KP_R4_Pin GPIO_PIN_9
#define KP_R4_GPIO_Port GPIOA

void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif
