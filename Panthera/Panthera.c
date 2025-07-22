/*
 * Panthera.c
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */
#include "Panthera.h"

void PTR_system_initialization(void)
{
	GPIO_SetPin(&LED3);
	GPIO_SetPin(&LED4);
	GPIO_SetPin(&LED5);
	osalThreadDelayMilliseconds(2000UL);
	GPIO_ClearPin(&LED3);
	GPIO_ClearPin(&LED4);
	GPIO_ClearPin(&LED5);
}

void PTR_main_interrupt_service(void)
{
	GPIO_TogglePin(&LED4);
	GPIO_TogglePin(&LED5);
}

void PTR_inf_loop_system(void)
{
	GPIO_TogglePin(&LED3);
}
