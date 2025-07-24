/*
 * Panthera.c
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */
#include "Panthera.h"

void PTR_system_initialization(void)
{
	SYS_Config();
}

void PTR_main_interrupt_service(void)
{
	begin = stm_lld_getcounter(&STMD3);
	GPIO_TogglePin(&LED3);
	GPIO_TogglePin(&LED4);
	GPIO_TogglePin(&LED5);
	GPIO_TogglePin(&LED4);
	GPIO_TogglePin(&LED5);
	GPIO_TogglePin(&LED4);
	GPIO_TogglePin(&LED5);
	GPIO_TogglePin(&LED4);
	GPIO_TogglePin(&LED5);

    end = stm_lld_getcounter(&STMD3);
    elapsed = end - begin;
}

void PTR_inf_loop_system(void)
{

}
