/*
 * Button.c
 *
 *  Created on: Jul 28, 2025
 *      Author: ADMIN
 */
#include "Button.h"
Btn_All_TypeDef		thisButtonAll;
Button_TypeDef		thisButton;
uint16_t			temp_btn_stt = 0x0000;

/**
 * @brief  Updates the current status of all buttons
 * @details Reads the physical state of all GPIO pins connected to buttons
 *          and updates the internal button status structure
 * @note   Should be called periodically to maintain current button states
 */


/**
 * @brief  Processes the button states and triggers corresponding actions
 * @details Analyzes the current button states and executes appropriate
 *          system responses based on which buttons are pressed
 * @note   This function is currently empty and needs to be implemented
 */
void BTN_GetGpioValue(GPIO_Input_TypeDef *pGpio)
{
	thisButtonAll.height_1.value 	= pGpio->height_1.value;
	thisButtonAll.height_2.value 	= pGpio->height_2.value;
	thisButtonAll.recliner_1.value 	= pGpio->recliner_1.value;
	thisButtonAll.recliner_2.value 	= pGpio->recliner_2.value;
	thisButtonAll.slider_1.value 	= pGpio->slider_1.value;
	thisButtonAll.slider_2.value 	= pGpio->slider_2.value;
	thisButtonAll.recall_1.value 	= pGpio->recall_1.value;
	thisButtonAll.recall_2.value 	= pGpio->recall_2.value;
	thisButtonAll.recall_3.value 	= pGpio->recall_3.value;
	thisButtonAll.save.value 		= pGpio->save.value;
}

_Bool BTN_Debouncing(Btn_Object_TypeDef *pButton, _Bool condition, uint16_t timeout)
{
	if (condition)
	{
		if (pButton->cnt_debounce++ > timeout)
		{
			pButton->cnt_debounce = 0;
			return 1;
		}
	} else	pButton->cnt_debounce = 0;
	return 0;
}

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
void BTN_Init(void)
{
	GPIO_Init();
	thisButton.name    = BTN_NA;
	thisButton.action  = ACTION_NA;
	thisButton.btn_stt = 0x0000;
	thisButton.cnt_action = 0;
}

/**
 * @brief  Gets the current status of all buttons
 * @details Updates the button status register (temp_btn_stt) based on the current
 *          state of each button input. Each button corresponds to a specific bit
 *          in the status register.
 * @note   This function performs direct GPIO reading without debouncing
 * @see    BTN_Update
 */
void BTN_GetStatusBtnReg(void)
{
//	if (BTN_Debouncing(&thisButtonAll.height_1, thisButtonAll.height_1.value, TIMEOUT_DEBOUNCE_BTN)) {
//		temp_btn_stt |= BTN_STT_0;
//	}
//
//	if (BTN_Debouncing(&thisButtonAll.height_2, thisButtonAll.height_2.value, TIMEOUT_DEBOUNCE_BTN)) {
//		temp_btn_stt |= BTN_STT_1;
//	}
//
//	if (BTN_Debouncing(&thisButtonAll.recliner_1, thisButtonAll.recliner_1.value, TIMEOUT_DEBOUNCE_BTN)) {
//		temp_btn_stt |= BTN_STT_2;
//	}
//
//	if (BTN_Debouncing(&thisButtonAll.recliner_2, thisButtonAll.recliner_2.value, TIMEOUT_DEBOUNCE_BTN)) {
//		temp_btn_stt |= BTN_STT_3;
//	}
//
//	if (BTN_Debouncing(&thisButtonAll.slider_1, thisButtonAll.slider_1.value, TIMEOUT_DEBOUNCE_BTN)) {
//		temp_btn_stt |= BTN_STT_4;
//	}
//
//	if (BTN_Debouncing(&thisButtonAll.slider_2, thisButtonAll.slider_2.value, TIMEOUT_DEBOUNCE_BTN)) {
//		temp_btn_stt |= BTN_STT_5;
//	}
//
//	if (BTN_Debouncing(&thisButtonAll.save, thisButtonAll.save.value, TIMEOUT_DEBOUNCE_BTN)) {
//		temp_btn_stt |= BTN_STT_6;
//	}
//
//	if (BTN_Debouncing(&thisButtonAll.recall_1, thisButtonAll.recall_1.value, TIMEOUT_DEBOUNCE_BTN)) {
//		temp_btn_stt |= BTN_STT_7;
//	}
//
//	if (BTN_Debouncing(&thisButtonAll.recall_2, thisButtonAll.recall_2.value, TIMEOUT_DEBOUNCE_BTN)) {
//		temp_btn_stt |= BTN_STT_8;
//	}
//
//	if (BTN_Debouncing(&thisButtonAll.recall_3, thisButtonAll.recall_3.value, TIMEOUT_DEBOUNCE_BTN)) {
//		temp_btn_stt |= BTN_STT_9;
//	}

	if (thisButtonAll.height_1.value) {
		temp_btn_stt |= BTN_STT_0;
	} else {
		temp_btn_stt &= ~BTN_STT_0;
	}
	
	if (thisButtonAll.height_2.value) {
		temp_btn_stt |= BTN_STT_1;
	} else {
		temp_btn_stt &= ~BTN_STT_1;
	}
	
	if (thisButtonAll.recliner_1.value == 1) {
		temp_btn_stt |= BTN_STT_2;
	} else {
		temp_btn_stt &= ~BTN_STT_2;
	}
	
	if (thisButtonAll.recliner_2.value == 1) {
		temp_btn_stt |= BTN_STT_3;
	} else {
		temp_btn_stt &= ~BTN_STT_3;
	}
	
	if (thisButtonAll.slider_1.value == 1) {
		temp_btn_stt |= BTN_STT_4;
	} else {
		temp_btn_stt &= ~BTN_STT_4;
	}
	
	if (thisButtonAll.slider_2.value == 1) {
		temp_btn_stt |= BTN_STT_5;
	} else {
		temp_btn_stt &= ~BTN_STT_5;
	}
	
	if (thisButtonAll.save.value == 1) {
		temp_btn_stt |= BTN_STT_6;
	} else {
		temp_btn_stt &= ~BTN_STT_6;
	}
	
	if (thisButtonAll.recall_1.value == 1) {
		temp_btn_stt |= BTN_STT_7;
	} else {
		temp_btn_stt &= ~BTN_STT_7;
	}

	if (thisButtonAll.recall_2.value == 1) {
		temp_btn_stt |= BTN_STT_8;
	} else {
		temp_btn_stt &= ~BTN_STT_8;
	}

	if (thisButtonAll.recall_3.value == 1) {
		temp_btn_stt |= BTN_STT_9;
	} else {
		temp_btn_stt &= ~BTN_STT_9;
	}
}

/**
 * @brief Checks if exactly one button is currently pressed
 *
 * This function checks the current button status register to determine
 * if exactly one button is being pressed.
 *
 * @return true if exactly one button is pressed
 * @return false if no buttons are pressed or if multiple buttons are pressed
 */
_Bool BTN_OnlyOneBtnPress(void)
{
    uint16_t buttonRegister = thisButton.btn_stt;
    if (buttonRegister == 0) {
        return false;
    }
    if ((buttonRegister & (buttonRegister - 1)) == 0) {
        return true;
    }
    return false;
}

/**
 * @brief  Analyzes the button input state to determine action and name
 * @param  pButton Pointer to a Button_TypeDef structure
 * @retval None
 *
 * @details This function determines the button action (PRESSED or HOLD) based on
 *          press duration and identifies the button name based on its state value.
 *          If the counter exceeds TIMEOUT_PRESSED_BTN, the action is set to HOLD,
 *          otherwise it's set to PRESSED. The button name is determined based on
 *          the bit position in the btn_stt field.
 */
void BTN_AnalyzeInputState(Button_TypeDef *pButton)
{
	// Analyze action of button
	if (pButton->cnt_action++ > TIMEOUT_PRESSED_BTN) {
		pButton->action = ACTION_HOLD;
	} else pButton->action = ACTION_PRESSED;

	// Analyze name of button
	switch (pButton->btn_stt) {
		case 1:
			pButton->name = BTN_HEIGHT_1;
			break;
		case 2:
			pButton->name = BTN_HEIGHT_2;
			break;
		case 4:
			pButton->name = BTN_RECLINER_1;
			break;
		case 8:
			pButton->name = BTN_RECLINER_2;
			break;
		case 16:
			pButton->name = BTN_SLIDER_1;
			break;
		case 32:
			pButton->name = BTN_SLIDER_2;
			break;
		case 64:
			pButton->name = BTN_SAVE;
			break;
		case 128:
			pButton->name = BTN_RECALL_1;
			break;
		case 256:
			pButton->name = BTN_RECALL_2;
			break;
		case 512:
			pButton->name = BTN_RECALL_3;
			break;
		default:
			pButton->name = BTN_NA;
			break;
	}
}

/**
 * @brief  Updates the button state and processes button input
 * @param  None
 * @retval None
 * 
 * @details This function performs the complete button update sequence:
 *          1. Updates GPIO values
 *          2. Gets button status from GPIO values
 *          3. If only one button is pressed, it analyzes the input state
 *          4. If multiple buttons are pressed or none, it resets button properties
 */
void BTN_Update(void)
{
	GPIO_Update();
	BTN_GetGpioValue(&thisGPIO);
	BTN_GetStatusBtnReg();
	if (BTN_OnlyOneBtnPress()) {
		BTN_AnalyzeInputState(&thisButton);
	} else {
		thisButton.name 	  = BTN_NA;
		thisButton.action 	  = ACTION_NA;
		thisButton.cnt_action = 0;
	}
}

ButtonState_TypeDef BTN_GetStatus(void)
{
	BTN_Update();
	ButtonState_TypeDef temp_button;
	temp_button.name = thisButton.name;
	temp_button.action 	 = thisButton.action;
	return temp_button;
}
