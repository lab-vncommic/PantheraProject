/*
 * meas_mid_lib.h
 *
 *  Created on: Sep 28, 2025
 *      Author: Tracy McConnell
 */

#ifndef SOURCE_LIBRARY__MIDDLE_INC_MEAS_MID_LIB_H_
#define SOURCE_LIBRARY__MIDDLE_INC_MEAS_MID_LIB_H_
#include "IQMath.h"

#define CALC_METHOD_F32				0
#define CALC_METHOD_Q15				1

#define PI							3.14159f

typedef struct{
	// Parameters for Q15 concepts
	uq15_t 	raw;               // Raw ADC value, use this variable for debugging
	q15_t 	offset;            // Calibration offset
	uq31_t 	scale_factor;      // Scaling factor to convert to physical units
	uq15_t 	alpha;             // Low-pass filter coefficient
    float 	value_f32;         // Real value
    uq15_t 	value_q15;		   // Normalized value in Q15
    float 	high_limit_soft;

    // Parameters for F32 concpets
    float 	gain_f32;		   // Gain value to convert ADC to real value
    float 	offset_f32;		   // Offset value to convert ADC to real value
    uint8_t calc_method;	   // Calculate real value with direct method or Q15 method

    // Low pass filter
    uq15_t	prev_value;
} meas_obj_t;

void meas_config(meas_obj_t *pMeas, q15_t offset, uq31_t scale_factor, uq15_t alpha, uq15_t high_limit_soft, float gain_f32, float offset_f32, uint8_t calc_method);
void meas_get_value_q15(meas_obj_t *pMeas);
void meas_get_value_f32(meas_obj_t *pMeas);
uint16_t meas_lpf(uint16_t new_value, uint16_t prev_value, uint16_t alpha);

#endif /* SOURCE_LIBRARY__MIDDLE_INC_MEAS_MID_LIB_H_ */
