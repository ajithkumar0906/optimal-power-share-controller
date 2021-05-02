#include "oled.h"


// Screenbuffer
static uint8_t OLED_Buffer[OLED_WIDTH * OLED_HEIGHT / 8];

// Screen object
static OLED_t OLED;


//
//  Send a byte to the command register
//
static void OLED_WriteCommand(uint8_t command)
{
	HAL_I2C_Mem_Write(&OLED_I2C_PORT, OLED_I2C_ADDR,0x00,1,&command,1,10);
}


//
//	Initialize the oled screen
//
uint8_t OLED_Init(void)
{	
	/* Init LCD */
	OLED_WriteCommand(0xAE); //display off
	OLED_WriteCommand(0x20); //Set Memory Addressing Mode   
	OLED_WriteCommand(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	OLED_WriteCommand(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	OLED_WriteCommand(0xC8); //Set COM Output Scan Direction
	OLED_WriteCommand(0x00); //---set low column address
	OLED_WriteCommand(0x10); //---set high column address
	OLED_WriteCommand(0x40); //--set start line address
	OLED_WriteCommand(0x81); //--set contrast control register
	OLED_WriteCommand(0xFF);
	OLED_WriteCommand(0xA1); //--set segment re-map 0 to 127
	OLED_WriteCommand(0xA6); //--set normal display
	OLED_WriteCommand(0xA8); //--set multiplex ratio(1 to 64)
	OLED_WriteCommand(0x3F); //
	OLED_WriteCommand(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	OLED_WriteCommand(0xD3); //-set display offset
	OLED_WriteCommand(0x00); //-not offset
	OLED_WriteCommand(0xD5); //--set display clock divide ratio/oscillator frequency
	OLED_WriteCommand(0xF0); //--set divide ratio
	OLED_WriteCommand(0xD9); //--set pre-charge period
	OLED_WriteCommand(0x22); //
	OLED_WriteCommand(0xDA); //--set com pins hardware configuration
	OLED_WriteCommand(0x12);
	OLED_WriteCommand(0xDB); //--set vcomh
	OLED_WriteCommand(0x20); //0x20,0.77xVcc
	OLED_WriteCommand(0x8D); //--set DC-DC enable
	OLED_WriteCommand(0x14); //
	OLED_WriteCommand(0xAF); //--turn on OLED panel
	
	// Clear screen
	OLED_Fill(Black);
	
	// Flush buffer to screen
	OLED_UpdateScreen();
	
	// Set default values for screen object
	OLED.CurrentX = 0;
	OLED.CurrentY = 0;
	
	OLED.Initialized = 1;
	
	return 1;
}

//
//  Fill the whole screen with the given color
//
void OLED_Fill(OLED_COLOR color) 
{
	/* Set memory */
	uint32_t i;

	for(i = 0; i < sizeof(OLED_Buffer); i++)
	{
		OLED_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
	}
}

//
//  Write the screenbuffer with changed to the screen
//
void OLED_UpdateScreen(void) 
{
	uint8_t i;
	
	for (i = 0; i < 8; i++) {
		OLED_WriteCommand(0xB0 + i);
		OLED_WriteCommand(0x00);
		OLED_WriteCommand(0x10);

		HAL_I2C_Mem_Write(&OLED_I2C_PORT, OLED_I2C_ADDR, 0x40, 1, &OLED_Buffer[OLED_WIDTH * i], OLED_WIDTH, 100);
	}
}

//
//	Draw one pixel in the screenbuffer
//	X => X Coordinate
//	Y => Y Coordinate
//	color => Pixel color
//
void OLED_DrawPixel(uint8_t x, uint8_t y, OLED_COLOR color)
{
	if (x >= OLED_WIDTH || y >= OLED_HEIGHT) 
	{
		// Don't write outside the buffer
		return;
	}
	
	// Check if pixel should be inverted
	if (OLED.Inverted) 
	{
		color = (OLED_COLOR)!color;
	}
	
	// Draw in the right color
	if (color == White)
	{
		OLED_Buffer[x + (y / 8) * OLED_WIDTH] |= 1 << (y % 8);
	} 
	else 
	{
		OLED_Buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y % 8));
	}
}

//
//  Draw 1 char to the screen buffer
//	ch 		=> char om weg te schrijven
//	Font 	=> Font waarmee we gaan schrijven
//	color 	=> Black or White
//
char OLED_WriteChar(char ch, FontDef Font, OLED_COLOR color)
{
	uint32_t i, b, j;
	
	// Check remaining space on current line
	if (OLED_WIDTH <= (OLED.CurrentX + Font.FontWidth) ||
		OLED_HEIGHT <= (OLED.CurrentY + Font.FontHeight))
	{
		// Not enough space on current line
		return 0;
	}
	
	// Use the font to write
	for (i = 0; i < Font.FontHeight; i++)
	{
		b = Font.data[(ch - 32) * Font.FontHeight + i];
		for (j = 0; j < Font.FontWidth; j++)
		{
			if ((b << j) & 0x8000) 
			{
				OLED_DrawPixel(OLED.CurrentX + j, (OLED.CurrentY + i), (OLED_COLOR) color);
			} 
			else 
			{
				OLED_DrawPixel(OLED.CurrentX + j, (OLED.CurrentY + i), (OLED_COLOR)!color);
			}
		}
	}
	
	// The current space is now taken
	OLED.CurrentX += Font.FontWidth;
	
	// Return written char for validation
	return ch;
}

//
//  Write full string to screenbuffer
//
char OLED_WriteString(char* str, FontDef Font, OLED_COLOR color)
{
	// Write until null-byte
	while (*str) 
	{
		if (OLED_WriteChar(*str, Font, color) != *str)
		{
			// Char could not be written
			return *str;
		}
		
		// Next char
		str++;
	}
	
	// Everything ok
	return *str;
}

//
//	Position the cursor
//
void OLED_SetCursor(uint8_t x, uint8_t y) 
{
	OLED.CurrentX = x;
	OLED.CurrentY = y;
}