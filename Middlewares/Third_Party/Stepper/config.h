#include "stm32f7xx_hal.h"

const uint32_t steps = 2038;

const GPIO_TypeDef* _GPIO_PORTS[] =
    {
        GPIOA, 
        GPIOC,
        GPIOC,
        GPIOF
    };

const uint16_t _GPIO_PINS[] =
    {
        GPIO_PIN_3,
        GPIO_PIN_0,
        GPIO_PIN_3,
        GPIO_PIN_3
    };
