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

void BTN_GetStatusButton(void)
{
	temp_btn_stt = 0x0000;
	if (BTN_Debouncing(&thisButtonAll.height_1, thisButtonAll.height_1.value, TIMEOUT_DEBOUNCE_BTN)) {
		temp_btn_stt |= BTN_STT_0;
	}
	
	if (BTN_Debouncing(&thisButtonAll.height_2, thisButtonAll.height_2.value, TIMEOUT_DEBOUNCE_BTN)) {
		temp_btn_stt |= BTN_STT_1;
	}
	
	if (BTN_Debouncing(&thisButtonAll.recliner_1, thisButtonAll.recliner_1.value, TIMEOUT_DEBOUNCE_BTN)) {
		temp_btn_stt |= BTN_STT_2;
	}
	
	if (BTN_Debouncing(&thisButtonAll.recliner_2, thisButtonAll.recliner_2.value, TIMEOUT_DEBOUNCE_BTN)) {
		temp_btn_stt |= BTN_STT_3;
	}
	
	if (BTN_Debouncing(&thisButtonAll.slider_1, thisButtonAll.slider_1.value, TIMEOUT_DEBOUNCE_BTN)) {
		temp_btn_stt |= BTN_STT_4;
	}
	
	if (BTN_Debouncing(&thisButtonAll.slider_2, thisButtonAll.slider_2.value, TIMEOUT_DEBOUNCE_BTN)) {
		temp_btn_stt |= BTN_STT_5;
	}
	
	if (BTN_Debouncing(&thisButtonAll.save, thisButtonAll.save.value, TIMEOUT_DEBOUNCE_BTN)) {
		temp_btn_stt |= BTN_STT_6;
	}
	
	if (BTN_Debouncing(&thisButtonAll.recall_1, thisButtonAll.recall_1.value, TIMEOUT_DEBOUNCE_BTN)) {
		temp_btn_stt |= BTN_STT_7;
	}

	if (BTN_Debouncing(&thisButtonAll.recall_2, thisButtonAll.recall_2.value, TIMEOUT_DEBOUNCE_BTN)) {
		temp_btn_stt |= BTN_STT_8;
	}

	if (BTN_Debouncing(&thisButtonAll.recall_3, thisButtonAll.recall_3.value, TIMEOUT_DEBOUNCE_BTN)) {
		temp_btn_stt |= BTN_STT_9;
	}
	thisButton.btn_stt = temp_btn_stt;
}

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

void BTN_Update(void)
{
	GPIO_Update();
	BTN_GetGpioValue(&thisGPIO);
	BTN_GetStatusButton();
	if (BTN_OnlyOneBtnPress()) {
		BTN_AnalyzeInputState(&thisButton);
	} else {
		thisButton.name 	  = BTN_NA;
		thisButton.action 	  = ACTION_NA;
		thisButton.cnt_action = 0;
		thisButton.btn_stt    = 0x0000;
	}
}
