/*
 * Button.h
 *
 *  Created on: Jul 28, 2025
 *      Author: ADMIN
 */

#ifndef SOURCE__MIDDLELAYER_BUTTON_BUTTON_H_
#define SOURCE__MIDDLELAYER_BUTTON_BUTTON_H_
#include "gpio.h"
#include "stdbool.h"

#define TIMEOUT_DEBOUNCE_BTN   5000    // 0.5s
#define TIMEOUT_PRESSED_BTN	   7500   // 1s

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

typedef enum{
	ACTION_RELEASED	= 0,
	ACTION_PRESSED	= 1,
	ACTION_HOLD		= 2
} Btn_Action_TypeDef;

typedef enum{
	BTN_NA 			= 0,
	BTN_HEIGHT_1	= 1,
	BTN_HEIGHT_2	= 2,
	BTN_RECLINER_1	= 3,
	BTN_RECLINER_2	= 4,
	BTN_SLIDER_1	= 5,
	BTN_SLIDER_2	= 6,
	BTN_SAVE		= 7,
	BTN_RECALL_1	= 8,
	BTN_RECALL_2	= 9,
	BTN_RECALL_3	= 10
} Btn_Name_TypeDef;

typedef struct {
	Btn_Name_TypeDef	name;
	Btn_Action_TypeDef	action;

	uint16_t			value;
	uint32_t			cnt_debounce;
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

typedef struct{
	Btn_Name_TypeDef		name;
	Btn_Action_TypeDef		action;
} Button_TypeDef;

typedef struct{
	Button_TypeDef recliner_1;
	Button_TypeDef recliner_2;
	Button_TypeDef height_1;
	Button_TypeDef height_2;
	Button_TypeDef slider_1;
	Button_TypeDef slider_2;
	Button_TypeDef save;
    Button_TypeDef recall_1;
    Button_TypeDef recall_2;
    Button_TypeDef recall_3;
} ButtonSystem_TypeDef;

typedef struct{
	uint16_t recliner_1_value;
	uint16_t recliner_2_value;
	uint16_t height_1_value;
	uint16_t height_2_value;
	uint16_t slider_1_value;
	uint16_t slider_2_value;
	uint16_t save_value;
	uint16_t recall_1_value;
    uint16_t recall_2_value;
    uint16_t recall_3_value;
} ButtonComm_TypeDef;

/*===========================================================================
 * Public API
 *===========================================================================*/
void BTN_Init(void);
void BTN_Update(ButtonSystem_TypeDef *pButton);
void BTN_UpdateToComm(ButtonComm_TypeDef *pButton);

extern Button_TypeDef		thisButton;
extern Btn_All_TypeDef		thisButtonAll;
#endif /* SOURCE__MIDDLELAYER_BUTTON_BUTTON_H_ */
