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
/**
 * @file    irq_cfg.c
 * @brief   IRQ configuration code.
 *
 * @addtogroup IRQ
 * @{
 */

#include "irq.h"

#include "board.h"

#include "../../../Panthera.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module ISRs.                                                              */
/*===========================================================================*/

/*
 * PIT0_CH1_IRQ ISR function (vector 227).
 */
IRQ_HANDLER(PIT0_CH1_TimerExpired) {

  IRQ_PROLOGUE();

  /* ISR code here.*/
  PTR_main_interrupt_service();
  PIT_0.CH[1].TFLG.R  = 1;    

  IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Generated initialization code.
 *
 * @special
 */
void irq_cfg_init(void) {

  INTC_PSR(ISR_VECTOR_PIT0_CH1_IRQ) = INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8);
}

/** @} */
