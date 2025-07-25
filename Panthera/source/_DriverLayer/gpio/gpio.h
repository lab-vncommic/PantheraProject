/*
 * gpio.h
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */

#ifndef DRIVERLAYER_GPIO_GPIO_H_
#define DRIVERLAYER_GPIO_GPIO_H_
#include "components.h"

#define LED3	((GPIO_PinDef){PORT_LED_3, LED_3})
#define LED4	((GPIO_PinDef){PORT_LED_4, LED_4})
#define LED5	((GPIO_PinDef){PORT_LED_5, LED_5})

typedef struct {
	uint16_t Port;
    uint16_t Pin;
} GPIO_PinDef;

void GPIO_SetPin(GPIO_PinDef *ppin);
void GPIO_ClearPin(GPIO_PinDef *ppin);
void GPIO_TogglePin(GPIO_PinDef *ppin);
uint8_t GPIO_ReadPin(GPIO_PinDef *ppin);
#endif /* DRIVERLAYER_GPIO_GPIO_H_ */
