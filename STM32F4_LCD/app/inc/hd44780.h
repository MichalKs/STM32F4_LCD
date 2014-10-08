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

void LCD_Init(void);
void LCD_Update(void);
void LCD_Home(void);
void LCD_Position(uint8_t positionX, uint8_t positionY);
void LCD_Clear(void);
void LCD_Putc(uint8_t c);
void LCD_Puts(char* s);
void LCD_ShifDisplay(uint8_t shift, uint8_t dir);

#endif
