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

int main(void) {

	__asm("SVC #25");

	// register uint32_t data __asm("r0");
	uint32_t data;

	__asm volatile("MOV %0,R0": "=r"(data));
	printf(" data = %lu\n", data);

	for(;;);
}

__attribute__ ((naked)) void SVC_Handler(void) {
	__asm("MRS R0,MSP");
	__asm("B SVC_Handler_c");
}

void SVC_Handler_c(uint32_t *pBaseOfStackFrame) {
	printf("In SVC Handler");
	uint8_t *pReturn_addr = (uint8_t*) pBaseOfStackFrame[6];
	pReturn_addr-=2;

	uint8_t svc_number = *pReturn_addr;

	printf("Svc number is : %d\n", svc_number);
	svc_number+=4;

	pBaseOfStackFrame[0] = svc_number;
}
