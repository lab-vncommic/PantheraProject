/*
 * gpio.c
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */
#include "gpio.h"

void GPIO_SetPin(GPIO_PinDef *ppin)
{
	pal_setpad(ppin->Port, ppin->Pin);
}

void GPIO_ClearPin(GPIO_PinDef *ppin)
{
	pal_clearpad(ppin->Port, ppin->Pin);
}

void GPIO_TogglePin(GPIO_PinDef *ppin)
{
	pal_togglepad(ppin->Port, ppin->Pin);
}

uint8_t GPIO_ReadPin(GPIO_PinDef *ppin)
{
	return pal_readpad(ppin->Port, ppin->Pin);
}
