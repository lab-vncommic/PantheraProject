/****************************************************************************
*
* Copyright © 2018-2019 STMicroelectronics - All Rights Reserved
*
* This software is licensed under SLA0098 terms that can be found in the
* DM00779817_1_0.pdf file in the licenses directory of this software product.
* 
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED, 
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
*****************************************************************************/

/**
 * @file    eeprom_cfg.h
 * @brief   EEPROM Driver configuration macros and structures.
 *
 * @addtogroup EEPROM
 * @{
 */

#ifndef _EEPROM_CFG_H_
#define _EEPROM_CFG_H_

#include "eeprom.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/
#define EEPROM_DATAFLASH_BLOCKSIZE          0x4000UL
#define EEPROM_DATAFLASH_ADDR(x)           (0x800000UL + (EEPROM_DATAFLASH_BLOCKSIZE *x))

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* EEPROM_CONFIG structure defined in eeprom_cfg.c.*/
extern EEPROMConfig eeprom_cfg;
extern BLOCKConfig blockconf[2];

#endif /* _EEPROM_CFG_H_ */

/** @} */
