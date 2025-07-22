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

/*
 * Application entry point.
 */
int main(void) {
  uint32_t reg;

  /* Initialization of all the imported components in the order specified in
     the application wizard. The function is generated automatically.*/
  componentsInit();


  /* PIT clock enabled.*/
  SPCSetPeripheralClockMode(30, SPC5_ME_PCTL_RUN(2) | SPC5_ME_PCTL_LP(2));

  /* Wait until PIT Peripheral will be ready.*/
  while (MC_ME.PS0.B.S_PIT_0 == 0U){
  }

  /* PIT configuration.*/
  reg = SPC5_PER_CLK - 1U;
  PIT_0.MCR.R         = 1;     /* PIT clock enabled, stop while debugging.  */
  PIT_0.CH[1].LDVAL.R = reg;   /* 1 second value using PIT clock frequency. */
  PIT_0.CH[1].CVAL.R  = reg;   /* 1 second value using PIT clock frequency. */
  PIT_0.CH[1].TFLG.R  = 1;     /* Interrupt flag cleared.                   */
  PIT_0.CH[1].TCTRL.R = 3;     /* Timer active, interrupt enabled.          */

  /* Interrupts are enabled and the system is ready.*/
  irqIsrEnable();

  PTR_system_initialization();

  /* Application main loop.*/
  for ( ; ; ) {
	  PTR_inf_loop_system();
	  osalThreadDelayMilliseconds(1000UL);
  }
}
