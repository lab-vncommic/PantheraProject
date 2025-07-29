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
 * @file    eeprom_cfg.c
 * @brief   EEPROM Driver configuration code.
 *
 * @addtogroup EEPROM
 * @{
 */

#include "eeprom_cfg.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/
/**
 * @brief   Block configuration structure.
 */
BLOCKConfig blockconf[2] = {
  {
  /* block start address*/
  EEPROM_DATAFLASH_ADDR(0UL),
  /*block size*/
  EEPROM_DATAFLASH_BLOCKSIZE,
  /* space */
  FLASH_HIGH_SPACE,
  /* number */
  0U,
  },
  {
   /* block start address*/
  EEPROM_DATAFLASH_ADDR(1UL),
  /*block size*/
  EEPROM_DATAFLASH_BLOCKSIZE,
  /* space */
  FLASH_HIGH_SPACE,
  /* number */
  1U,
  },
};

/**
 * @brief   EEPROM configuration structure.
 */
EEPROMConfig eeprom_cfg = {
  /* the number of blocks used for emulation */
  2U,
  /* the block configuration array pointer   */
  blockconf,
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
