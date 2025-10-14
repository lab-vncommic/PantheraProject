/*
 * adc_drv_hanlde.c
 *
 *  Created on: Sep 29, 2025
 *      Author: ADMIN
 */
#include "adc_drv_handle.h"
adc_obj_t adc_inst;

void adc_init(void)
{
	adc_create_obj(&adc_inst);
	adc_inst.config(&adc_inst.ctx);

	adc_inst.ctx.value.adc_mulsens_1 = 0;
	adc_inst.ctx.value.adc_mulsens_2 = 0;
	adc_inst.ctx.value.adc_mulsens_3 = 0;
	adc_inst.ctx.value.adc_fb_5v	 = 0;
}

void adc_update(adc_obj_t *p_adc)
{
	p_adc->update(&p_adc->ctx);
}

void saradcconf_conv_cb(SARADCDriver *saradcp) {
    adc_inst.ctx.value.adc_mulsens_1 = saradc_lld_readchannel(saradcp, CHANNEL_MULSENS_1);
    adc_inst.ctx.value.adc_mulsens_2 = saradc_lld_readchannel(saradcp, CHANNEL_MULSENS_2);
    adc_inst.ctx.value.adc_mulsens_3 = saradc_lld_readchannel(saradcp, CHANNEL_MULSENS_3);
    adc_inst.ctx.value.adc_fb_5v 	 = saradc_lld_readchannel(saradcp, CHANNEL_FB_5V);
}

void adc_update_to_meas(adc_value_t *p_adc_value)
{
	adc_update(&adc_inst);

	p_adc_value->adc_mulsens_1 = adc_inst.ctx.value.adc_mulsens_1;
	p_adc_value->adc_mulsens_2 = adc_inst.ctx.value.adc_mulsens_2;
	p_adc_value->adc_mulsens_3 = adc_inst.ctx.value.adc_mulsens_3;
	p_adc_value->adc_fb_5v 	   = adc_inst.ctx.value.adc_fb_5v;
}
