/*
 * meas_mid_handle.h
 *
 *  Created on: Sep 28, 2025
 *      Author: Tracy McConnell
 */

#ifndef SOURCE_HANDLE__MIDDLE_INC_MEAS_MID_HANDLE_H_
#define SOURCE_HANDLE__MIDDLE_INC_MEAS_MID_HANDLE_H_
#include "meas_mid_lib.h"
#include "mid_config.h"
#include "drv_config.h"
#include "adc_drv_handle.h"

typedef struct{
	uint16_t current_slider_q15;
	uint16_t current_height_q15;
	uint16_t current_recliner_q15;
	uint16_t thermal_slider_q15;
	uint16_t thermal_height_q15;
	uint16_t thermal_recliner_q15;
} meas_sys_t;

typedef struct{
	meas_obj_t current_slider;
	meas_obj_t current_height;
	meas_obj_t current_recliner;
	meas_obj_t thermal_slider;
	meas_obj_t thermal_height;
	meas_obj_t thermal_recliner;

	adc_value_t adc_raw_value;
} meas_t;

void meas_init(void);
void meas_update_value_to_state(meas_sys_t *pMeas, MeasType_TypeDef meas_type);

extern meas_t meas_inst;
#endif /* SOURCE_HANDLE__MIDDLE_INC_MEAS_MID_HANDLE_H_ */
