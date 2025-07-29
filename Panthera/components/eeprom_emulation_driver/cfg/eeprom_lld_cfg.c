/****************************************************************************
*
* Copyright © 2017-2019 STMicroelectronics - All Rights Reserved
*
* This software is licensed under SLA0098 terms that can be found in the
* DM00779817_1_0.pdf file in the licenses directory of this software product.
* 
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED, 
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
*****************************************************************************/

/**
 * @file    eeprom_lld_cfg.c
 * @brief   EEPROM Driver configuration code.
 *
 * @addtogroup EEPROM
 * @{
 */

#include "eeprom_lld_cfg.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/
SSD_CONFIG ssdConfig = {
  C55_REG_BASE, /* C55 control register base */
  MAIN_ARRAY_BASE, /* base of main array */
  { 0, 0, 0 }, /* blocks info of low address space */
  { 0, 0, 0 }, /* blocks info of mid address space */
  { 0, 0, 0 }, /* blocks info of high address space */
  0, /* number of blocks in 256K address space */
  UTEST_ARRAY_BASE, /* base of UTEST array */
  TRUE, /* interface flag indicate main or alternate interface */
  C55_PROGRAMMABLE_SIZE,/* programmable size */
  FALSE /* debug mode selection */
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

/** @} */
