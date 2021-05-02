#ifndef	_KEYPADCONFIG_H
#define	_KEYPADCONFIG_H
#include "stm32f7xx_hal.h"

#define           _KEYPAD_DEBOUNCE_TIME_MS        20
#define           _KEYPAD_USE_FREERTOS            1

const GPIO_TypeDef* _KEYPAD_ROW_GPIO_PORT[] =
{
  GPIOE,
  GPIOF,
  GPIOG,
  GPIOG
};

const uint16_t _KEYPAD_ROW_GPIO_PIN[] =
{
  GPIO_PIN_13,
  GPIO_PIN_15,
  GPIO_PIN_14,
  GPIO_PIN_9
};

const GPIO_TypeDef* _KEYPAD_COLUMN_GPIO_PORT[] =
{
  GPIOF,
  GPIOE,
  GPIOE,
  GPIOF
};

const uint16_t _KEYPAD_COLUMN_GPIO_PIN[] =
{
  GPIO_PIN_13,
  GPIO_PIN_9,
  GPIO_PIN_11,
  GPIO_PIN_14
};

#endif
