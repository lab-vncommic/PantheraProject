/*
 * gpio.c
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */
#include "gpio.h"

/**
 * @brief Sets a GPIO pin high
 * @details Sets the specified GPIO pin to high state by calling the platform abstraction layer function
 * @param[in] ppin Pointer to GPIO pin definition structure containing port and pin information
 * @return None
 */
void GPIO_SetPin(GPIO_TypeDef *ppin)
{
	// pal_setpad(ppin->port, ppin->pin);
	ppin->value = 1; // Update the value to high
}

/**
 * @brief Clears a GPIO pin (sets it low)
 * @details Sets the specified GPIO pin to low state by calling the platform abstraction layer function
 * @param[in] ppin Pointer to GPIO pin definition structure containing port and pin information
 * @return None
 */
void GPIO_ClearPin(GPIO_TypeDef *ppin)
{
	// pal_clearpad(ppin->port, ppin->pin);
	ppin->value = 0; // Update the value to low
}

/**
 * @brief Toggles the state of a GPIO pin
 * @details Inverts the current state of the specified GPIO pin by calling the platform abstraction layer function
 * @param[in] ppin Pointer to GPIO pin definition structure containing port and pin information
 * @return None
 */
void GPIO_TogglePin(GPIO_TypeDef *ppin)
{
	// pal_togglepad(ppin->port, ppin->pin);
	ppin->value = !ppin->value; // Toggle the value
}

/**
 * @brief Reads the state of a GPIO pin
 * @details Reads and returns the current state of the specified GPIO pin using the platform abstraction layer function
 * @param[in] ppin Pointer to GPIO pin definition structure containing port and pin information
 * @return uint8_t The current state of the pin (0 = low, 1 = high)
 */
uint8_t GPIO_ReadPin(GPIO_TypeDef *ppin)
{
	// return pal_readpad(ppin->port, ppin->pin);
	return ppin->value; // Return the current value of the pin
}
