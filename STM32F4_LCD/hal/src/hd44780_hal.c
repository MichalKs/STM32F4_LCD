/**
 * @file: 	hd44780_hal.c
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

#include <hd44780_hal.h>

#include <stm32f4xx.h>

/*
 * Ports and pins of the LCD
 */
#define LCD_DATA_PORT GPIOD                 ///< LCD data GPIO
#define LCD_DATA_CLK  RCC_AHB1Periph_GPIOD  ///< LCD data RCC bit
#define LCD_CTRL_PORT GPIOD                 ///< LCD control GPIO
#define LCD_CTRL_CLK  RCC_AHB1Periph_GPIOD  ///< LCD control RCC bit

#define LCD_RS  GPIO_Pin_4 ///< Register select pin
#define LCD_RW  GPIO_Pin_5 ///< Read/write pin
#define LCD_E   GPIO_Pin_6 ///< Enable pin

/*
 * We use the 4-bit interface
 */
#define LCD_D4  GPIO_Pin_0 ///< Data 4 pin
#define LCD_D5  GPIO_Pin_1 ///< Data 5 pin
#define LCD_D6  GPIO_Pin_2 ///< Data 6 pin
#define LCD_D7  GPIO_Pin_3 ///< Data 7 pin

/**
 * @brief Low level initalization of the LCD.
 */
void LCD_HAL_Init(void) {

  // enable clocks for GPIOs
  RCC_AHB1PeriphClockCmd(LCD_DATA_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd(LCD_CTRL_CLK, ENABLE);

  // Set LCD data pins initially as output
  LCD_HAL_DataOut();

  // Set control pins as output
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin   = (LCD_RS|LCD_RW|LCD_E);
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(LCD_CTRL_PORT,&GPIO_InitStructure);

  // Clear all control signals initially
  GPIO_ResetBits(LCD_CTRL_PORT, LCD_RW|LCD_RS|LCD_E);

}

void LCD_HAL_LowRS(void) {
  GPIO_ResetBits(LCD_CTRL_PORT, LCD_RS);
}
void LCD_HAL_HighRS(void) {
  GPIO_SetBits(LCD_CTRL_PORT, LCD_RS);
}
void LCD_HAL_LowRW(void) {
  GPIO_ResetBits(LCD_CTRL_PORT, LCD_RW);
}
void LCD_HAL_HighRW(void) {
  GPIO_SetBits(LCD_CTRL_PORT, LCD_RW);
}
void LCD_HAL_HighE(void) {
  GPIO_SetBits(LCD_CTRL_PORT, LCD_E);
}
void LCD_HAL_LowE(void) {
  GPIO_ResetBits(LCD_CTRL_PORT, LCD_E);
}

/**
 * @brief Set data lines as output.
 */
void LCD_HAL_DataOut(void) {
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin   = (LCD_D4|LCD_D5|LCD_D6|LCD_D7);
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(LCD_DATA_PORT, &GPIO_InitStructure);
}

/**
 * @brief Set data lines as input with pull up
 */
void LCD_HAL_DataIn(void) {
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin   = (LCD_D4|LCD_D5|LCD_D6|LCD_D7);
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(LCD_DATA_PORT,&GPIO_InitStructure);
}

/**
 * @brief This functions sets data on the data lines when writing.
 * @param data Data to write
 */
void LCD_HAL_Write(uint8_t data) {

  // set the various bits
  if (data & (1<<3))
    GPIO_SetBits(LCD_DATA_PORT, LCD_D7);
  else
    GPIO_ResetBits(LCD_DATA_PORT, LCD_D7);

  if (data & (1<<2))
    GPIO_SetBits(LCD_DATA_PORT, LCD_D6);
  else
    GPIO_ResetBits(LCD_DATA_PORT, LCD_D6);

  if (data & (1<<1))
    GPIO_SetBits(LCD_DATA_PORT, LCD_D5);
  else
    GPIO_ResetBits(LCD_DATA_PORT, LCD_D5);

  if (data & (1<<0))
    GPIO_SetBits(LCD_DATA_PORT, LCD_D4);
  else
    GPIO_ResetBits(LCD_DATA_PORT, LCD_D4);
}

/**
 * @brief Reads the data lines
 * @return Read data.
 */
uint8_t LCD_HAL_Read(void) {
  uint8_t result = 0;

  GPIO_SetBits(LCD_CTRL_PORT, LCD_E);

  if(GPIO_ReadInputDataBit(LCD_DATA_PORT,LCD_D7))
    result |= (1<<3);
  if(GPIO_ReadInputDataBit(LCD_DATA_PORT,LCD_D6))
    result |= (1<<2);
  if(GPIO_ReadInputDataBit(LCD_DATA_PORT,LCD_D5))
    result |= (1<<1);
  if(GPIO_ReadInputDataBit(LCD_DATA_PORT,LCD_D4))
    result |= (1<<0);

  GPIO_ResetBits(LCD_CTRL_PORT, LCD_E);
  return result;
}
