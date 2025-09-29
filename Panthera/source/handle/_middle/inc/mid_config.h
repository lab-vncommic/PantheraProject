/*
 * mid_config.h
 *
 *  Created on: Sep 28, 2025
 *      Author: Tracy McConnell
 */

#ifndef SOURCE_HANDLE__MIDDLE_INC_MID_CONFIG_H_
#define SOURCE_HANDLE__MIDDLE_INC_MID_CONFIG_H_
#define F_SAMPLE	20000.0f
#define T_SAMPLE	1/20000.0f

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

typedef enum{
	MEAS_CURRENT 	= 0,
	MEAS_THERMAL	= 1,
	MEAS_VCC		= 2,
	MEAS_NONE		= 3
} MeasType_TypeDef;

#endif /* SOURCE_HANDLE__MIDDLE_INC_MID_CONFIG_H_ */
