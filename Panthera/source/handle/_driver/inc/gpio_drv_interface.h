/*
 * gpio_drv_interface.h
 *
 *  Created on: Sep 29, 2025
 *      Author: Tracy McConnell
 */

#ifndef SOURCE_HANDLE__DRIVER_INC_GPIO_DRV_INTERFACE_H_
#define SOURCE_HANDLE__DRIVER_INC_GPIO_DRV_INTERFACE_H_
#include "drv_config.h"

typedef struct {

} gpio_hw_cfg_t;

typedef struct {
	gpio_hw_cfg_t	hw_cfg;
} gpio_ctx_t;

typedef struct {
	gpio_ctx_t	ctx;

	void(*set)(gpio_ctx_t *ctx);
	void(*clear)(gpio_ctx_t *ctx);
	void(*read)(gpio_ctx_t *ctx);
	void(*toggle)(gpio_ctx_t *ctx);
} gpio_obj_t;

void gpio_create_obj(gpio_obj_t *p_gpio_obj);

#endif /* SOURCE_HANDLE__DRIVER_INC_GPIO_DRV_INTERFACE_H_ */
