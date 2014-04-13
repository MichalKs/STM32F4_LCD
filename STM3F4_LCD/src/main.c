/**
 * @file: 	main.c
 * @brief:	Blinking LED test program with printf redirected to USART2
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

extern void Reset_Handler();
// Begin address for the initialisation values of the .data section.
// defined in linker script
extern unsigned int _sidata;
// Begin address for the .data section; defined in linker script
extern unsigned int __data_start__;
// End address for the .data section; defined in linker script
extern unsigned int __data_end__;
// Begin address for the .bss section; defined in linker script
extern unsigned int __bss_start__;
// End address for the .bss section; defined in linker script
extern unsigned int __bss_end__;
extern unsigned int _estack;
extern unsigned int __end__;
extern void (*__preinit_array_start[])(void) __attribute__((weak));


#define SYSTICK_FREQ 1000 ///< Frequency of the SysTick.

/**
 * Main function
 * @return Nothing
 */
int main(void) {

	UART2_Init(); // Initialize USART2 (for printf)
	TimerInit(SYSTICK_FREQ); // Initialize timer

	LED_TypeDef led;
	led.nr=LED0;
	led.gpio=GPIOD;
	led.pin=12;
	led.clk=RCC_AHB1Periph_GPIOD;

	LED_Add(&led); // Add an LED

//	char c;

	printf("Starting program\r\n");
	printf("Reset Handler at address: 0x%08x\r\n", (unsigned int)Reset_Handler);
	printf("Preinit array start at address: 0x%08x\r\n", (unsigned int)__preinit_array_start);

//	printf("Stack end at address: 0x%08x\r\n", _estack);
//	printf("Data initialization values starts at: 0x%08x\r\n", _sidata);
//	printf("Data starts at: 0x%08x\r\n", __data_start__);
//	printf("Data ends at: 0x%08x\r\n", __data_end__);
//	printf("BSS starts at: 0x%08x\r\n", __bss_start__);
//	printf("BSS ends at: 0x%08x\r\n", __bss_end__);
	printf("Testowy string!!!!!!!!!!!!!!!!!!!!!!!\r\n");
	printf("Testowy string!!!!!!!!!!!!!!!!!!!!!!!\r\n");
	printf("Testowy string!!!!!!!!!!!!!!!!!!!!!!!\r\n");
	printf("Testowy string!!!!!!!!!!!!!!!!!!!!!!!\r\n");
	printf("Testowy string!!!!!!!!!!!!!!!!!!!!!!!\r\n");
	printf("Testowy string!!!!!!!!!!!!!!!!!!!!!!!\r\n");

	while (1) {

		LED_Toggle(LED0); // Toggle LED
		LED_Toggle(LED1); // Toggle inexistent LED

//		c = USART2_Getc();
//		USART2_Putc(c);
		printf("Test string sent from STM32F4!!!\r\n"); // Print test string

		TimerDelay(2000); // Delay

	}

	return 0;
}




