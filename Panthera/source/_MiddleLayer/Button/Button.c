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

void BTN_GetGpioValue(GPIO_Input_TypeDef *pGpio);

void BTN_InitBtn(Btn_Object_TypeDef *pButton, Btn_Name_TypeDef name);
void BTN_Analyze(Btn_Object_TypeDef *pButton);
void BTN_UpdateToState(Button_TypeDef *pButtonTemp, Btn_Object_TypeDef *pButton);


/**
 * @brief Updates the button values from GPIO inputs
 * @details Copies the current GPIO input values for all buttons to the 
 *          internal button state structure
 * @param[in] pGpio Pointer to the GPIO input structure containing current pin values
 * @return None
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
	BTN_InitBtn(&thisButtonAll.height_1, BTN_HEIGHT_1);
	BTN_InitBtn(&thisButtonAll.height_2, BTN_HEIGHT_2);
	BTN_InitBtn(&thisButtonAll.recliner_1, BTN_RECLINER_1);
	BTN_InitBtn(&thisButtonAll.recliner_2, BTN_RECLINER_2);
	BTN_InitBtn(&thisButtonAll.slider_1, BTN_SLIDER_1);
	BTN_InitBtn(&thisButtonAll.slider_2, BTN_SLIDER_2);
	BTN_InitBtn(&thisButtonAll.save, BTN_SAVE);
	BTN_InitBtn(&thisButtonAll.recall_1, BTN_RECALL_1);
	BTN_InitBtn(&thisButtonAll.recall_2, BTN_RECALL_2);
	BTN_InitBtn(&thisButtonAll.recall_3, BTN_RECALL_3);
}

void BTN_InitBtn(Btn_Object_TypeDef *pButton, Btn_Name_TypeDef name)
{
	pButton->name 		  = name;
	pButton->action		  = BTN_NA;
	pButton->value		  = 0;
	pButton->cnt_debounce = 0;
}

void BTN_Analyze(Btn_Object_TypeDef *pButton)
{
	if(pButton->value == 1){
		pButton->cnt_debounce++;
		if (pButton->cnt_debounce < TIMEOUT_DEBOUNCE_BTN ) {
			pButton->action = ACTION_NA;
		} else if ((pButton->cnt_debounce >= TIMEOUT_DEBOUNCE_BTN) && (pButton->cnt_debounce < TIMEOUT_PRESSED_BTN)) {
			pButton->action = ACTION_PRESSED;
		} else pButton->action = ACTION_HOLD;
	} else {
		pButton->action 	  = ACTION_NA;
		pButton->cnt_debounce = 0;
	}
}

void BTN_UpdateToState(Button_TypeDef *pButtonTemp, Btn_Object_TypeDef *pButton)
{
	pButtonTemp->name   = pButton->name;
	pButtonTemp->action = pButton->action;
}

ButtonSystem_TypeDef BTN_Update(void)
{
	ButtonSystem_TypeDef temp_button;
	
	// Update GPIO values
	GPIO_Update();
	BTN_GetGpioValue(&thisGPIO);
	
	// Analyze all buttons
	BTN_Analyze(&thisButtonAll.height_1);
	BTN_Analyze(&thisButtonAll.height_2);
	BTN_Analyze(&thisButtonAll.recliner_1);
	BTN_Analyze(&thisButtonAll.recliner_2);
	BTN_Analyze(&thisButtonAll.slider_1);
	BTN_Analyze(&thisButtonAll.slider_2);
	BTN_Analyze(&thisButtonAll.save);
	BTN_Analyze(&thisButtonAll.recall_1);
	BTN_Analyze(&thisButtonAll.recall_2);
	BTN_Analyze(&thisButtonAll.recall_3);

	BTN_UpdateToState(&temp_button.height_1, &thisButtonAll.height_1);
	BTN_UpdateToState(&temp_button.height_2, &thisButtonAll.height_2);
	BTN_UpdateToState(&temp_button.recliner_1, &thisButtonAll.recliner_1);
	BTN_UpdateToState(&temp_button.recliner_2, &thisButtonAll.recliner_2);
	BTN_UpdateToState(&temp_button.slider_1, &thisButtonAll.slider_1);
	BTN_UpdateToState(&temp_button.slider_2, &thisButtonAll.slider_2);
	BTN_UpdateToState(&temp_button.save, &thisButtonAll.save);
	BTN_UpdateToState(&temp_button.recall_1, &thisButtonAll.recall_1);
	BTN_UpdateToState(&temp_button.recall_2, &thisButtonAll.recall_2);
	BTN_UpdateToState(&temp_button.recall_3, &thisButtonAll.recall_3);

	return temp_button;
}
