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
 * @file    FlashMap.h
 * @brief   FlashMap for SPC582Bxx
 * @details See component documentation
 *
 *
 * @addtogroup SPC5_FLASH
 * @{
 */
/*lint -save -e* */
#ifndef _FLASHMAP_H_
#define _FLASHMAP_H_

#define C55_REG_BASE                    0xF7FE0000UL
#define MAIN_ARRAY_BASE                 0x00404000UL
#define UTEST_ARRAY_BASE                0x00400000UL
#define UTEST_ARRAY_SIZE                0x00004000UL

#define C55_PROGRAMMABLE_SIZE           0x80U

#define BUFFER_SIZE_BYTE                0x1000U

/* Lock State */
#define LOCK_ALL_BLOCKS                 0xFFFFFFFFUL
#define UNLOCK_ALL_BLOCKS               0x00000000UL

/* Low Block address map */
static const UINT32 LowBlock_addr[] =
{                                                    
  0x00404000, 0x00FC0000, 0x00FC4000, 0x00FC8000,    /* 0 - 1 - 2 - 3 */
  0x00FCC000, 0x00FD0000, 0x00FD8000, 0x00FE0000,    /* 4 - 5 - 6 - 7 */
  0x00FF0000,                                        /* 8             */
};
/* Low Block size */
static const UINT32 LowBlock_size[] =
{                                                   
  0x00004000, 0x00004000, 0x00004000, 0x00004000,    /* 0 - 1 - 2 - 3 */
  0x00004000, 0x00008000, 0x00008000, 0x00010000,    /* 4 - 5 - 6 - 7 */
  0x00010000,                                        /* 8             */
};

/* Large Block address map */
static const UINT32 LargeBlock_addr[] =
{                                                    
  0x01000000, 0x01020000, 0x01040000, 0x01060000,    /* 0 - 1 - 2 - 3 */
  0x01080000, 0x010A0000                             /* 4 - 5         */
};
/* Large Block size */
static const UINT32 LargeBlock_size[] =
{                                                    
  0x00020000, 0x00020000, 0x00020000, 0x00020000,	 /* 0 - 1 - 2 - 3 */
  0x00020000, 0x00020000,                            /* 4 - 5         */
};

/* Dataflash block (High Address Space)  address map */
static const UINT32 DataflashBlock_addr[] =
{                                                    
  0x00800000, 0x00804000, 0x00808000, 0x0080C000,    /* 0 - 1 - 2 - 3 */
};

/* Dataflash block (High Address Space)  address map */
static const UINT32 DataflashBlock_size[] =
{                                                    
  0x00004000, 0x00004000, 0x00004000, 0x00004000,    /* 0 - 1 - 2 - 3 */
};

#define FLASH_ARRAY_SIZE                0x100000UL    /* Flash total size */


/* Prototype of error trap function */
void ErrorTrap(UINT32 returnCode);

/* Typedef for null callback */
typedef void (*tpfNullCallback)(void);

#endif
/*lint -restore */
/** @} */
