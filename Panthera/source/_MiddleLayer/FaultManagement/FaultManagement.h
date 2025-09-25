/*
 * FaultManagement.h
 *
 *  Created on: Jul 23, 2025
 *      Author: ADMIN
 */

#ifndef MIDDLELAYER_FAULTMANAGEMENT_FAULTMANAGEMENT_H_
#define MIDDLELAYER_FAULTMANAGEMENT_FAULTMANAGEMENT_H_
#include "GlobalDefine.h"
#include "Measurement.h"

/*===========================================================================
 * REUSABLE CODE SECTION
 *===========================================================================*/
#define FAULT_SAVE_MEMORY			 	10U
#define FAULT_STT_INIT_VALUE			0x00

#define FAULT_STT_0   ((uint8_t) 1U<<0)  // Over current motor Hieght
#define FAULT_STT_1   ((uint8_t) 1U<<1)  // Over current motor Recliner
#define FAULT_STT_2   ((uint8_t) 1U<<2)  // Over current motor Slider
#define FAULT_STT_3   ((uint8_t) 1U<<3)  // Over temperature motor Hieght
#define FAULT_STT_4   ((uint8_t) 1U<<4)  // Over temperature motor Recliner
#define FAULT_STT_5   ((uint8_t) 1U<<5)  // Over temperature motor Slider
#define FAULT_STT_6   ((uint8_t) 1U<<6)
#define FAULT_STT_7   ((uint8_t) 1U<<7)

#define FAULT_LATCH_BITS				0
#define FAULT_AUTO_RECOVERY_BITS		(FAULT_STT_0 | FAULT_STT_1 | FAULT_STT_2 | FAULT_STT_3 | FAULT_STT_4 | FAULT_STT_5)

typedef enum {
	FAULT_NONE 			= 0,
	FAULT_LATCH			= 1,
	FAULT_AUTO_RECOVER	= 2
} FaultMode_TypeDef;

typedef struct{
	uint8_t					fault_status;
	FaultMode_TypeDef 	  	fault_mode;
	uint8_t					limit;
	uint8_t					counter;
	uint8_t 			  	fault_save[FAULT_SAVE_MEMORY];
} Fault_TypeDef;

typedef struct{
	FaultMode_TypeDef		mode;
	uint8_t					limit;
	uint8_t					counter;

	uint32_t				cnt_debounce_auto_recovery;
} fault_system_t;

/*===========================================================================
 * PTR CODE SECTION
 *===========================================================================*/
#define CURRENT_MT1_OCP					 17.5f		// Unit: A
#define THERMAL_MT1_OTP					100.0f		// Unit: oC
#define CURRENT_MT2_OCP					 17.5f		// Unit: A
#define THERMAL_MT2_OTP					100.0f		// Unit: oC
#define CURRENT_MT3_OCP					 17.5f		// Unit: A
#define THERMAL_MT3_OTP					100.0f		// Unit: oC

/* ====== Fault Protection Thresholds in Q15 format ====== */
#define FAULT_MT1_OCP_THRESH_Q15      	_q15(CURRENT_MT1_OCP / CURRENT_MT1_HIGH_LIMIT_SOFT)
#define FAULT_MT1_OTP_THRESH_Q15      	(uq31_t)((THERMAL_MT1_OTP - THERMAL_MT1_OFFSET_F32)/(2*THERMAL_MT1_GAIN_F32))
#define FAULT_MT2_OCP_THRESH_Q15      	_q15(CURRENT_MT2_OCP / CURRENT_MT2_HIGH_LIMIT_SOFT)
#define FAULT_MT2_OTP_THRESH_Q15      	(uq31_t)((THERMAL_MT2_OTP - THERMAL_MT2_OFFSET_F32)/(2*THERMAL_MT2_GAIN_F32))
#define FAULT_MT3_OCP_THRESH_Q15      	_q15(CURRENT_MT3_OCP / CURRENT_MT3_HIGH_LIMIT_SOFT)
#define FAULT_MT3_OTP_THRESH_Q15      	(uq31_t)((THERMAL_MT3_OTP - THERMAL_MT3_OFFSET_F32)/(2*THERMAL_MT3_GAIN_F32))

/* ====== Macros for Fault Protection in Q15 format ====== */
#define FAULT_IS_MT1_OCP(value)       	(value > FAULT_MT1_OCP_THRESH_Q15)
#define FAULT_IS_MT1_OTP(value)       	(value < FAULT_MT1_OTP_THRESH_Q15)
#define FAULT_IS_MT2_OCP(value)       	(value > FAULT_MT2_OCP_THRESH_Q15)
#define FAULT_IS_MT2_OTP(value)       	(value < FAULT_MT2_OTP_THRESH_Q15)
#define FAULT_IS_MT3_OCP(value)       	(value > FAULT_MT3_OCP_THRESH_Q15)
#define FAULT_IS_MT3_OTP(value)       	(value < FAULT_MT3_OTP_THRESH_Q15)

// Public API
void FAULT_Init(void);
void FAULT_Handle(fault_system_t *pFault, meas_system_t *pMeas);
void FAULT_LogError(void);
void fault_reset(void);

// FIXME: Only use extern in dev
extern Fault_TypeDef 	thisFault;
#endif /* MIDDLELAYER_FAULTMANAGEMENT_FAULTMANAGEMENT_H_ */
