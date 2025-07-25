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
 * @file    saradc_lld_cfg.c
 * @brief   SARADC Driver configuration code.
 *
 * @addtogroup SARADC
 * @{
 */

#include "saradc_lld_cfg.h"

#if (LLD_USE_SARADC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/




/**
 * @brief   Structure defining the configuration "saradcconfdma".
 */
SARADCConfig saradc_config_saradcconfdma = {
   /* conversion mode */
  SARADC_MODE_SCAN,
  
  /* Trigger Mode */
  SARADC_TRIGGER_DISABLED,
  
  /* CTR registers configuration (CRES,PRECHG,INPSAMP) */
  {
  /* CTR0 */ {SARADC_CRES_HIGH, 0U, 100U},
  /* CTR1 */ {SARADC_CRES_HIGH, 0U, 100U},
  /* CTR2 */ {SARADC_CRES_HIGH, 0U, 100U},
  /* CTR3 */ {SARADC_CRES_HIGH, 0U, 100U},
  },
  /* DMA enabled */
  TRUE,
  /* DMA configuration */
  {SARADC_CHANNEL_INTERNAL,63U,SARADC_REFERENCE_DEFAULT,SARADC_PRECHARGE_DISABLED,SARADC_CTR0,3U,16U},  
   /* DMA conversion callback */
  saradcconfdma_conv_cb,
  /* Channels configurations */
  NULL,

  /* Number of channels */
  0U,

  /* Conversion callback */
  NULL,

  /* No analog watchdog configured */
  {
  {(float)0,(float)0},
  },
  (float)0,
  0U,
};

/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/


#endif /* LLD_USE_SARADC */
