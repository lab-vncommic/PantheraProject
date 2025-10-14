/*
 * adc_drv_hanlde.h
 *
 *  Created on: Sep 29, 2025
 *      Author: ADMIN
 */

#ifndef SOURCE_PANTHERA_DRIVER_LAYER_INC_ADC_DRV_HANDLE_H_
#define SOURCE_PANTHERA_DRIVER_LAYER_INC_ADC_DRV_HANDLE_H_
#include "_drv_config.h"
#include "adc_drv_interface.h"
#include "spc5_adc.h"

void adc_init(void);
void adc_update_to_meas(adc_value_t *p_adc_value);

#endif /* SOURCE_PANTHERA_DRIVER_LAYER_INC_ADC_DRV_HANDLE_H_ */
