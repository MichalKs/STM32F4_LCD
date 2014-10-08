/**
 * @file: 	hd44780_hal.h
 * @brief:	   
 * @date: 	8 paź 2014
 * @author: Michal Ksiezopolski
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

#ifndef HD44780_HAL_H_
#define HD44780_HAL_H_

#include <inttypes.h>


void    LCD_HAL_Write   (uint8_t data);
uint8_t LCD_HAL_Read    (void);
void    LCD_HAL_DataOut (void);
void    LCD_HAL_DataIn  (void);
void    LCD_HAL_Init    (void);
void    LCD_HAL_LowRS   (void);
void    LCD_HAL_HighRS  (void);
void    LCD_HAL_LowRW   (void);
void    LCD_HAL_HighRW  (void);
void    LCD_HAL_HighE   (void);
void    LCD_HAL_LowE    (void);

#endif /* HD44780_HAL_H_ */
