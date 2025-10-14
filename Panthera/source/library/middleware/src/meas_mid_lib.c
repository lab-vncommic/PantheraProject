/*
 * meas_mid_lib.c
 *
 *  Created on: Sep 28, 2025
 *      Author: Tracy McConnell
 */
#include "../../middleware/inc/meas_mid_lib.h"

void meas_config(meas_obj_t *pMeas, q15_t offset, uq31_t scale_factor, uq15_t alpha, uq15_t high_limit_soft, float gain_f32, float offset_f32, uint8_t calc_method)
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

void meas_get_value_q15(meas_obj_t *pMeas)
{
    if(pMeas == NULL)
    	return;

    if (pMeas->calc_method == CALC_METHOD_Q15) {
//        pMeas->prev_value = pMeas->value_q15;
        pMeas->value_q15 = _q15_mul(pMeas->raw >> 1 , pMeas->scale_factor);
//        pMeas->value_q15 = MEAS_LPF(pMeas->value_q15, pMeas->prev_value, pMeas->alpha);
	} else pMeas->value_q15 = _q15_mul(pMeas->raw >>1 , _q15(1));
}

void meas_get_value_f32(meas_obj_t *pMeas)
{
    if(pMeas == NULL)
        return;

    if (pMeas->calc_method == CALC_METHOD_Q15) {
    	pMeas->value_f32 = _q15_i2f((pMeas->value_q15) * pMeas->high_limit_soft);
	} else pMeas->value_f32 = pMeas->raw * pMeas->gain_f32 + pMeas->offset_f32;
}


uint16_t meas_lpf(uint16_t new_value, uint16_t prev_value, uint16_t alpha)
{
    int32_t temp1 = ((int32_t)alpha * new_value) >> 15;
    int16_t one_minus_alpha = 32767 - alpha;
    int32_t temp2 = ((int32_t)one_minus_alpha * prev_value) >> 15;
    return (uint16_t)(temp1 + temp2);
}
