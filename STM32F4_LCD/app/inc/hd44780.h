/**
 * @file: 	hd44780.h
 * @brief:  HD44780 LCD library
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

#ifndef HD44780_H_
#define HD44780_H_

#include <inttypes.h>

/*
 * LCD commands HD44780 (as per datasheet)
 */
#define LCD_CLEAR_DISPLAY 		0x01 ///< Clear the display and set DDRAM address to 0
#define LCD_HOME 				0x02 ///< Set DDRAM to 0, cancels any shifts
#define LCD_ENTRY_MODE 			0x04 ///< Sets cursor move direction and specifies display shift
	#define LCD_INCREMENT 		0x02 ///< Increments (1) or decrements (0) DDRAM address by 1 when data is written or read from DDRAM @see LCD_ENTRY_MODE
	#define LCD_SHIFT_LEFT 		0x01 ///< Shifts the entire display (1) in direction specified by ID bit when writing to DDRAM - display moves, not cursor. @see LCD_ENTRY_MODE
#define LCD_DISPLAY_ON_OFF 		0x08 ///< Sets display on/off, cursor on/off and blinking of cursor
	#define LCD_DISPLAY_ON		0x04 ///< Display on/off bit @see LCD_DISPLAY_ON_OFF
	#define LCD_CURSOR_ON 		0x02 ///< Cursor on/off bit @see LCD_DISPLAY_ON_OFF
	#define LCD_BLINK_ON 		0x01 ///< Blink on/off bit @see LCD_DISPLAY_ON_OFF
#define LCD_CURSOR_SHIFT 		0x10 ///< Moves cursor and shift display without changing DDRAM contents
	#define LCD_SHIFT_RIGHT 	0x04 ///< Shift right bit (1), shift left (0) @see LCD_CURSOR_SHIFT
	#define LCD_SHIFT_DISPLAY 	0x08 ///< Display shift (1), cursor shift (0) @see LCD_CURSOR_SHIFT
#define LCD_FUNCTION			0x20 ///< Sets interface data length, number of display lines and character font
	#define LCD_2_ROWS	 		0x08 ///< Display 2 rows bit @see LCD_FUNCTION
	#define LCD_FORMAT_5x10 	0x04 ///< Format 5x10 pixels bit @see LCD_FUNCTION
	#define LCD_8_BIT 			0x10 ///< 8-bit interface bit @see LCD_FUNCTION
#define LCD_SET_CGRAM 			0x40 ///< Sets CGRAM address
#define LCD_SET_DDRAM 			0x80 ///< Sets DDRAM address

#define LCD_ROW1 0x00			///< First row address of the LCD
#define LCD_ROW2 0x40			///< Second row address of the LCD
#define LCD_BUSY_FLAG (1<<7) 	///< Busy flag mask

void LCD_Init(void);
void LCD_Update(void);
void LCD_Home(void);
void LCD_Position(uint8_t positionX, uint8_t positionY);
void LCD_Clear(void);
void LCD_Putc(uint8_t c);
void LCD_Puts(char* s);
void LCD_ShifDisplay(uint8_t shift, uint8_t dir);


#endif
