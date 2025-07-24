/*
 * Measurement.h
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */

#ifndef MIDDLELAYER_MEASUREMENT_MEASUREMENT_H_
#define MIDDLELAYER_MEASUREMENT_MEASUREMENT_H_
#include "components.h"
#include "IQMath.h"

/*===========================================================================
 * REUSABLE CODE SECTION
 *===========================================================================*/
#define CALC_METHOD_F32				0
#define CALC_METHOD_Q15				1

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
} Meas_TypeDef;

/*===========================================================================
 * PTR CODE SECTION
 *===========================================================================*/
// Motor 1: Current parameters for Q15 concept
#define MT1_CURRENT_HIGH_LIMIT_HARD  25.0f
#define MT1_CURRENT_HIGH_LIMIT_SOFT  20.0f
#define MT1_CURRENT_SCALE_FACTOR     (uq31_t)(MT1_CURRENT_HIGH_LIMIT_HARD/MT1_CURRENT_HIGH_LIMIT_SOFT*32768.0f + 0.5f)
#define MT1_CURRENT_OFFSET           (0U)

// Motor 2: Current parameters for Q15 concept
#define MT2_CURRENT_HIGH_LIMIT_HARD  25.0f
#define MT2_CURRENT_HIGH_LIMIT_SOFT  20.0f
#define MT2_CURRENT_SCALE_FACTOR     (uq31_t)(MT2_CURRENT_HIGH_LIMIT_HARD/MT2_CURRENT_HIGH_LIMIT_SOFT*32768.0f + 0.5f)
#define MT2_CURRENT_OFFSET           (0U)

// Motor 3: Current parameters for Q15 concept
#define MT3_CURRENT_HIGH_LIMIT_HARD  25.0f
#define MT3_CURRENT_HIGH_LIMIT_SOFT  20.0f
#define MT3_CURRENT_SCALE_FACTOR     (uq31_t)(MT3_CURRENT_HIGH_LIMIT_HARD/MT3_CURRENT_HIGH_LIMIT_SOFT*32768.0f + 0.5f)
#define MT3_CURRENT_OFFSET           (0U)

typedef struct{
	Meas_TypeDef current_mt1;
	Meas_TypeDef current_mt2;
	Meas_TypeDef current_mt3;
	Meas_TypeDef thermal_mt1;
	Meas_TypeDef thermal_mt2;
	Meas_TypeDef thermal_mt3;
} MeasAll_TypeDef;

/*===========================================================================
 * REUSABLE CODE SECTION
 *===========================================================================*/
void MEAS_Config(Meas_TypeDef *pMeas, q15_t offset, uq31_t scale_factor, uq15_t alpha, uq15_t high_limit_soft, float gain_f32, float offset_f32, uint8_t calc_method);
void MEAS_GetRawValue(Meas_TypeDef *pMeas, q15_t raw_value);
void MEAS_GetValueQ15(Meas_TypeDef *pMeas);
void MEAS_GetValueF32(Meas_TypeDef *pMeas);

/*===========================================================================
 * PTR CODE SECTION
 *===========================================================================*/
void MEAS_Init(void);
void MEAS_GetValue(void);
void MEAS_GetRealValue(void);

extern MeasAll_TypeDef thisMeas;
#endif /* MIDDLELAYER_MEASUREMENT_MEASUREMENT_H_ */
