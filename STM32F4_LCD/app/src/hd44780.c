/**
 * @file: 	hd44780.c
 * @brief:
 * @date: 	9 kwi 2014
 * @author: Michal Ksiezopolski
 *
 *
 * @verbatim
 * Copyright (c) 2014 Michal Ksiezopolski.
 * All rights reserved. This program and the
 * accompanying materials are made available
 * under the terms of the GNU Public License
 * v3.0 which accompanies this distribution,
 * and is available at
 * http://www.gnu.org/licenses/gpl.html
 * @endverbatim
 */

#include <hd44780.h>
#include <timers.h>
#include <fifo.h>
#include <stdio.h>
#include <hd44780_hal.h>

static void LCD_SendData(uint8_t data);
static void LCD_SendCommand(uint8_t command);
static uint8_t LCD_ReadFlag(void);


#define LCD_BUF_LEN 256  	///< LCD buffer length
#define LCD_DATA	  0x80	///< LCD data ID
#define LCD_COMMAND	0x40	///< LCD command ID

static uint8_t lcdBuffer[LCD_BUF_LEN]; 	///< Buffer for LCD commands and data
static FIFO_TypeDef lcdFifo;			///< FIFO for LCD data

/**
 * @brief Update the LCD.
 *
 * @details This function should be used in the main program loop
 * to send data and commands to the LCD. If the LCD is
 * busy the simply function returns and tries to send
 * the data later.
 */
void LCD_Update(void) {

	// If the LCD is still busy - do nothing in current run
	if (LCD_ReadFlag()  & LCD_BUSY_FLAG)
		return;

	// If the LCD FIFO is empty
	if (FIFO_IsEmpty(&lcdFifo))
		return;

	// First byte identifies whether we're dealing with data
	// or a command
	uint8_t dataOrCommand;
	FIFO_Pop(&lcdFifo, &dataOrCommand);

	// Second byte is the thing to be sent
	uint8_t toSend;
	FIFO_Pop(&lcdFifo, &toSend);

	switch (dataOrCommand) {

	// Send data
	case LCD_DATA:
		LCD_SendData(toSend);
		break;

	// Send a command
	case LCD_COMMAND:
		LCD_SendCommand(toSend);
		break;

	default:
		printf("LCD error: Neither data nor command!");
	}
}
/**
 * @brief Initialize the display.
 * @warning This is a blocking function (can last about 60ms) - only called once though
 */
void LCD_Init(void) {

	// Wait 50 ms for voltage to settle.
	TIMER_Delay(50);
	LCD_HAL_Init();


	//initialization in 4-bit interface (as per datasheet)
	LCD_Write(0b0011);
	TIMER_Delay(5);

	LCD_Write(0b0011);
	TIMER_Delay(1);

	LCD_Write(0b0011);
	TIMER_Delay(1);

	LCD_Write(0b0010);
	TIMER_Delay(1);

	// Initialize the LCD FIFO
	lcdFifo.buf = lcdBuffer;
	lcdFifo.len = LCD_BUF_LEN;

	FIFO_Add(&lcdFifo);

	// 2 row display
	LCD_SendCommand(LCD_FUNCTION|LCD_2_ROWS);
	// Wait until LCD is ready
	while (LCD_ReadFlag()  & LCD_BUSY_FLAG);
	// Turn on display, cursor and blinking
	LCD_SendCommand(LCD_DISPLAY_ON_OFF|LCD_DISPLAY_ON|LCD_CURSOR_ON|LCD_BLINK_ON);
	// Wait until LCD is ready
	while (LCD_ReadFlag()  & LCD_BUSY_FLAG);
	// Clear the display
	LCD_SendCommand(LCD_CLEAR_DISPLAY);
	// Wait until LCD is ready
	while (LCD_ReadFlag()  & LCD_BUSY_FLAG);
}
/**
 * @brief Clear the display.
 * @details Write space code 20H to all DDRAM addresses.
 * Sets DDRAM address to 0. Removes all shifts.
 * Sets I/D to 1 (increment mode) in entry mode.
 */
void LCD_Clear(void) {

	FIFO_Push(&lcdFifo, LCD_COMMAND);
	FIFO_Push(&lcdFifo, LCD_CLEAR_DISPLAY);
}
/**
 * @brief Go to the beginning of the display.
 * @details Sets DDRAM address to 0. Removes all shifts.
 */
void LCD_Home(void) {

	FIFO_Push(&lcdFifo, LCD_COMMAND);
	FIFO_Push(&lcdFifo, LCD_HOME);
}

/**
 * @brief Position the LCD at a given memory location.
 * @param positionX Column of LCD
 * @param positionY Row of LCD - should be 0 (upper row) or 1 (lower row)
 */
void LCD_Position(uint8_t positionX, uint8_t positionY) {

	uint8_t new_pos;

	switch (positionY) {

	case 0:
		new_pos = LCD_ROW1;
		break;
	case 1:
		new_pos = LCD_ROW2;
		break;
	default:
		printf ("LCD Error: Wrong row!!!");
		return;
	}

	new_pos += positionX;
	FIFO_Push(&lcdFifo, LCD_COMMAND);
	FIFO_Push(&lcdFifo, LCD_SET_DDRAM | (new_pos & 0x7f));
}
/**
 * @brief Shifts the display in the specified direction.
 * @param shift Amount of chars to shift by.
 * @param dir Direction of shift: 0 - left, 1 - right
 */
void LCD_ShifDisplay(uint8_t shift, uint8_t dir) {

	switch (dir) {

	case 0:
		break;

	case 1:
		dir = LCD_SHIFT_RIGHT;
		break;

	default:
		printf ("LCD Error: Wrong direction!!!");
		return;
	}

	uint8_t i;
	for (i = 0; i < shift; i++) {
		FIFO_Push(&lcdFifo, LCD_COMMAND);
		FIFO_Push(&lcdFifo, LCD_CURSOR_SHIFT | LCD_SHIFT_DISPLAY | dir);
	}

}
/**
 *
 * @param onOff
 * @param blink
 */
void LCD_SetCursor(uint8_t onOff, uint8_t blink) {

	switch (onOff) {

	case 0:
		break;

	case 1:
		onOff = LCD_CURSOR_ON;
		break;

	default:
		printf ("LCD Error: Wrong parameter in LCD_SetCursor!!!");
		return;
	}

	switch (blink) {

	case 0:
		break;

	case 1:
		blink = LCD_BLINK_ON;
		break;

	default:
		printf ("LCD Error: Wrong parameter in LCD_SetCursor!!!");
		return;
	}

	FIFO_Push(&lcdFifo, LCD_COMMAND);
	FIFO_Push(&lcdFifo, LCD_DISPLAY_ON_OFF | LCD_DISPLAY_ON | blink | onOff);

}
/**
 * @brief Print a character.
 * @param c Character to print.
 */
void LCD_Putc(uint8_t c) {

	FIFO_Push(&lcdFifo, LCD_DATA);
	FIFO_Push(&lcdFifo, c);
}
/**
 * @brief Print a string ended with '\0'.
 * @param s String
 */
void LCD_Puts(char* s) {

	uint8_t i=0;
	while (s[i]!='\0') {
		LCD_Putc((uint8_t)s[i++]);
	}
}
/**
 * @brief Send data to LCD.
 * @param data Data to send.
 */
static void LCD_SendData(uint8_t data) {

	// rs high and rw low for writing data
  LCD_HAL_HighRS();
	LCD_HAL_LowRW();

	LCD_DataOut();

	// write higher 4 bits first
	LCD_HAL_HighE();
	LCD_Write(data>>4);
	LCD_HAL_LowE();

	LCD_HAL_HighE();
	LCD_Write(data);
	LCD_HAL_LowE();

}

/**
 * @brief Send a command to the LCD.
 * @param command Command to send.
 */
static void LCD_SendCommand(uint8_t command) {
	// rs low and rw low for writing command
  LCD_HAL_LowRW();
  LCD_HAL_LowRS();

	LCD_DataOut();

	// write higher 4 bits first
	LCD_HAL_HighE();
	LCD_Write(command>>4);
	LCD_HAL_LowE();

	LCD_HAL_HighE();
	LCD_Write(command);
	LCD_HAL_LowE();
}
/**
 * @brief Read busy flag.
 * @return
 */
static uint8_t LCD_ReadFlag(void) {
	LCD_DataIn();
	LCD_HAL_LowRS();
	LCD_HAL_HighRW();

	uint8_t result=0;
	result=(LCD_Read()<<4);
	result|=LCD_Read();
	return result;

}


