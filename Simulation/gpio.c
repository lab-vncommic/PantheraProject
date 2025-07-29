/*
 * gpio.c
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */
#include "gpio.h"
GPIO_Input_TypeDef thisGPIO;

/**
 * @brief Sets a GPIO pin high
 * @details Sets the specified GPIO pin to high state by calling the platform abstraction layer function
 * @param[in] pPin Pointer to GPIO pin definition structure containing port and pin information
 * @return None
 */
void GPIO_SetPin(GPIO_TypeDef *pPin)
{
	// pal_setpad(pPin->port, pPin->pin);
}

/**
 * @brief Clears a GPIO pin (sets it low)
 * @details Sets the specified GPIO pin to low state by calling the platform abstraction layer function
 * @param[in] pPin Pointer to GPIO pin definition structure containing port and pin information
 * @return None
 */
void GPIO_ClearPin(GPIO_TypeDef *pPin)
{
	// pal_clearpad(pPin->port, pPin->pin);
}

/**
 * @brief Toggles the state of a GPIO pin
 * @details Inverts the current state of the specified GPIO pin by calling the platform abstraction layer function
 * @param[in] pPin Pointer to GPIO pin definition structure containing port and pin information
 * @return None
 */
void GPIO_TogglePin(GPIO_TypeDef *pPin)
{
	// pal_togglepad(pPin->port, pPin->pin);
}

/**
 * @brief Reads the state of a GPIO pin
 * @details Reads and returns the current state of the specified GPIO pin using the platform abstraction layer function
 * @param[in] pPin Pointer to GPIO pin definition structure containing port and pin information
 * @return uint8_t The current state of the pin (0 = low, 1 = high)
 */
uint8_t GPIO_ReadPin(GPIO_TypeDef *pPin)
{
	// return pal_readpad(pPin->port, pPin->pin);
	return pPin->value; // Assuming value is updated elsewhere
}

void GPIO_Init(void)
{
	// thisGPIO.height_1   = (GPIO_TypeDef){PORT_HIEGHT_1, HIEGHT_1, 0};
	// thisGPIO.height_2   = (GPIO_TypeDef){PORT_HIEGHT_2, HIEGHT_2, 0};
	// thisGPIO.recall_1  	= (GPIO_TypeDef){PORT_RECALL_1, RECALL_1, 0};
	// thisGPIO.recall_2   = (GPIO_TypeDef){PORT_RECALL_2, RECALL_2, 0};
	// thisGPIO.recall_3   = (GPIO_TypeDef){PORT_RECALL_3, RECALL_3, 0};
	// thisGPIO.recliner_1 = (GPIO_TypeDef){PORT_RECLIER_1, RECLIER_1, 0};
	// thisGPIO.recliner_2 = (GPIO_TypeDef){PORT_RECLIER_2, RECLIER_2, 0};
	// thisGPIO.slider_1   = (GPIO_TypeDef){PORT_SLIDER_1, SLIDER_1, 0};
	// thisGPIO.slider_2  	= (GPIO_TypeDef){PORT_SLIDER_2, SLIDER_2, 0};
	// thisGPIO.save 	  	= (GPIO_TypeDef){PORT_SAVE, SAVE, 0};
	// thisGPIO.hall_1     = (GPIO_TypeDef){PORT_HALL_1, HALL_1, 0};
	// thisGPIO.hall_2     = (GPIO_TypeDef){PORT_HALL_2, HALL_2, 0};
	// thisGPIO.hall_3     = (GPIO_TypeDef){PORT_HALL_3, HALL_3, 0};
}

void GPIO_Update(void)
{
	// thisGPIO.height_1.value     = GPIO_ReadPin(&thisGPIO.height_1);
	// thisGPIO.height_2.value     = GPIO_ReadPin(&thisGPIO.height_2);
	// thisGPIO.recall_1.value     = GPIO_ReadPin(&thisGPIO.recall_1);
	// thisGPIO.recall_2.value     = GPIO_ReadPin(&thisGPIO.recall_2);
	// thisGPIO.recall_3.value     = GPIO_ReadPin(&thisGPIO.recall_3);
	// thisGPIO.recliner_1.value   = GPIO_ReadPin(&thisGPIO.recliner_1);
	// thisGPIO.recliner_2.value   = GPIO_ReadPin(&thisGPIO.recliner_2);
	// thisGPIO.slider_1.value     = GPIO_ReadPin(&thisGPIO.slider_1);
	// thisGPIO.slider_2.value     = GPIO_ReadPin(&thisGPIO.slider_2);
	// thisGPIO.save.value         = GPIO_ReadPin(&thisGPIO.save);
	// thisGPIO.hall_1.value       = GPIO_ReadPin(&thisGPIO.hall_1);
	// thisGPIO.hall_2.value       = GPIO_ReadPin(&thisGPIO.hall_2);
	// thisGPIO.hall_3.value       = GPIO_ReadPin(&thisGPIO.hall_3);
}




