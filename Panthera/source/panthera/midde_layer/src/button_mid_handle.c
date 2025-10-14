/*
 * button_mid_handle.c
 *
 *  Created on: Oct 13, 2025
 *      Author: ADMIN
 */
#include "button_mid_handle.h"
btn_obj_t button_inst[NUMER_OF_BUTTON];

void btn_init(void)
{

}

void btn_update_gpio_status(void)
{
	button_inst[BTN_RECLINER_1_ID].value = gpio_read_pin(GPIO_RECLINER_1);
	button_inst[BTN_RECLINER_2_ID].value = gpio_read_pin(GPIO_RECLINER_2);
	button_inst[BTN_SLIDER_1_ID].value   = gpio_read_pin(GPIO_SLIDER_1);
	button_inst[BTN_SLIDER_2_ID].value   = gpio_read_pin(GPIO_SLIDER_2);
	button_inst[BTN_HEIGHT_1_ID].value   = gpio_read_pin(GPIO_HEIGHT_1);
	button_inst[BTN_HEIGHT_2_ID].value   = gpio_read_pin(GPIO_HEIGHT_2);
	button_inst[BTN_SAVE_ID].value       = gpio_read_pin(GPIO_SAVE);
	button_inst[BTN_RECALL_1_ID].value   = gpio_read_pin(GPIO_RECALL_1);
	button_inst[BTN_RECALL_2_ID].value   = gpio_read_pin(GPIO_RECALL_2);
	button_inst[BTN_RECALL_3_ID].value   = gpio_read_pin(GPIO_RECALL_3);
}

void btn_update_to_comm()
{

}
