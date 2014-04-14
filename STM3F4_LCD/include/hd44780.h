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

#include <stm32f4xx.h>

/*
 * Ports and pins of the LCD
 */
#define LCD_DATA_PORT GPIOD 				///< LCD data GPIO
#define LCD_DATA_CLK  RCC_AHB1Periph_GPIOD 	///< LCD data RCC bit
#define LCD_CTRL_PORT GPIOD 				///< LCD control GPIO
#define LCD_CTRL_CLK  RCC_AHB1Periph_GPIOD 	///< LCD control RCC bit

#define LCD_RS 	GPIO_Pin_4 ///< Register select pin
#define LCD_RW 	GPIO_Pin_5 ///< Read/write pin
#define LCD_E 	GPIO_Pin_6 ///< Enable pin

/*
 * We use the 4-bit interface
 */
#define LCD_D4 	GPIO_Pin_0 ///< Data 4 pin
#define LCD_D5 	GPIO_Pin_1 ///< Data 5 pin
#define LCD_D6 	GPIO_Pin_2 ///< Data 6 pin
#define LCD_D7 	GPIO_Pin_3 ///< Data 7 pin

/*
 * LCD commands HD44780
 */
#define LCD_CLEAR_DISPLAY 		0x01
#define LCD_HOME 				0x02
#define LCD_ENTRY_MODE 			0x04
	#define LCD_INCREMENT 		0x02
	#define LCD_SHIFT_LEFT 		0x01
#define LCD_DISPLAY_ON_OFF 		0x08
	#define LCD_DISPLAY_ON		0x04
	#define LCD_CURSOR_ON 		0x02
	#define LCD_BLINK_ON 		0x01
#define LCD_CURSOR_SHIFT 		0x10
	#define LCD_SHIFT_RIGHT 	0x04
	#define LCD_SHIFT_DISPLAY 	0x08
#define LCD_FUNCTION			0x20
	#define LCD_2_ROWS	 		0x08
	#define LCD_FORMAT_5x10 	0x04
	#define LCD_8_BIT 			0x10
#define LCD_SET_CGRAM 			0x40
#define LCD_SET_DDRAM 			0x80

#define LCD_ROW1 0x00
#define LCD_ROW2 0x40
#define LCD_BUSY_FLAG (1<<7)

void LCD_Init(void);
void LCD_Update(void);
void LCD_Home(void);
void LCD_Position(uint8_t position);
void LCD_Clear(void);
void LCD_Putc(uint8_t c);
void LCD_Puts(char* s);


#endif
