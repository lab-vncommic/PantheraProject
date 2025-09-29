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
#include "adc.h"


typedef struct{
	meas_obj_t current_slider;
	meas_obj_t current_height;
	meas_obj_t current_recliner;
	meas_obj_t thermal_slider;
	meas_obj_t thermal_height;
	meas_obj_t thermal_recliner;
} meas_t;

typedef struct{
	uq15_t current_slider_q15;
	uq15_t current_height_q15;
	uq15_t current_recliner_q15;
	uq15_t thermal_slider_q15;
	uq15_t thermal_height_q15;
	uq15_t thermal_recliner_q15;
} meas_to_system_t;

void meas_init(void);
void meas_update_value(meas_to_system_t *pMeas, MeasType_TypeDef meas_type);

#endif /* SOURCE_HANDLE__MIDDLE_INC_MEAS_MID_HANDLE_H_ */
