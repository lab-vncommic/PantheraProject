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
    MEAS_Config(&thisMeas.current_mt1, MT1_CURRENT_OFFSET, MT1_CURRENT_SCALE_FACTOR, 0U, MT1_CURRENT_HIGH_LIMIT_SOFT, 0U, 0U, CALC_METHOD_Q15);
    MEAS_Config(&thisMeas.current_mt2, MT2_CURRENT_OFFSET, MT2_CURRENT_SCALE_FACTOR, 0U, MT2_CURRENT_HIGH_LIMIT_SOFT, 0U, 0U, CALC_METHOD_Q15);
    MEAS_Config(&thisMeas.current_mt3, MT3_CURRENT_OFFSET, MT3_CURRENT_SCALE_FACTOR, 0U, MT3_CURRENT_HIGH_LIMIT_SOFT, 0U, 0U, CALC_METHOD_Q15);
}

void MEAS_GetValue(void)
{
	// Get raw value from adc driver


	// Get value in Q15 concept
	MEAS_GetValueQ15(&thisMeas.current_mt1);
	MEAS_GetValueQ15(&thisMeas.current_mt2);
	MEAS_GetValueQ15(&thisMeas.current_mt3);
}

void MEAS_GetRealValue(void)
{
	MEAS_GetValueF32(&thisMeas.current_mt1);
	MEAS_GetValueF32(&thisMeas.current_mt2);
	MEAS_GetValueF32(&thisMeas.current_mt3);
}



