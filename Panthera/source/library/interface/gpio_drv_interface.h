/*
 * gpio_drv_interface.h
 *
 *  Created on: Sep 29, 2025
 *      Author: Tracy McConnell
 */

#ifndef SOURCE_PANTHERA__DRIVER_INC_GPIO_DRV_INTERFACE_H_
#define SOURCE_PANTHERA__DRIVER_INC_GPIO_DRV_INTERFACE_H_
#include "_drv_config.h"

typedef struct {
	uint16_t port;
    uint16_t pin;
} gpio_hw_cfg_t;

typedef struct {
	gpio_id_e		id;
	gpio_hw_cfg_t	hw_cfg;
} gpio_ctx_t;

typedef struct {
	void(*set)(gpio_ctx_t *ctx);
	void(*clear)(gpio_ctx_t *ctx);
	uint8_t(*read)(gpio_ctx_t *ctx);
	void(*toggle)(gpio_ctx_t *ctx);
} gpio_method_t;

typedef struct {
	gpio_ctx_t		ctx;
	gpio_method_t	*method;
} gpio_obj_t;

void gpio_create_obj(gpio_obj_t *p_gpio_obj);

#endif /* SOURCE_PANTHERA__DRIVER_INC_GPIO_DRV_INTERFACE_H_ */
