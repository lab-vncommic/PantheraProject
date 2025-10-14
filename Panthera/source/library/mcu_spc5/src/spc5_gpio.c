/*
 * spc5_gpio.c
 *
 *  Created on: Sep 30, 2025
 *      Author: Tracy McConnell
 */
#include "spc5_gpio.h"

void spc5_gpio_clear(gpio_ctx_t *ctx);
uint8_t spc5_gpio_read(gpio_ctx_t *ctx);
void spc5_gpio_set(gpio_ctx_t *ctx);
void spc5_gpio_toggle(gpio_ctx_t *ctx);

gpio_method_t gpio_std_method = {
		.clear  = spc5_gpio_clear,
		.read   = spc5_gpio_read,
		.set 	= spc5_gpio_set,
		.toggle = spc5_gpio_toggle
};

void gpio_create_obj(gpio_obj_t *p_gpio_obj)
{
	p_gpio_obj->method = &gpio_std_method;
}

void spc5_gpio_clear(gpio_ctx_t *ctx)
{
	pal_clearpad(ctx->hw_cfg.port, ctx->hw_cfg.pin);
}

uint8_t spc5_gpio_read(gpio_ctx_t *ctx)
{
	return pal_readpad(ctx->hw_cfg.port, ctx->hw_cfg.pin);
}

void spc5_gpio_set(gpio_ctx_t *ctx)
{
	pal_lld_setpad(ctx->hw_cfg.port, ctx->hw_cfg.pin);
}
void spc5_gpio_toggle(gpio_ctx_t *ctx)
{
	pal_togglepad(ctx->hw_cfg.port, ctx->hw_cfg.pin);
}
