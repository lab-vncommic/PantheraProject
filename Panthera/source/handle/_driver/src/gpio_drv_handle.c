/*
 * gpio_drv_handle.c
 *
 *  Created on: Sep 30, 2025
 *      Author: Tracy McConnell
 */
#include "gpio_drv_handle.h"
gpio_obj_t gpio_inst[10];

void gpio_init(void)
{
	for (int i = 0; i < 10; i++) {
		gpio_create_obj(&gpio_inst[i]);
	}
}

