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
#include"main.h"

void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);

void enable_processor_faults(void);
void init_systick_timer(uint32_t tick_hz);
void init_scheduler_stack(uint32_t sched_top_of_stack);
void init_tasks_stack(void);
void switch_sp_to_psp(void);


uint32_t psp_of_tasks[MAX_TASKS] = {T1_STACK_START, T2_STACK_START, T3_STACK_START, T4_STACK_START};
uint32_t task_handlers[MAX_TASKS];
int current_task = 0;

int main(void) {

	enable_processor_faults();

	init_scheduler_stack(SCHED_STACK_START);

	task_handlers[0] = (uint32_t)task1_handler;
	task_handlers[1] = (uint32_t)task2_handler;
	task_handlers[2] = (uint32_t)task3_handler;
	task_handlers[3] = (uint32_t)task4_handler;

	init_tasks_stack();

	init_systick_timer(TICK_HZ);

	switch_sp_to_psp();

	task1_handler();

	for(;;);
}

void task1_handler(void) {
	while(1) {
		printf("This is task1\n");
	}
}

void task2_handler(void) {
	while(1) {
		printf("This is task2\n");
	}
}

void task3_handler(void) {
	while(1) {
		printf("This is task3\n");
	}
}

void task4_handler(void) {
	while(1) {
		printf("This is task4\n");
	}
}

void init_systick_timer(uint32_t tick_hz) {
	uint32_t *pSRVR =  (uint32_t*) 0xE000E014;
	uint32_t *pSCSR = (uint32_t*) 0xE000E010;
	uint32_t count_value = (SYSTICK_TIMER_CLK/tick_hz) - 1;

	// clear the value of SVR
	*pSRVR &= ~(0x00FFFFFFFF);

	// load the value into SVR
	*pSRVR |= count_value;

	// do some settings
	*pSCSR |= (1 << 1); // Enable systick exception request
	*pSCSR |= (1 << 2); // Indicates the clock source as the processor clock

	// enable the systick
	*pSCSR |= (1 << 0); // enables the counter
}

__attribute__((naked)) void init_scheduler_stack(uint32_t sched_top_of_stack) {
	__asm volatile("MSR MSP,%0": : "r" (sched_top_of_stack) : );
	__asm volatile("BX LR");
}

void init_tasks_stack(void) {
	uint32_t *pPSP;

	for(int i = 0; i < MAX_TASKS; i++) {
		pPSP = (uint32_t*) psp_of_tasks[i];

		pPSP--;
		*pPSP = DUMMY_XPSR; // 0x001000000

		pPSP--; // PC
		*pPSP = task_handlers[i];

		pPSP--; // LR
		*pPSP = 0xFFFFFFFD;

		for(int j = 0; j < 13; j++) {
			pPSP--;
			*pPSP = 0;
		}

		psp_of_tasks[i] = (uint32_t)pPSP;
	}
}

void enable_processor_faults(void) {
	uint32_t *pSHCSR = (uint32_t*)0xE000ED24;

	*pSHCSR |= (1 << 16); // mem manage
	*pSHCSR |= (1 << 17); // bus fault
	*pSHCSR |= (1 << 18); // usage fault
}

uint32_t get_psp_value(void) {
	return psp_of_tasks[current_task];
}

__attribute__ ((naked)) void switch_sp_to_psp(void) {
	//1. initialize the PSP with TASK1 stack start address

	// get the value of psp of current taks
	__asm volatile("PUSH {LR}"); // preserve LR which connects back to main
	__asm volatile("BL get_psp_value");
	__asm volatile("MSR PSP, R0");
	__asm volatile("POP {LR}");

	//2. change sp to PSP using CONTROL register
	__asm volatile("MOV R0,#0x02");
	__asm volatile("MSR CONTROL,R0");
	__asm volatile("BX LR");
}

void save_psp_value(uint32_t current_psp_value) {
	psp_of_tasks[current_task] = current_psp_value;
}

void update_next_task(void) {
	current_task++;
	current_task %= MAX_TASKS;
}

void SysTick_Handler(void) {
	/* Save the context of the current task */

	// !. Get current running task's PSP value
	__asm volatile("MRS R0,PSP");
	// 2. Using that PSP value store SF2 (R4 to R11)
	__asm volatile("STMDB R0!,{R4-R11}");

	// 3. Save the current value of PSP
	__asm volatile("BL save_psp_value");

	/* Retrieve the current value of the next task */

	// 1. Decide next task to run
	__asm volatile("BL update_next_task");

	// 2. get its past PSP value
	__asm volatile("BL get_psp_value");

	// 3. Using that PSP value retrieve SF2(R4 to R11)
	__asm volatile("LDMIA R0!,{R4-R11}");

	// 4. update PSP and exit
	__asm volatile("MSR PSP,R0");
}
