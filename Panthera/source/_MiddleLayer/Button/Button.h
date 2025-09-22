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

#define TIMEOUT_RELEASE_BTN    1000
#define TIMEOUT_PRESSED_BTN	   2000
#define TIMEOUT_HOLD_BTN	  10000

typedef enum{
	ACTION_RELEASED	   = 0,
	ACTION_PRESSED	   = 1,
	ACTION_HOLD		   = 2,
	ACTION_HOLD_LONG   = 3
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
