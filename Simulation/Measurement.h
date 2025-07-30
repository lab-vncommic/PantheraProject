/*
 * Measurement.h
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */

#ifndef MIDDLELAYER_MEASUREMENT_MEASUREMENT_H_
#define MIDDLELAYER_MEASUREMENT_MEASUREMENT_H_
// #include "components.h"
#include "IQMath.h"
#include "adc.h"

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
#define CURRENT_MT1_HIGH_LIMIT_HARD  25.0f
#define CURRENT_MT1_HIGH_LIMIT_SOFT  20.0f
#define CURRENT_MT1_SCALE_FACTOR     (uq31_t)(CURRENT_MT1_HIGH_LIMIT_HARD/CURRENT_MT1_HIGH_LIMIT_SOFT*32768.0f + 0.5f)
#define CURRENT_MT1_OFFSET           (0U)

// Motor 2: Current parameters for Q15 concept
#define CURRENT_MT2_HIGH_LIMIT_HARD  25.0f
#define CURRENT_MT2_HIGH_LIMIT_SOFT  20.0f
#define CURRENT_MT2_SCALE_FACTOR     (uq31_t)(CURRENT_MT2_HIGH_LIMIT_HARD/CURRENT_MT2_HIGH_LIMIT_SOFT*32768.0f + 0.5f)
#define CURRENT_MT2_OFFSET           (0U)

// Motor 3: Current parameters for Q15 concept
#define CURRENT_MT3_HIGH_LIMIT_HARD  25.0f
#define CURRENT_MT3_HIGH_LIMIT_SOFT  20.0f
#define CURRENT_MT3_SCALE_FACTOR     (uq31_t)(CURRENT_MT3_HIGH_LIMIT_HARD/CURRENT_MT3_HIGH_LIMIT_SOFT*32768.0f + 0.5f)
#define CURRENT_MT3_OFFSET           (0U)

// Motor 1: Temperature parameters for F32 concept
#define THERMAL_MT1_GAIN_F32				-0.01387162642f		// Use for calculate real value
#define THERMAL_MT1_OFFSET_F32				402.4545454545f		// Use for calculate real value

// Motor 2: Temperature parameters for F32 concept
#define THERMAL_MT2_GAIN_F32				-0.01387162642f		// Use for calculate real value
#define THERMAL_MT2_OFFSET_F32				402.4545454545f		// Use for calculate real value

// Motor 3: Temperature parameters for F32 concept
#define THERMAL_MT3_GAIN_F32				-0.01387162642f		// Use for calculate real value
#define THERMAL_MT3_OFFSET_F32				402.4545454545f		// Use for calculate real value

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
