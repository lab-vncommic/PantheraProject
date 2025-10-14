/*
 * Measurement.h
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */

#ifndef MIDDLELAYER_MEASUREMENT_MEASUREMENT_H_
#define MIDDLELAYER_MEASUREMENT_MEASUREMENT_H_
#include "../../panthera/midde_layer/inc/_mid_config.h"
#include "GlobalDefine.h"
#include "IQMath.h"
#include "adc.h"

/*===========================================================================
 * REUSABLE CODE SECTION
 *===========================================================================*/
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
} Meas_TypeDef;

/*===========================================================================
 * PTR CODE SECTION
 *===========================================================================*/
// Motor 1: Current parameters for Q15 concept (Motor Slider)
#define CURRENT_MT1_HIGH_LIMIT_HARD  22.75f
#define CURRENT_MT1_HIGH_LIMIT_SOFT  22.0f
#define CURRENT_MT1_SCALE_FACTOR     (uq31_t)(CURRENT_MT1_HIGH_LIMIT_HARD/CURRENT_MT1_HIGH_LIMIT_SOFT*32768.0f + 0.5f)
#define CURRENT_MT1_FREQ_CUTOFF		 3000U			// Uint: Hz
#define CURRENT_MT1_ALPHA			 _q15((2*PI*CURRENT_MT1_FREQ_CUTOFF) / ((2*PI*CURRENT_MT1_FREQ_CUTOFF) + F_SAMPLE))
#define CURRENT_MT1_OFFSET           (0U)

// Motor 2: Current parameters for Q15 concept (Motor Height)
#define CURRENT_MT2_HIGH_LIMIT_HARD  22.75f
#define CURRENT_MT2_HIGH_LIMIT_SOFT  22.0f
#define CURRENT_MT2_SCALE_FACTOR     (uq31_t)(CURRENT_MT2_HIGH_LIMIT_HARD/CURRENT_MT2_HIGH_LIMIT_SOFT*32768.0f + 0.5f)
#define CURRENT_MT2_FREQ_CUTOFF		 3000U			// Uint: Hz
#define CURRENT_MT2_ALPHA			 _q15((2*PI*CURRENT_MT2_FREQ_CUTOFF) / ((2*PI*CURRENT_MT2_FREQ_CUTOFF) + F_SAMPLE))
#define CURRENT_MT2_OFFSET           (0U)

// Motor 3: Current parameters for Q15 concept (Motor Recliner)
#define CURRENT_MT3_HIGH_LIMIT_HARD  22.75f
#define CURRENT_MT3_HIGH_LIMIT_SOFT  22.0f
#define CURRENT_MT3_SCALE_FACTOR     (uq31_t)(CURRENT_MT3_HIGH_LIMIT_HARD/CURRENT_MT3_HIGH_LIMIT_SOFT*32768.0f + 0.5f)
#define CURRENT_MT3_FREQ_CUTOFF		 3000U			// Uint: Hz
#define CURRENT_MT3_ALPHA			 _q15((2*PI*CURRENT_MT3_FREQ_CUTOFF) / ((2*PI*CURRENT_MT3_FREQ_CUTOFF) + F_SAMPLE))
#define CURRENT_MT3_OFFSET           (0U)

// Motor 1: Temperature parameters for F32 concept (Motor Slider)
#define THERMAL_MT1_GAIN_F32				-0.01387162642f		// Use for calculate real value
#define THERMAL_MT1_OFFSET_F32				402.4545454545f		// Use for calculate real value

// Motor 2: Temperature parameters for F32 concept (Motor Height)
#define THERMAL_MT2_GAIN_F32				-0.01387162642f		// Use for calculate real value
#define THERMAL_MT2_OFFSET_F32				402.4545454545f		// Use for calculate real value

// Motor 3: Temperature parameters for F32 concept (Motor Recliner)
#define THERMAL_MT3_GAIN_F32				-0.01387162642f		// Use for calculate real value
#define THERMAL_MT3_OFFSET_F32				402.4545454545f		// Use for calculate real value

#define SAMPLE_TIME_MEAS_THERMAL	20000	// 20000/20000 = 1s

//typedef enum{
//	MEAS_CURRENT 	= 0,
//	MEAS_THERMAL	= 1,
//	MEAS_VCC		= 2,
//	MEAS_NONE		= 3
//} MeasType_TypeDef;

typedef struct{
	Meas_TypeDef current_slider;
	Meas_TypeDef current_height;
	Meas_TypeDef current_recliner;
	Meas_TypeDef thermal_slider;
	Meas_TypeDef thermal_height;
	Meas_TypeDef thermal_recliner;
} MeasAll_TypeDef;

typedef struct{
	uq15_t current_slider_q15;
	uq15_t current_height_q15;
	uq15_t current_recliner_q15;
	uq15_t thermal_slider_q15;
	uq15_t thermal_height_q15;
	uq15_t thermal_recliner_q15;
} meas_system_t;

// Public API
void MEAS_Init(void);
void MEAS_UpdateValue(meas_system_t *pMeas, MeasType_TypeDef meas_type);

// Uncomment extern variables for debugging
extern MeasAll_TypeDef thisMeas;
#endif /* MIDDLELAYER_MEASUREMENT_MEASUREMENT_H_ */
