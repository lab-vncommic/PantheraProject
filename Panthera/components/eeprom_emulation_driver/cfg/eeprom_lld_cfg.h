/****************************************************************************
*
* Copyright © 2018-2019 STMicroelectronics - All Rights Reserved
*
* This software is licensed under SLA0098 terms that can be found in the
* DM00779817_1_0.pdf file in the licenses directory of this software component.
* 
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED, 
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
*****************************************************************************/


/**
 * @file    eeprom_lld_cfg.h
 * @brief   EEPROM Driver configuration macros and structures.
 *
 * @addtogroup EEPROM
 * @{
 */

#ifndef _EEPROM_LLD_CFG_H_
#define _EEPROM_LLD_CFG_H_

#include "flashdriver.h"
#include "spc5_lld.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    EEPROM Emulation flag definitions
 * @{
 */

#define EEPROM_SWAP_START                0xFEFFFFFFFFFFFFFFUL
#define EEPROM_BLOCK_ACTIVE              0x80FCFEFEFFFFFFFFUL
#define EEPROM_BLOCK_INVALID             0x80FCFCFEFCFFFFFFUL

#define EEPROM_PROGRAM_START             0x80F8FCFCFCFFFCFCUL
#define EEPROM_PROGRAM_COMPLETE          0x80E0E0FCF0F8F8FCUL
#define EEPROM_ID_INVALID                0x00E0E0F080E0F8F8UL
#define EEPROM_ID_BAD					 0x00C0E0E080E0E0E0UL

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

/* SSD_CONFIG structure defined in eeprom_lld_cfg.c.*/
extern SSD_CONFIG ssdConfig;

#endif /* _EEPROM_LLD_CFG_H_ */

/** @} */
