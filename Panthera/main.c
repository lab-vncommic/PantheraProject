/****************************************************************************
*
* Copyright © 2015-2019 STMicroelectronics - All Rights Reserved
*
* This software is licensed under SLA0098 terms that can be found in the
* DM00779817_1_0.pdf file in the licenses directory of this software product.
* 
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED, 
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
*****************************************************************************/

/* Inclusion of the main header files of all the imported components in the
   order specified in the application wizard. The file is generated
   automatically.*/
#include "components.h"

// User's library
#include "Panthera.h"

uint16_t cnt;

void PIT0_ISR(void)
{
	PTR_main_interrupt_service();
}

/*
 * Application entry point.
 */
int main(void) {
	/* Initialization of all the imported components in the order specified in
	 the application wizard. The function is generated automatically.*/
	componentsInit();
	PTR_system_initialization();

	/* Interrupts are enabled and the system is ready.*/
	irqIsrEnable();

	/* Application main loop.*/
	for ( ; ; ) {
	  PTR_inf_loop_system();
	}
}
