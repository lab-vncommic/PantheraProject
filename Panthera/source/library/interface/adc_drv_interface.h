/*
 * adc_drv_if.h
 *
 *  Created on: Sep 29, 2025
 *      Author: ADMIN
 */

#ifndef SOURCE_PANTHERA__DRIVER_INC_ADC_DRV_INTERFACE_H_
#define SOURCE_PANTHERA__DRIVER_INC_ADC_DRV_INTERFACE_H_
#include "_drv_config.h"

typedef struct {

} adc_hw_cfg_t;

typedef struct {
	adc_value_t		value;
	adc_hw_cfg_t	hw_cfg;
} adc_ctx_t;

typedef struct {
	adc_ctx_t	ctx;

	void(*config)(adc_ctx_t *ctx);
	void(*update)(adc_ctx_t *ctx);
} adc_obj_t;

void adc_create_obj(adc_obj_t *p_adc_obj);

#endif /* SOURCE_PANTHERA__DRIVER_INC_ADC_DRV_INTERFACE_H_ */
