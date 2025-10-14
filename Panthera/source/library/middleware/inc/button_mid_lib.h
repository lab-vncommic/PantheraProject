/*
 * button_mid_lib.h
 *
 *  Created on: Oct 14, 2025
 *      Author: ADMIN
 */

#ifndef SOURCE_LIBRARY_MIDDLEWARE_INC_BUTTON_MID_LIB_H_
#define SOURCE_LIBRARY_MIDDLEWARE_INC_BUTTON_MID_LIB_H_
#include "stdint.h"

#define TIMEOUT_RELEASE_BTN    1000
#define TIMEOUT_PRESSED_BTN	   2000
#define TIMEOUT_HOLD_BTN	  10000

typedef enum{
	BTN_ACTION_RELEASE     = 0,
	BTN_ACTION_PRESSED	   = 1,
	BTN_ACTION_HOLD		   = 2,
	BTN_ACTION_HOLD_LONG   = 3
} btn_action_e;

typedef struct {
	uint8_t				value;
	btn_action_e		action;
	uint32_t			cnt_debounce;
} btn_obj_t;

void btn_analyze(btn_obj_t *p_button);

#endif /* SOURCE_LIBRARY_MIDDLEWARE_INC_BUTTON_MID_LIB_H_ */
