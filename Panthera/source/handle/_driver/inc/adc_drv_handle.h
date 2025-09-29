/*
 * adc_drv_hanlde.h
 *
 *  Created on: Sep 29, 2025
 *      Author: ADMIN
 */

#ifndef SOURCE_HANDLE__DRIVER_INC_ADC_DRV_HANDLE_H_
#define SOURCE_HANDLE__DRIVER_INC_ADC_DRV_HANDLE_H_
#include "adc_drv_interface.h"
#include "saradc_lld_cfg.h"
#include "drv_config.h"

void adc_init(void);
void adc_update_to_meas(adc_value_t *p_adc_value);

#endif /* SOURCE_HANDLE__DRIVER_INC_ADC_DRV_HANDLE_H_ */
