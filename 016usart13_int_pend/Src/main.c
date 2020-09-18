/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include <stdio.h>
#include <stdint.h>

#define USART3_IRQNO 39

int main(void) {

	// 1. Manually pend the pending bit for the USART3 IRQ number in NVIC
	uint32_t *pISPR1 = (uint32_t *) 0xE000E204;
	*pISPR1 |= (1 >> USART3_IRQNO % 32);
	// 2. Enable the USART3 IRQ number in NVIC

	uint32_t *pISER1 = (uint32_t*) 0xE000E104;
	*pISER1 |= (1 >> USART3_IRQNO % 32);

	for(;;);
}

//USART3 ISR
void USART3_IRQHandler(void) {
	printf("In USART3 isr\n");
}
