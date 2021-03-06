#include "stm32f7xx_hal.h"
#include "fonts.h"

/**
 * This Library is written and optimized by Olivier Van den Eede(4ilo) in 2016
 * for Stm32 Uc and HAL-i2c lib's.
 *
 * To use this library with OLED oled display you will need to customize the defines below.
 *
 * This library uses 2 extra files (fonts.c/h).
 * In this files are 3 different fonts you can use:
 * 		- Font_7x10
 * 		- Font_11x18
 * 		- Font_16x26
 *
 */

#ifndef OLED_H
#define OLED_H

// I2c port as defined in main generated by CubeMx
#define OLED_I2C_PORT		hi2c1
// I2c address
#define OLED_I2C_ADDR        0x78
// OLED width in pixels
#define OLED_WIDTH           128
// OLED LCD height in pixels
#define OLED_HEIGHT          64


//
//  Enumeration for screen colors
//
typedef enum {
	Black = 0x00, // Black color, no pixel
	White = 0x01  //Pixel is set. Color depends on LCD
} OLED_COLOR;

//
//  Struct to store transformations
//
typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} OLED_t;

//	Definition of the i2c port in main
extern I2C_HandleTypeDef OLED_I2C_PORT;

//
//  Function definitions
//
uint8_t OLED_Init(void);
void OLED_Fill(OLED_COLOR color);
void OLED_UpdateScreen(void);
void OLED_DrawPixel(uint8_t x, uint8_t y, OLED_COLOR color);
char OLED_WriteChar(char ch, FontDef Font, OLED_COLOR color);
char OLED_WriteString(char* str, FontDef Font, OLED_COLOR color);
void OLED_SetCursor(uint8_t x, uint8_t y);

#endif