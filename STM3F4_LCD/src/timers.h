/**
 * @file: 	timers.h
 * @brief:	Timing control functions.
 * @date: 	9 kwi 2014
 * @author: Michal Ksiezopolski
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

#ifndef TIMERS_H_
#define TIMERS_H_

/**
 * @addtogroup StandardDrivers
 * @{
 */

/**
 * @addtogroup Timers
 * @{
 */

void TimerInit(uint32_t freq);
void TimerDelay(uint32_t ms);

/**
 * @}
 */

/**
 * @}
 */

#endif /* TIMERS_H_ */
