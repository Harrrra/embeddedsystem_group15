#include "main.h"
#include "keypad.h"

TIM_HandleTypeDef htim2;

volatile char keypad_last_key;
volatile KeypadEventType keypad_last_event;
volatile uint32_t keypad_event_count;

static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM2_Init();

    if (Keypad_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }

    while (1)
    {
        KeypadEvent event;

        while (Keypad_GetEvent(&event))
        {
            keypad_last_key = event.key;
            keypad_last_event = event.type;
            keypad_event_count++;
        }
    }
}

static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef oscConfig = {0};
    RCC_ClkInitTypeDef clockConfig = {0};

    oscConfig.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    oscConfig.HSEState = RCC_HSE_ON;
    oscConfig.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    oscConfig.HSIState = RCC_HSI_ON;
    oscConfig.PLL.PLLState = RCC_PLL_ON;
    oscConfig.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    oscConfig.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&oscConfig) != HAL_OK)
    {
        Error_Handler();
    }

    clockConfig.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                            RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clockConfig.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clockConfig.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clockConfig.APB1CLKDivider = RCC_HCLK_DIV2;
    clockConfig.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&clockConfig, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef gpioConfig = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    HAL_GPIO_WritePin(KP_C1_GPIO_Port, KP_C1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, KP_C2_Pin | KP_C3_Pin | KP_C4_Pin, GPIO_PIN_SET);

    gpioConfig.Pin = KP_C1_Pin;
    gpioConfig.Mode = GPIO_MODE_OUTPUT_PP;
    gpioConfig.Pull = GPIO_NOPULL;
    gpioConfig.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(KP_C1_GPIO_Port, &gpioConfig);

    gpioConfig.Pin = KP_C2_Pin | KP_C3_Pin | KP_C4_Pin;
    HAL_GPIO_Init(GPIOB, &gpioConfig);

    gpioConfig.Pin = KP_R1_Pin | KP_R2_Pin | KP_R3_Pin | KP_R4_Pin;
    gpioConfig.Mode = GPIO_MODE_INPUT;
    gpioConfig.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &gpioConfig);
}

static void MX_TIM2_Init(void)
{
    TIM_ClockConfigTypeDef clockSource = {0};

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 7199;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 99;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }

    clockSource.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim2, &clockSource) != HAL_OK)
    {
        Error_Handler();
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *timer)
{
    Keypad_OnTimerElapsed(timer);
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}
