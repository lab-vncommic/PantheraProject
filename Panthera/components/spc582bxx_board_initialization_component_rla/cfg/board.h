/****************************************************************************
*
* Copyright © 2017-2022 STMicroelectronics - All Rights Reserved
*
* This software is licensed under SLA0098 terms that can be found in the
* DM00779817_1_0.pdf file in the licenses directory of this software product.
* 
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED, 
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
*****************************************************************************/

#ifndef _BOARD_H_
#define _BOARD_H_

#include "pal.h"

/*
 * Setup for a generic SPC582Bxx board.
 */

/*
 * Board identifiers.
 */
#define BOARD_SPC582B_DISCOVERY
#define BOARD_NAME                  "STMicroelectronics SPC582B Discovery"

/*
 * Support macros.
 */
#define MSCR_IO_INDEX(port, pin)    (((port) * 16U) + (pin))

/*
 * I/O definitions.
 */
#define CAN_RX                      1U
#define MulEN_2                     10U
#define SEL0_2                      11U
#define CAN_TX                      2U
#define MulSens_2                   10U
#define INB_2                       11U
#define MulSens_1                   8U
#define INA_2                       9U
#define MulEN_3                     1U
#define PWM_2                       13U
#define SEL1_1                      14U
#define SEL1_2                      15U
#define SEL0_3                      2U
#define INA_3                       3U
#define INB_3                       4U
#define Mulsens_3                   11U
#define PWM_1                       12U
#define SLIDER_1                    13U
#define RECLIER_1                   14U
#define RECLIER_2                   15U
#define INB_1                       5U
#define PWM_3                       10U
#define SEL1_3                      11U
#define SLIDER_2                    3U
#define MulEN_1                     3U
#define FB_5V                       12U
#define SEL0_1                      4U
#define HIEGHT_1                    1U
#define HIEGHT_2                    2U
#define RECALL_3                    3U
#define RECALL_2                    4U
#define SAVE                        6U
#define RECALL_1                    7U
#define INA_1                       14U

/*
 * PORT definitions.
 */
#define PORT_CAN_RX                 PORT_A
#define PORT_MulEN_2                PORT_A
#define PORT_SEL0_2                 PORT_A
#define PORT_CAN_TX                 PORT_A
#define PORT_MulSens_2              PORT_B
#define PORT_INB_2                  PORT_B
#define PORT_MulSens_1              PORT_B
#define PORT_INA_2                  PORT_B
#define PORT_MulEN_3                PORT_C
#define PORT_PWM_2                  PORT_C
#define PORT_SEL1_1                 PORT_C
#define PORT_SEL1_2                 PORT_C
#define PORT_SEL0_3                 PORT_C
#define PORT_INA_3                  PORT_C
#define PORT_INB_3                  PORT_C
#define PORT_Mulsens_3              PORT_D
#define PORT_PWM_1                  PORT_D
#define PORT_SLIDER_1               PORT_D
#define PORT_RECLIER_1              PORT_D
#define PORT_RECLIER_2              PORT_D
#define PORT_INB_1                  PORT_D
#define PORT_PWM_3                  PORT_E
#define PORT_SEL1_3                 PORT_E
#define PORT_SLIDER_2               PORT_E
#define PORT_MulEN_1                PORT_F
#define PORT_FB_5V                  PORT_G
#define PORT_SEL0_1                 PORT_H
#define PORT_HIEGHT_1               PORT_I
#define PORT_HIEGHT_2               PORT_I
#define PORT_RECALL_3               PORT_I
#define PORT_RECALL_2               PORT_I
#define PORT_SAVE                   PORT_I
#define PORT_RECALL_1               PORT_I
#define PORT_INA_1                  PORT_M

/*
 * MSCR_IO definitions.
 */
#define MSCR_IO_CAN_RX              MSCR_IO_INDEX(PORT_CAN_RX, CAN_RX)
#define MSCR_IO_MulEN_2             MSCR_IO_INDEX(PORT_MulEN_2, MulEN_2)
#define MSCR_IO_SEL0_2              MSCR_IO_INDEX(PORT_SEL0_2, SEL0_2)
#define MSCR_IO_CAN_TX              MSCR_IO_INDEX(PORT_CAN_TX, CAN_TX)
#define MSCR_IO_MulSens_2           MSCR_IO_INDEX(PORT_MulSens_2, MulSens_2)
#define MSCR_IO_INB_2               MSCR_IO_INDEX(PORT_INB_2, INB_2)
#define MSCR_IO_MulSens_1           MSCR_IO_INDEX(PORT_MulSens_1, MulSens_1)
#define MSCR_IO_INA_2               MSCR_IO_INDEX(PORT_INA_2, INA_2)
#define MSCR_IO_MulEN_3             MSCR_IO_INDEX(PORT_MulEN_3, MulEN_3)
#define MSCR_IO_PWM_2               MSCR_IO_INDEX(PORT_PWM_2, PWM_2)
#define MSCR_IO_SEL1_1              MSCR_IO_INDEX(PORT_SEL1_1, SEL1_1)
#define MSCR_IO_SEL1_2              MSCR_IO_INDEX(PORT_SEL1_2, SEL1_2)
#define MSCR_IO_SEL0_3              MSCR_IO_INDEX(PORT_SEL0_3, SEL0_3)
#define MSCR_IO_INA_3               MSCR_IO_INDEX(PORT_INA_3, INA_3)
#define MSCR_IO_INB_3               MSCR_IO_INDEX(PORT_INB_3, INB_3)
#define MSCR_IO_Mulsens_3           MSCR_IO_INDEX(PORT_Mulsens_3, Mulsens_3)
#define MSCR_IO_PWM_1               MSCR_IO_INDEX(PORT_PWM_1, PWM_1)
#define MSCR_IO_SLIDER_1            MSCR_IO_INDEX(PORT_SLIDER_1, SLIDER_1)
#define MSCR_IO_RECLIER_1           MSCR_IO_INDEX(PORT_RECLIER_1, RECLIER_1)
#define MSCR_IO_RECLIER_2           MSCR_IO_INDEX(PORT_RECLIER_2, RECLIER_2)
#define MSCR_IO_INB_1               MSCR_IO_INDEX(PORT_INB_1, INB_1)
#define MSCR_IO_PWM_3               MSCR_IO_INDEX(PORT_PWM_3, PWM_3)
#define MSCR_IO_SEL1_3              MSCR_IO_INDEX(PORT_SEL1_3, SEL1_3)
#define MSCR_IO_SLIDER_2            MSCR_IO_INDEX(PORT_SLIDER_2, SLIDER_2)
#define MSCR_IO_MulEN_1             MSCR_IO_INDEX(PORT_MulEN_1, MulEN_1)
#define MSCR_IO_FB_5V               MSCR_IO_INDEX(PORT_FB_5V, FB_5V)
#define MSCR_IO_SEL0_1              MSCR_IO_INDEX(PORT_SEL0_1, SEL0_1)
#define MSCR_IO_HIEGHT_1            MSCR_IO_INDEX(PORT_HIEGHT_1, HIEGHT_1)
#define MSCR_IO_HIEGHT_2            MSCR_IO_INDEX(PORT_HIEGHT_2, HIEGHT_2)
#define MSCR_IO_RECALL_3            MSCR_IO_INDEX(PORT_RECALL_3, RECALL_3)
#define MSCR_IO_RECALL_2            MSCR_IO_INDEX(PORT_RECALL_2, RECALL_2)
#define MSCR_IO_SAVE                MSCR_IO_INDEX(PORT_SAVE, SAVE)
#define MSCR_IO_RECALL_1            MSCR_IO_INDEX(PORT_RECALL_1, RECALL_1)
#define MSCR_IO_INA_1               MSCR_IO_INDEX(PORT_INA_1, INA_1)

/*
 * MSCR_MUX definitions.
 */
#define MSCR_MUX_CAN_RX             337U

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
