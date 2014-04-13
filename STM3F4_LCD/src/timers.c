/**
 * @file: 	timers.c
 * @brief:	Timing control functions.
 * @date: 	9 kwi 2014
 * @author: Michal Ksiezopolski
 * 
 *
 * Control of the SysTick and software timers
 * incremented based on SysTick interrupts.
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


volatile uint32_t delayTimer; ///< Delay timer.

/**
 * @brief Initiate SysTick with a given frequency.
 * @param freq Required frequency of the timer
 *
 */
void TimerInit(uint32_t freq) {

	RCC_ClocksTypeDef RCC_Clocks;

	RCC_GetClocksFreq(&RCC_Clocks); // Complete the clocks structure with current clock settings.

	SysTick_Config(RCC_Clocks.HCLK_Frequency / freq); // Set SysTick frequency
}
/**
 * @brief Delay function.
 * @param ms Milliseconds to delay.
 * @see delayTimer
 */
void TimerDelay(uint32_t ms) {

	delayTimer = ms;

	while (delayTimer); // Delay

}

/**
 * @brief Interrupt handler for SysTick.
 */
void SysTick_Handler(void) {

	if (delayTimer) {
	  delayTimer--; // Decrement delayTimer
	}

}
