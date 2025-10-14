/*
 * meas_mid_handle.c
 *
 *  Created on: Sep 28, 2025
 *      Author: Tracy McConnell
 */
#include "meas_mid_handle.h"
meas_t meas_inst;

void meas_init(void)
{
	// Config driver ADC
	adc_init();

	// Config parameters for measurement objects
    meas_config(&meas_inst.current_slider, CURRENT_MT1_OFFSET, CURRENT_MT1_SCALE_FACTOR, CURRENT_MT1_ALPHA, CURRENT_MT1_HIGH_LIMIT_SOFT, 0U, 0U, CALC_METHOD_Q15);
    meas_config(&meas_inst.current_height, CURRENT_MT2_OFFSET, CURRENT_MT2_SCALE_FACTOR, CURRENT_MT2_ALPHA, CURRENT_MT2_HIGH_LIMIT_SOFT, 0U, 0U, CALC_METHOD_Q15);
    meas_config(&meas_inst.current_recliner, CURRENT_MT3_OFFSET, CURRENT_MT3_SCALE_FACTOR, CURRENT_MT3_ALPHA, CURRENT_MT3_HIGH_LIMIT_SOFT, 0U, 0U, CALC_METHOD_Q15);
    meas_config(&meas_inst.thermal_slider, 0U, 0U, 0U, 0U, THERMAL_MT1_GAIN_F32, THERMAL_MT1_OFFSET_F32, CALC_METHOD_F32);
    meas_config(&meas_inst.thermal_height, 0U, 0U, 0U, 0U, THERMAL_MT2_GAIN_F32, THERMAL_MT2_OFFSET_F32, CALC_METHOD_F32);
    meas_config(&meas_inst.thermal_recliner, 0U, 0U, 0U, 0U, THERMAL_MT3_GAIN_F32, THERMAL_MT3_OFFSET_F32, CALC_METHOD_F32);
}

void meas_get_value(MeasType_TypeDef meas_type)
{
	adc_update_to_meas(&meas_inst.adc_raw_value);

	if (meas_type) {
		meas_inst.thermal_recliner.raw = meas_inst.adc_raw_value.adc_mulsens_1;
		meas_inst.thermal_slider.raw   = meas_inst.adc_raw_value.adc_mulsens_2;
		meas_inst.thermal_height.raw   = meas_inst.adc_raw_value.adc_mulsens_3;

		// Get value in Q15 concept
	    meas_get_value_q15(&meas_inst.thermal_slider);
	    meas_get_value_q15(&meas_inst.thermal_height);
	    meas_get_value_q15(&meas_inst.thermal_recliner);
	} else {
		meas_inst.current_recliner.raw = meas_inst.adc_raw_value.adc_mulsens_1;
		meas_inst.current_slider.raw   = meas_inst.adc_raw_value.adc_mulsens_2;
		meas_inst.current_height.raw   = meas_inst.adc_raw_value.adc_mulsens_3;

		// Get value in Q15 concept
		meas_get_value_q15(&meas_inst.current_slider);
		meas_get_value_q15(&meas_inst.current_height);
		meas_get_value_q15(&meas_inst.current_recliner);
	}

	// FIXME: Only use for debugging
//	MEAS_GetRealValue(meas_type);
}

void meas_get_real_value(MeasType_TypeDef meas_type)
{
	if (meas_type) {
	    meas_get_value_f32(&meas_inst.thermal_slider);
	    meas_get_value_f32(&meas_inst.thermal_height);
	    meas_get_value_f32(&meas_inst.thermal_recliner);
	} else {
		meas_get_value_f32(&meas_inst.current_slider);
		meas_get_value_f32(&meas_inst.current_height);
		meas_get_value_f32(&meas_inst.current_recliner);
	}
}

void meas_update_value_to_state(meas_sys_t *pMeas, MeasType_TypeDef meas_type)
{
	meas_get_value(meas_type);

	pMeas->current_slider_q15   = meas_inst.current_slider.value_q15;
	pMeas->current_height_q15   = meas_inst.current_height.value_q15;
	pMeas->current_recliner_q15 = meas_inst.current_recliner.value_q15;
	pMeas->thermal_slider_q15   = meas_inst.thermal_slider.value_q15;
	pMeas->thermal_height_q15   = meas_inst.thermal_height.value_q15;
	pMeas->thermal_recliner_q15 = meas_inst.thermal_recliner.value_q15;
}

