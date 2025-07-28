/*
 * FaultManagement.h
 *
 *  Created on: Jul 23, 2025
 *      Author: ADMIN
 */

#ifndef MIDDLELAYER_FAULTMANAGEMENT_FAULTMANAGEMENT_H_
#define MIDDLELAYER_FAULTMANAGEMENT_FAULTMANAGEMENT_H_
// #include "components.h"
#include "Measurement.h"

/*===========================================================================
 * REUSABLE CODE SECTION
 *===========================================================================*/
#define FAULT_SAVE_MEMORY			 	3U

#define FAULT_STT_0   ((uint16_t) 1U<<0)  // Over current motor Hieght
#define FAULT_STT_1   ((uint16_t) 1U<<1)  // Over current motor Recliner
#define FAULT_STT_2   ((uint16_t) 1U<<2)  // Over current motor Slider
#define FAULT_STT_3   ((uint16_t) 1U<<3)  // Over temperature motor Hieght
#define FAULT_STT_4   ((uint16_t) 1U<<4)  // Over temperature motor Recliner
#define FAULT_STT_5   ((uint16_t) 1U<<5)  // Over temperature motor Slider
#define FAULT_STT_6   ((uint16_t) 1U<<6)
#define FAULT_STT_7   ((uint16_t) 1U<<7)

#define FAULT_LATCH_BITS				0
#define FAULT_AUTO_RECOVERY_BITS		(FAULT_STT_0 | FAULT_STT_1 | FAULT_STT_2 | FAULT_STT_3 | FAULT_STT_4 | FAULT_STT_5)

typedef enum {
	FAULT_NONE = 0,
	FAULT_LATCH,
	FAULT_AUTO_RECOVER,
} Fault_Mode_TypeDef;

typedef struct{
	uint8_t					fault_status;
	uint8_t 			  	fault_save[FAULT_SAVE_MEMORY];
	Fault_Mode_TypeDef 	  	fault_mode;
} Fault_TypeDef;

/*===========================================================================
 * PTR CODE SECTION
 *===========================================================================*/
#define CURRENT_MT1_OCP					1.5f		// Unit: A
#define TEMPERATURE_MT1_OTP				90.0f		// Unit: oC
#define CURRENT_MT2_OCP					17.0f		// Unit: A
#define TEMPERATURE_MT2_OTP				90.0f		// Unit: oC
#define CURRENT_MT3_OCP					17.0f		// Unit: A
#define TEMPERATURE_MT3_OTP				90.0f		// Unit: oC

/* ====== Fault Protection Thresholds in Q15 format ====== */
#define FAULT_MT1_OCP_THRESH_Q15      	_q15(CURRENT_MT1_OCP / CURRENT_MT1_HIGH_LIMIT_SOFT)
#define FAULT_MT1_OTP_THRESH_Q15      	(TEMPERATURE_MT1_OTP - THERMAL_MT1_OFFSET_F32)/(2*THERMAL_MT1_GAIN_F32)
#define FAULT_MT2_OCP_THRESH_Q15      	_q15(CURRENT_MT2_OCP / CURRENT_MT2_HIGH_LIMIT_SOFT)
#define FAULT_MT2_OTP_THRESH_Q15      	(TEMPERATURE_MT2_OTP - THERMAL_MT2_OFFSET_F32)/(2*THERMAL_MT2_GAIN_F32)
#define FAULT_MT3_OCP_THRESH_Q15      	_q15(CURRENT_MT3_OCP / CURRENT_MT3_HIGH_LIMIT_SOFT)
#define FAULT_MT3_OTP_THRESH_Q15      	(TEMPERATURE_MT3_OTP - THERMAL_MT3_OFFSET_F32)/(2*THERMAL_MT3_GAIN_F32)

/* ====== Macros for Fault Protection in Q15 format ====== */
#define FAULT_IS_MT1_OCP(pMeas)       	((pMeas)->value_q15 > FAULT_MT1_OCP_THRESH_Q15)
#define FAULT_IS_MT1_OTP(pMeas)       	((pMeas)->value_q15 < FAULT_MT1_OTP_THRESH_Q15)
#define FAULT_IS_MT2_OCP(pMeas)       	((pMeas)->value_q15 > FAULT_MT2_OCP_THRESH_Q15)
#define FAULT_IS_MT2_OTP(pMeas)       	((pMeas)->value_q15 < FAULT_MT2_OTP_THRESH_Q15)
#define FAULT_IS_MT3_OCP(pMeas)       	((pMeas)->value_q15 > FAULT_MT3_OCP_THRESH_Q15)
#define FAULT_IS_MT3_OTP(pMeas)       	((pMeas)->value_q15 < FAULT_MT3_OTP_THRESH_Q15)

void FAULT_Init(void);
void FAULT_UpdateStatus(void);
void FAULT_UpdateMode(void);

extern Fault_TypeDef 	thisFault;
#endif /* MIDDLELAYER_FAULTMANAGEMENT_FAULTMANAGEMENT_H_ */
