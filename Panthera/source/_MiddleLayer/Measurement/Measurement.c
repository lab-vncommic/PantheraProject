/*
 * Measurement.c
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */
#include "Measurement.h"
MeasAll_TypeDef thisMeas;

void MEAS_Config(Meas_TypeDef *pMeas, q15_t offset, uq31_t scale_factor, uq15_t alpha, uq15_t high_limit_soft, float gain_f32, float offset_f32, uint8_t calc_method)
{
    if(pMeas == NULL)
        return;

    pMeas->raw 			   = 0U;
    pMeas->value_f32       = 0.0f;
    pMeas->value_q15	   = 0U;
    pMeas->offset 		   = offset;
    pMeas->scale_factor    = scale_factor;
    pMeas->alpha 		   = alpha;
    pMeas->high_limit_soft = high_limit_soft;
    pMeas->gain_f32        = gain_f32;
    pMeas->offset_f32	   = offset_f32;
    pMeas->calc_method	   = calc_method;
}

void MEAS_GetRawValue(Meas_TypeDef *pMeas, q15_t raw_value)
{
    if(pMeas == NULL)
        return;

    pMeas->raw = raw_value;
}

void MEAS_GetValueQ15(Meas_TypeDef *pMeas)
{
    if(pMeas == NULL)
    	return;
    if (pMeas->calc_method == CALC_METHOD_Q15) {
        pMeas->value_q15 = _q15_mul(pMeas->raw >> 1 , pMeas->scale_factor);
	} else pMeas->value_q15 = _q15_mul(pMeas->raw >>1 , _q15(1));
}

void MEAS_GetValueF32(Meas_TypeDef *pMeas)
{
    if(pMeas == NULL)
        return;

    if (pMeas->calc_method == CALC_METHOD_Q15) {
    	pMeas->value_f32 = _q15_i2f((pMeas->value_q15) * pMeas->high_limit_soft);
	} else pMeas->value_f32 = pMeas->raw * pMeas->gain_f32 + pMeas->offset_f32;
}

void MEAS_Init(void)
{
	// Config driver ADC
	ADC_Init(&thatADC);

	// Config parameters for measurement objects
    MEAS_Config(&thisMeas.current_mt1, CURRENT_MT1_OFFSET, CURRENT_MT1_SCALE_FACTOR, 0U, CURRENT_MT1_HIGH_LIMIT_SOFT, 0U, 0U, CALC_METHOD_Q15);
    MEAS_Config(&thisMeas.current_mt2, CURRENT_MT2_OFFSET, CURRENT_MT2_SCALE_FACTOR, 0U, CURRENT_MT2_HIGH_LIMIT_SOFT, 0U, 0U, CALC_METHOD_Q15);
    MEAS_Config(&thisMeas.current_mt3, CURRENT_MT3_OFFSET, CURRENT_MT3_SCALE_FACTOR, 0U, CURRENT_MT3_HIGH_LIMIT_SOFT, 0U, 0U, CALC_METHOD_Q15);
    MEAS_Config(&thisMeas.thermal_mt1, 0U, 0U, 0U, 0U, THERMAL_MT1_GAIN_F32, THERMAL_MT1_OFFSET_F32, CALC_METHOD_F32);
    MEAS_Config(&thisMeas.thermal_mt2, 0U, 0U, 0U, 0U, THERMAL_MT2_GAIN_F32, THERMAL_MT2_OFFSET_F32, CALC_METHOD_F32);
    MEAS_Config(&thisMeas.thermal_mt3, 0U, 0U, 0U, 0U, THERMAL_MT3_GAIN_F32, THERMAL_MT3_OFFSET_F32, CALC_METHOD_F32);
}

void MEAS_GetValue(void)
{
	// Get adc value
	ADC_startOfConversion();

	// Get raw value from adc driver
    MEAS_GetRawValue(&thisMeas.current_mt1, thatADC.ADC_MulSens_1);
    MEAS_GetRawValue(&thisMeas.current_mt2, thatADC.ADC_MulSens_2);
    MEAS_GetRawValue(&thisMeas.current_mt3, thatADC.ADC_MulSens_3);

	// Get value in Q15 concept
	MEAS_GetValueQ15(&thisMeas.current_mt1);
	MEAS_GetValueQ15(&thisMeas.current_mt2);
	MEAS_GetValueQ15(&thisMeas.current_mt3);
    MEAS_GetValueQ15(&thisMeas.thermal_mt1);
    MEAS_GetValueQ15(&thisMeas.thermal_mt2);
    MEAS_GetValueQ15(&thisMeas.thermal_mt3);
}

void MEAS_GetRealValue(void)
{
	MEAS_GetValueF32(&thisMeas.current_mt1);
	MEAS_GetValueF32(&thisMeas.current_mt2);
	MEAS_GetValueF32(&thisMeas.current_mt3);
    MEAS_GetValueF32(&thisMeas.thermal_mt1);
    MEAS_GetValueF32(&thisMeas.thermal_mt2);
    MEAS_GetValueF32(&thisMeas.thermal_mt3);
}



