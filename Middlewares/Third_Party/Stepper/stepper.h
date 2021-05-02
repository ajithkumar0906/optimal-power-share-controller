#ifndef _STEPPER_H
#define _STEPPER_H

#include <stdint.h>
#include <stdbool.h>

void Stepper_Init(void);
void Stepper_Rotate(uint16_t angle);
void Stepper_Rotate_Reverse(uint16_t angle);

#endif