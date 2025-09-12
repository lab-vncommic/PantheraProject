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
 * @file    can_lld_cfg.c
 * @brief   CAN Driver configuration code.
 *
 * @addtogroup CAN
 * @{
 */
 
#include "can_lld_cfg.h"

#if (LLD_USE_CAN == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Structure defining the CAN configuration "mcanconf".
 */
CANConfig can_config_mcanconf = {
  /* loopback */
  CAN_NO_LOOPBACK,
  /* endianness */
  CAN_BIG_ENDIAN,
  /* sync jump width */ 
  3U,  
  /* time segment before sample point */
  2U,
  /* time segment after sample point */
  3U,
  /* clock prescaler */
  50U,
  
  /* Canfd configuration */
  FALSE,
  /* canfd bit rate switch */
  FALSE,
   /* sync jump width */ 
  1U,  
  /* time segment before sample point */
  1U,
  /* time segment after sample point */
  0U,
  /* clock prescaler */
  10U,
  /* transceiver delay compensation enabled */
  FALSE,
  /* transmitter delay compensation value */
  0U,
  /* transmitter delay compensation offset */
  0U,
  /* transmitter delay compensation filter window length */
  0U,

  /*can rx interrupt line*/
  CAN_LINE1_INT,
  /*can rx interrupt call back */
   mcanconf_panthera_rx_can,
  /* FIFO 0 is not used */
  CAN_FIFO0_INT_DISABLE,0,NULL,0,NULL,0,NULL,0,NULL,
  /* FIFO 1 is not used */
  CAN_FIFO1_INT_DISABLE,0,NULL,0,NULL,0,NULL,0,NULL,
  /* rx standard buffer filters */
  {
  {0x0UL,50UL,0U,7U}, /* standard filter 0*/
  {0x1UL,50UL,0U,7U}, /* standard filter 1*/
  {0x2UL,50UL,0U,7U}, /* standard filter 2*/
  {0x3UL,50UL,0U,7U}, /* standard filter 3*/
  {0x4UL,50UL,0U,7U}, /* standard filter 4*/
  {0x5UL,50UL,0U,7U}, /* standard filter 5*/
  {0x6UL,50UL,0U,7U}, /* standard filter 6*/
  {0x7UL,50UL,0U,7U}, /* standard filter 7*/
  {0x8UL,50UL,0U,7U}, /* standard filter 8*/
  {0x9UL,50UL,0U,7U}, /* standard filter 9*/
  {0xaUL,50UL,0U,7U}, /* standard filter 10*/
  {0xbUL,50UL,0U,7U}, /* standard filter 11*/
  {0xcUL,50UL,0U,7U}, /* standard filter 12*/
  {0xdUL,50UL,0U,7U}, /* standard filter 13*/
  {0xeUL,50UL,0U,7U}, /* standard filter 14*/
  {0xfUL,50UL,0U,7U}, /* standard filter 15*/
  {0x10UL,50UL,0U,7U}, /* standard filter 16*/
  {0x11UL,50UL,0U,7U}, /* standard filter 17*/
  {0x12UL,50UL,0U,7U}, /* standard filter 18*/
  {0x13UL,50UL,0U,7U}, /* standard filter 19*/
  {0x14UL,50UL,0U,7U}, /* standard filter 20*/
  {0x15UL,50UL,0U,7U}, /* standard filter 21*/
  {0x16UL,50UL,0U,7U}, /* standard filter 22*/
  {0x17UL,50UL,0U,7U}, /* standard filter 23*/
  {0x18UL,50UL,0U,7U}, /* standard filter 24*/
  {0x19UL,50UL,0U,7U}, /* standard filter 25*/
  {0UL,0UL,0U,0U},  /* standard_filter unused 0 (MISRA Compliance) */
  {0UL,0UL,0U,0U},  /* standard_filter unused 1 (MISRA Compliance) */
  },
    /* rx extended buffer filters */
  {
  {0UL,0UL,0U,0U},  /* extended_filter unused 0 (MISRA Compliance) */
  {0UL,0UL,0U,0U},  /* extended_filter unused 1 (MISRA Compliance) */
  },
/* number of standard filters */
  26U,
/* number of extended filters */
  0U,
  /* number of Rx Buffers */
  51U,
/* number of FIFO 0 Buffers */
  0U,
/* number of FIFO 1 Buffers */
  0U,
/* Transmission Mode */
  CAN_DEDICATED_TXBUFFER,
/* number of Tx Buffers */
  16U,
/* number of FIFO or QUEUE Tx Buffers */
  8U,
/* Error Callback*/
  NULL,
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

#endif /* LLD_USE_CAN */

/** @} */
