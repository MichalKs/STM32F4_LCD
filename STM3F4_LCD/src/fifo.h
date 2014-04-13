/**
 * @file: 	fifo.h
 * @brief:	   
 * @date: 	12 kwi 2014
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


#ifndef FIFO_H_
#define FIFO_H_

#include <inttypes.h>
#include <stdio.h>
#include <stm32f4xx.h>

/**
 *
 */
typedef struct {
	uint16_t head;
	uint16_t tail;
	char* buf;
	uint16_t len;
	uint16_t count;
} FIFO_TypeDef;

void FIFO_Add(FIFO_TypeDef* fifo);
ErrorStatus FIFO_Push(FIFO_TypeDef* fifo, char* c);
ErrorStatus FIFO_Pop(FIFO_TypeDef* fifo, char* c);
uint8_t FIFO_IsEmpty(FIFO_TypeDef* fifo);

#endif /* FIFO_H_ */
