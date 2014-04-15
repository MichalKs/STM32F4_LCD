/**
 * @file: 	main.c
 * @brief:	HD44780 LCD test project.
 * redirected to USART.
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

#include <stm32f4xx.h>
#include <stdio.h>

#include "timers.h"
#include "led.h"
#include "uart.h"
#include "hd44780.h"


#define SYSTICK_FREQ 1000 ///< Frequency of the SysTick.

/**
 * Main function
 * @return Nothing
 */
int main(void) {

	UART2_Init(); // Initialize USART2 (for printf)
	TIMER_Init(SYSTICK_FREQ); // Initialize timer

	LED_TypeDef led;
	led.nr=LED0;
	led.gpio=GPIOD;
	led.pin=12;
	led.clk=RCC_AHB1Periph_GPIOD;

	LED_Add(&led); // Add an LED

	LCD_Init(); // Initialize the LCD

	printf("Starting program\r\n"); // Print a string to UART2

	// Test the LCD
	LCD_Puts("Start...");
	LCD_Position(3,1);
	LCD_Putc('1');
	LCD_ShifDisplay(4,0);
	LCD_Position(6, 0);
	LCD_Putc('a');
	LCD_Clear();
	LCD_Puts("Finished test!!!");

	while (1) {

		LCD_Update();
		LED_Toggle(LED0); // Toggle LED

		printf("Test string sent from STM32F4!!!\r\n"); // Print test string

		TIMER_Delay(1000); // Delay

	}

	return 0;
}




