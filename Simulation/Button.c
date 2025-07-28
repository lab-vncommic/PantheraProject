/*
 * Button.c
 *
 *  Created on: Jul 28, 2025
 *      Author: ADMIN
 */
#include "Button.h"
Button_TypeDef thisButton;

/**
 * @brief  Initializes all buttons in the system
 * @details Sets up the GPIO configuration for all buttons including:
 *          - Height adjustment buttons (up/down)
 *          - Recall memory buttons (1/2/3)
 *          - Recliner adjustment buttons
 *          - Slider adjustment buttons
 *          - Save button
 * @note   Must be called before any other button functions
 */
void BT_Init(void)
{
	// thisButton.height_1   = (GPIO_TypeDef){PORT_HIEGHT_1, HIEGHT_1, 0};
	// thisButton.height_2   = (GPIO_TypeDef){PORT_HIEGHT_2, HIEGHT_2, 0};
	// thisButton.recall_1   = (GPIO_TypeDef){PORT_RECALL_1, RECALL_1, 0};
	// thisButton.recall_2   = (GPIO_TypeDef){PORT_RECALL_2, RECALL_2, 0};
	// thisButton.recall_3   = (GPIO_TypeDef){PORT_RECALL_3, RECALL_3, 0};
	// thisButton.recliner_1 = (GPIO_TypeDef){PORT_RECLIER_1, RECLIER_1, 0};
	// thisButton.recliner_2 = (GPIO_TypeDef){PORT_RECLIER_2, RECLIER_2, 0};
	// thisButton.slider_1   = (GPIO_TypeDef){PORT_SLIDER_1, SLIDER_1, 0};
	// thisButton.slider_2   = (GPIO_TypeDef){PORT_SLIDER_2, SLIDER_2, 0};
	// thisButton.save 	   = (GPIO_TypeDef){PORT_SAVE, SAVE, 0};
}

/**
 * @brief  Updates the current status of all buttons
 * @details Reads the physical state of all GPIO pins connected to buttons
 *          and updates the internal button status structure
 * @note   Should be called periodically to maintain current button states
 */
void BT_UpdateStatus(void)
{
	thisButton.height_1.value 	= GPIO_ReadPin(&thisButton.height_1);
	thisButton.height_2.value 	= GPIO_ReadPin(&thisButton.height_2);
	thisButton.recall_1.value 	= GPIO_ReadPin(&thisButton.recall_1);
	thisButton.recall_2.value 	= GPIO_ReadPin(&thisButton.recall_2);
	thisButton.recall_3.value 	= GPIO_ReadPin(&thisButton.recall_3);
	// thisButton.recliner_1.value = GPIO_ReadPin(&thisButton.recliner_1);
	// thisButton.recliner_2.value = GPIO_ReadPin(&thisButton.recliner_2);
	// thisButton.slider_1.value 	= GPIO_ReadPin(&thisButton.slider_1);
	// thisButton.slider_2.value 	= GPIO_ReadPin(&thisButton.slider_2);
	thisButton.save.value 		= GPIO_ReadPin(&thisButton.save);
}

/**
 * @brief  Processes the button states and triggers corresponding actions
 * @details Analyzes the current button states and executes appropriate 
 *          system responses based on which buttons are pressed
 * @note   This function is currently empty and needs to be implemented
 */
void BT_ProcessState(void)
{

}
