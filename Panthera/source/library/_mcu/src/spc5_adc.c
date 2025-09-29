/*
 * spc5_adc.c
 *
 *  Created on: Sep 29, 2025
 *      Author: ADMIN
 */
#include "spc5_adc.h"

void spc5_adc_config(adc_ctx_t *ctx);
void spc5_adc_soc(adc_ctx_t *ctx);

void adc_create_obj(adc_obj_t *p_adc_obj)
{
	p_adc_obj->config = spc5_adc_config;
	p_adc_obj->update = spc5_adc_soc;
}

void spc5_adc_config(adc_ctx_t *ctx)
{
	(void)ctx;
	/* Start SARADC Driver */
	saradc_lld_start(&SARADC12DSV, &saradc_config_saradcconf);

	 // Align left ADC value
	SARADC12DSV.saradc->MCR.B.WLSIDE = 1U;
}

void spc5_adc_soc(adc_ctx_t *ctx)
{
	(void)ctx;
	saradc_lld_start_conversion(&SARADC12DSV);
}
