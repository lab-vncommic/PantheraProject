/*
 * Button.h
 *
 *  Created on: Jul 28, 2025
 *      Author: ADMIN
 */

#ifndef SOURCE__MIDDLELAYER_BUTTON_BUTTON_H_
#define SOURCE__MIDDLELAYER_BUTTON_BUTTON_H_
#include "gpio.h"

#define TIMEOUT_DEBOUNCE_BTN   500    // 50ms
#define TIMEOUT_PRESSED_BTN	   1000   // 100ms

// Button status bit definitions (16-bit)
#define BTN_STT_0     ((uint16_t) 1U<<0)  // BT_HEIGHT_1 - Height up button
#define BTN_STT_1     ((uint16_t) 1U<<1)  // BT_HEIGHT_2 - Height down button
#define BTN_STT_2     ((uint16_t) 1U<<2)  // BT_RECLINER_1 - Recliner forward button
#define BTN_STT_3     ((uint16_t) 1U<<3)  // BT_RECLINER_2 - Recliner backward button
#define BTN_STT_4     ((uint16_t) 1U<<4)  // BT_SLIDER_1 - Slider forward button
#define BTN_STT_5     ((uint16_t) 1U<<5)  // BT_SLIDER_2 - Slider backward button
#define BTN_STT_6     ((uint16_t) 1U<<6)  // BT_SAVE - Save position button
#define BTN_STT_7     ((uint16_t) 1U<<7)  // BT_RECALL_1 - Recall position 1 button
#define BTN_STT_8     ((uint16_t) 1U<<8)  // BT_RECALL_2 - Recall position 2 button
#define BTN_STT_9     ((uint16_t) 1U<<9)  // BT_RECALL_3 - Recall position 3 button
#define BTN_STT_10    ((uint16_t) 1U<<10) // Reserved for future use
#define BTN_STT_11    ((uint16_t) 1U<<11) // Reserved for future use
#define BTN_STT_12    ((uint16_t) 1U<<12) // Reserved for future use
#define BTN_STT_13    ((uint16_t) 1U<<13) // Reserved for future use
#define BTN_STT_14    ((uint16_t) 1U<<14) // Reserved for future use
#define BTN_STT_15    ((uint16_t) 1U<<15) // Reserved for future use

/*===========================================================================
 * REUSABLE CODE SECTION
 *===========================================================================*/
typedef enum{
	ACTION_NA,
	ACTION_PRESSED,
	ACTION_HOLD
} Btn_Action_TypeDef;

/*===========================================================================
 * PTR CODE SECTION
 *===========================================================================*/
typedef enum{
	BTN_NA = 0,
	BTN_HEIGHT_1,
	BTN_HEIGHT_2,
	BTN_RECLINER_1,
	BTN_RECLINER_2,
	BTN_SLIDER_1,
	BTN_SLIDER_2,
	BTN_SAVE,
	BTN_RECALL_1,
	BTN_RECALL_2,
	BTN_RECALL_3
} Btn_Name_TypeDef;

typedef struct {
	uint16_t	value;
	uint16_t	cnt_debounce;
} Btn_Object_TypeDef;

typedef struct {
	Btn_Object_TypeDef recliner_1;
	Btn_Object_TypeDef recliner_2;
	Btn_Object_TypeDef height_1;
	Btn_Object_TypeDef height_2;
	Btn_Object_TypeDef slider_1;
	Btn_Object_TypeDef slider_2;
    Btn_Object_TypeDef save;
    Btn_Object_TypeDef recall_1;
    Btn_Object_TypeDef recall_2;
    Btn_Object_TypeDef recall_3;
} Btn_All_TypeDef;

typedef struct {
	Btn_Name_TypeDef 	name;
	Btn_Action_TypeDef 	action;

	uint32_t 			cnt_action;
	uint16_t			btn_stt;
} Button_TypeDef;

/*===========================================================================
 * REUSABLE CODE SECTION
 *===========================================================================*/
void BTN_GetGpioValue(GPIO_Input_TypeDef *pGpio);
_Bool BTN_Debouncing(Btn_Object_TypeDef *pButton, _Bool condition, uint16_t timeout);

/*===========================================================================
 * PTR CODE SECTION
 *===========================================================================*/
void BTN_Init(void);
void BTN_GetStatusButton(void);
_Bool BTN_IsMultiButtonPress(void);
void BTN_AnalyzeInputState(Button_TypeDef *pButton);
void BTN_Update(void);

extern Button_TypeDef		thisButton;
#endif /* SOURCE__MIDDLELAYER_BUTTON_BUTTON_H_ */
