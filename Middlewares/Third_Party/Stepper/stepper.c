/**
 * A Stepper Motor Library for STM32 Devices Running with a HAL on an RTOS with a ULN2003 Driver
 * 
 * Author: Rohith Balaji
 * GitHub: https://github.com/RohithCIS
 * 
 */

#include "config.h"
#include "stepper.h"
#include "cmsis_os.h"

void driveMotor(bool, bool, bool, bool);

/**
 * Initialise the Stepper Motor
 */
void Stepper_Init(void)
{
    GPIO_InitTypeDef gpio;
    for (uint8_t i = 0; i < 4; i++)
    {
        gpio.Mode = GPIO_MODE_OUTPUT_PP;
        gpio.Speed = GPIO_SPEED_FREQ_LOW;
        gpio.Pin = _GPIO_PINS[i];
        HAL_GPIO_Init((GPIO_TypeDef *)_GPIO_PORTS[i], &gpio);
    }
}

/**
 * Rotate the Stepper Motor Clock-Wise by a given angle
 * 
 * @param angle Angle by which the Stepper is Rotated
 */
void Stepper_Rotate(uint16_t angle)
{
    for (uint32_t cycle = 0; cycle < angle * 512 / 360; cycle++)
    {
        driveMotor(true, false, false, false);
        osDelay(5);
        driveMotor(false, true, false, false);
        osDelay(5);
        driveMotor(false, false, true, false);
        osDelay(5);
        driveMotor(false, false, false, true);
        osDelay(5);
    }
    driveMotor(false, false, false, false);
}

/**
 * Rotate the Stepper Motor Counter-Clock-Wise by a given angle
 * 
 * @param angle Angle by which the Stepper is Rotated
 */
void Stepper_Rotate_Reverse(uint16_t angle)
{
    for (uint32_t cycle = 0; cycle < angle * 512 / 360; cycle++)
    {
        driveMotor(false, false, false, true);
        osDelay(5);
        driveMotor(false, false, true, false);
        osDelay(5);
        driveMotor(false, true, false, false);
        osDelay(5);
        driveMotor(true, false, false, false);
        osDelay(5);
    }
    driveMotor(false, false, false, false);
}

/**
 * Drives the Motor Pins via ULN2003 IC.
 * 
 * @param one Drives the Blue Wire of the Stepper
 * @param two Drives the Pink Wire of the Stepper
 * @param three Drives the Yellow Wire of the Stepper
 * @param four Drives the Orange Wire of the Stepper
 * 
 */
void driveMotor(bool one, bool two, bool three, bool four)
{
    one ? HAL_GPIO_WritePin((GPIO_TypeDef *)_GPIO_PORTS[0], _GPIO_PINS[0], GPIO_PIN_SET) : HAL_GPIO_WritePin((GPIO_TypeDef *)_GPIO_PORTS[0], _GPIO_PINS[0], GPIO_PIN_RESET);
    two ? HAL_GPIO_WritePin((GPIO_TypeDef *)_GPIO_PORTS[1], _GPIO_PINS[1], GPIO_PIN_SET) : HAL_GPIO_WritePin((GPIO_TypeDef *)_GPIO_PORTS[1], _GPIO_PINS[1], GPIO_PIN_RESET);
    three ? HAL_GPIO_WritePin((GPIO_TypeDef *)_GPIO_PORTS[2], _GPIO_PINS[2], GPIO_PIN_SET) : HAL_GPIO_WritePin((GPIO_TypeDef *)_GPIO_PORTS[2], _GPIO_PINS[2], GPIO_PIN_RESET);
    four ? HAL_GPIO_WritePin((GPIO_TypeDef *)_GPIO_PORTS[3], _GPIO_PINS[3], GPIO_PIN_SET) : HAL_GPIO_WritePin((GPIO_TypeDef *)_GPIO_PORTS[3], _GPIO_PINS[3], GPIO_PIN_RESET);
}