/*
 * gpio_drv_handle.h
 *
 *  Created on: Sep 30, 2025
 *      Author: Tracy McConnell
 */

#ifndef SOURCE_PANTHERA_DRIVER_LAYER_INC_GPIO_DRV_HANDLE_H_
#define SOURCE_PANTHERA_DRIVER_LAYER_INC_GPIO_DRV_HANDLE_H_
#include "_drv_config.h"
#include "gpio_drv_interface.h"
#include "spc5_gpio.h"

void gpio_init(void);
void gpio_set_pin(gpio_id_e id);
void gpio_clear_pin(gpio_id_e id);
uint8_t gpio_read_pin(gpio_id_e id);
void gpio_toggle_pin(gpio_id_e id);
void gpio_update_status_button_pin(gpio_btn_value_t *p_button);

#endif /* SOURCE_PANTHERA_DRIVER_LAYER_INC_GPIO_DRV_HANDLE_H_ */
