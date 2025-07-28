/*
 * Button.h
 *
 *  Created on: Jul 28, 2025
 *      Author: ADMIN
 */

#ifndef SOURCE__MIDDLELAYER_BUTTON_BUTTON_H_
#define SOURCE__MIDDLELAYER_BUTTON_BUTTON_H_
#include "gpio.h"

/*===========================================================================
 * PTR CODE SECTION
 *===========================================================================*/
typedef struct {
    GPIO_TypeDef recliner_1;
    GPIO_TypeDef recliner_2;
    GPIO_TypeDef height_1;
    GPIO_TypeDef height_2;
    GPIO_TypeDef slider_1;
    GPIO_TypeDef slider_2;
    GPIO_TypeDef save;
    GPIO_TypeDef recall_1;
    GPIO_TypeDef recall_2;
    GPIO_TypeDef recall_3;
} Button_TypeDef;

void BT_Init(void);
void BT_UpdateStatus(void);
void BT_ProcessState(void);

extern Button_TypeDef thisButton;
#endif /* SOURCE__MIDDLELAYER_BUTTON_BUTTON_H_ */
