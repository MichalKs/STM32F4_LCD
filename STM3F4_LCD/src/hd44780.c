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

#include "hd44780.h"
#include "timers.h"

static void LCD_Write(uint8_t data);
static uint8_t LCD_Read();
static void LCD_DataOut();
static void LCD_DataIn();
static void LCD_SendData(uint8_t data);
static void LCD_SendCommand(uint8_t command);
static uint8_t LCD_ReadFlag();

/**
 * Current row and column for keeping track of position
 */
static uint8_t row;
static uint8_t column;
/**
 * Data displayed on the LCD
 */
//static char data[32];

/**
 * Initialize the display.
 *
 */
void LCD_Init() {

	// Wait 50 ms for voltage to settle.
	TimerDelay(50);
	LCD_DataOut();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=(LCD_RS|LCD_RW|LCD_E);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(LCD_CTRL_PORT,&GPIO_InitStructure);

	//clear all control signals initially
	GPIO_ResetBits(LCD_CTRL_PORT,LCD_RW|LCD_RS|LCD_E);

	//initialization in 4-bit interface
	LCD_Write(0b0011);
	TimerDelay(5);

	LCD_Write(0b0011);
	TimerDelay(1);

	LCD_Write(0b0011);
	TimerDelay(1);

	LCD_Write(0b0010);
	TimerDelay(1);
	// 2 row display
	LCD_SendCommand(LCD_FUNCTION|LCD_2_ROWS);
	LCD_SendCommand(LCD_DISPLAY_ON_OFF|LCD_DISPLAY_ON|LCD_CURSOR_ON|LCD_BLINK_ON);

	LCD_Clear();
}
/**
 * Go to the beginning of the display.
 *
 */
void LCD_Home() {
	LCD_SendCommand(LCD_HOME);
	row=column=0;
}

/**
 * Position the LCD at a given memory location.
 * @param position
 */
void LCD_Position(uint8_t position) {
	LCD_SendCommand(LCD_SET_DDRAM| (position & 0x7f));
}
/**
 * Clear the display.
 */
void LCD_Clear() {
	LCD_SendCommand(LCD_CLEAR_DISPLAY);
}
/**
 * Print a character.
 */
void LCD_Putc(char c) {
	LCD_SendData(c);
}
/**
 * Print a string ended with '\0'.
 */
void LCD_Puts(char* s) {
	uint8_t i=0;
	while (s[i]!='\0') {
		LCD_Putc(s[i++]);
	}
}
/**
 * Send data to LCD
 */
static void LCD_SendData(uint8_t data) {

	// rs high and rw low for writing data
	GPIO_SetBits(LCD_CTRL_PORT,LCD_RS);
	GPIO_ResetBits(LCD_CTRL_PORT,LCD_RW);

	LCD_DataOut();

	// write higher 4 bits first
	GPIO_SetBits(LCD_CTRL_PORT,LCD_E);
	LCD_Write(data>>4);
	GPIO_ResetBits(LCD_CTRL_PORT,LCD_E);

	GPIO_SetBits(LCD_CTRL_PORT,LCD_E);
	LCD_Write(data);
	GPIO_ResetBits(LCD_CTRL_PORT,LCD_E);

	/*// update position
	column+=1;
	// my lcd has max 16 characters
	if (column==16) {
		row=~row; // we have two rows
		column=0;
		//change row when columns full
		if (row==0)
			LCD_Position(LCD_ROW1);
		else
			LCD_Position(LCD_ROW2);
	}*/
	// wait until the command gets processed
	while (LCD_ReadFlag()  & LCD_BUSY_FLAG);

}
/**
 * Send a command to the LCD.
 */
static void LCD_SendCommand(uint8_t command) {
	// rs low and rw low for writing command
	GPIO_ResetBits(LCD_CTRL_PORT,LCD_RW|LCD_RS);
	LCD_DataOut();

	// write higher 4 bits first
	GPIO_SetBits(LCD_CTRL_PORT,LCD_E);
	LCD_Write(command>>4);
	GPIO_ResetBits(LCD_CTRL_PORT,LCD_E);

	GPIO_SetBits(LCD_CTRL_PORT,LCD_E);
	LCD_Write(command);
	GPIO_ResetBits(LCD_CTRL_PORT,LCD_E);

	// wait until the command gets processed
	while (LCD_ReadFlag()  & LCD_BUSY_FLAG);


}
/**
 * Read the busy flag.
 */
static uint8_t LCD_ReadFlag() {
	LCD_DataIn();
	GPIO_ResetBits(LCD_CTRL_PORT,LCD_RS);
	GPIO_SetBits(LCD_CTRL_PORT,LCD_RW);

	uint8_t result=0;
	result=(LCD_Read()<<4);
	result|=LCD_Read();
	return result;

}
/**
 * Set data lines as output.
 */
static void LCD_DataOut() {
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin=(LCD_D4|LCD_D5|LCD_D6|LCD_D7);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(LCD_DATA_PORT,&GPIO_InitStructure);
}
/**
 * Set data lines as input with pull up
 */
static void LCD_DataIn() {
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin=(LCD_D4|LCD_D5|LCD_D6|LCD_D7);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(LCD_DATA_PORT,&GPIO_InitStructure);
}
/**
 * This functions sets data on the data lines when writing.
 */
static void LCD_Write(uint8_t data) {

	if (data & (1<<3))
		GPIO_SetBits(LCD_DATA_PORT,LCD_D7);
	else
		GPIO_ResetBits(LCD_DATA_PORT,LCD_D7);

	if (data & (1<<2))
		GPIO_SetBits(LCD_DATA_PORT,LCD_D6);
	else
		GPIO_ResetBits(LCD_DATA_PORT,LCD_D6);

	if (data & (1<<1))
		GPIO_SetBits(LCD_DATA_PORT,LCD_D5);
	else
		GPIO_ResetBits(LCD_DATA_PORT,LCD_D5);

	if (data & (1<<0))
		GPIO_SetBits(LCD_DATA_PORT,LCD_D4);
	else
		GPIO_ResetBits(LCD_DATA_PORT,LCD_D4);

}
/**
 * Reads the data lines
 */
static uint8_t LCD_Read() {
	uint8_t result=0;

	GPIO_SetBits(LCD_CTRL_PORT,LCD_E);

	if(GPIO_ReadInputDataBit(LCD_DATA_PORT,LCD_D7))
		result |= (1<<3);
	if(GPIO_ReadInputDataBit(LCD_DATA_PORT,LCD_D6))
		result |= (1<<2);
	if(GPIO_ReadInputDataBit(LCD_DATA_PORT,LCD_D5))
		result |= (1<<1);
	if(GPIO_ReadInputDataBit(LCD_DATA_PORT,LCD_D4))
		result |= (1<<0);

	GPIO_ResetBits(LCD_CTRL_PORT,LCD_E);
	return result;
}

