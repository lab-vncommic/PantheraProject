/*
 * button_mid_lib.c
 *
 *  Created on: Oct 14, 2025
 *      Author: ADMIN
 */
#include "button_mid_lib.h"

void btn_analyze(btn_obj_t *p_button)
{
	if(p_button->value == 1){
		p_button->cnt_debounce++;
		if (p_button->cnt_debounce < TIMEOUT_RELEASE_BTN ) {
			p_button->action = BTN_ACTION_RELEASE;
		} else if ((p_button->cnt_debounce >= TIMEOUT_RELEASE_BTN) && (p_button->cnt_debounce < TIMEOUT_PRESSED_BTN)) {
			p_button->action = BTN_ACTION_PRESSED;
		} else if ((p_button->cnt_debounce >= TIMEOUT_PRESSED_BTN) && (p_button->cnt_debounce < TIMEOUT_HOLD_BTN)) {
			p_button->action = BTN_ACTION_HOLD;
		} else p_button->action = BTN_ACTION_HOLD_LONG;
	} else {
		p_button->action 	  = BTN_ACTION_RELEASE;
		p_button->cnt_debounce = 0;
	}
}
