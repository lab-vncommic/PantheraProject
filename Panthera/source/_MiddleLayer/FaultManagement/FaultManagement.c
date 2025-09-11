/*
 * FaultManagement.c
 *
 *  Created on: Jul 23, 2025
 *      Author: ADMIN
 */
#include "FaultManagement.h"
Fault_TypeDef 	thisFault;
uint8_t 		temp_fault_status;


void FAULT_UpdateStatus(MeasSystem_TypeDef *pMeas);
void FAULT_UpdateMode(Fault_TypeDef *pFault, MeasSystem_TypeDef *pMeas);

/**
 * @brief Initializes the fault management system
 *
 * This function initializes the fault management module by clearing the fault status
 * and resetting all fault history entries in the fault save array to zero.
 *
 * @note Call this function once during system initialization before using other fault management functions
 *
 * @return None
 */
void FAULT_Init(void)
{
	thisFault.fault_mode   = FAULT_NONE;
	thisFault.fault_status = FAULT_STT_INIT_VALUE;
	thisFault.limit		   = FAULT_SAVE_MEMORY;
	thisFault.counter	   = 0;
	 for (uint8_t i = 0; i < FAULT_SAVE_MEMORY; i++) {
	 	thisFault.fault_save[i] = FAULT_STT_INIT_VALUE;
	 }
}

/**
 * @brief Updates the fault status register based on motor protection conditions
 *
 * This function evaluates various over-current and over-temperature protection 
 * conditions for all three motors and updates the fault status register accordingly.
 * Each fault condition sets a corresponding bit in the fault status register.
 *
 * The following fault bits are updated:
 * - FAULT_STT_0: Motor 1 over-current protection
 * - FAULT_STT_1: Motor 2 over-current protection
 * - FAULT_STT_2: Motor 3 over-current protection
 * - FAULT_STT_3: Motor 1 over-temperature protection
 * - FAULT_STT_4: Motor 2 over-temperature protection
 * - FAULT_STT_5: Motor 3 over-temperature protection
 *
 * @return void
 * 
 * @note This function must be called periodically to keep fault status up-to-date
 */
void FAULT_UpdateStatus(MeasSystem_TypeDef *pMeas)
{
    if (FAULT_IS_MT1_OCP(pMeas->current_recliner_q15)) {
    	temp_fault_status |= FAULT_STT_0;
    }

    if (FAULT_IS_MT2_OCP(pMeas->current_slider_q15)) {
    	temp_fault_status |= FAULT_STT_1;
    }

	if (FAULT_IS_MT3_OCP(pMeas->current_height_q15)) {
		temp_fault_status |= FAULT_STT_2;
	}

//    if (FAULT_IS_MT1_OTP(pMeas->thermal_slider_q15)) {
//    	temp_fault_status |= FAULT_STT_3;
//    }
//
//    if (FAULT_IS_MT2_OTP(pMeas->thermal_height_q15)) {
//    	temp_fault_status |= FAULT_STT_4;
//    }
//
//    if (FAULT_IS_MT3_OTP(pMeas->thermal_recliner_q15)) {
//    	temp_fault_status |= FAULT_STT_5;
//    }
   thisFault.fault_status = temp_fault_status;
}

/**
 * @brief Updates the fault mode based on current fault status
 *
 * This function first updates the fault status by calling FAULT_UpdateStatus(),
 * then determines the appropriate fault mode based on the status bits.
 * The fault mode will be set to one of the following:
 * - FAULT_LATCH: If any latch fault bits are active
 * - FAULT_AUTO_RECOVER: If any auto-recovery fault bits are active
 * - FAULT_NONE: If no fault bits are active
 *
 * @return None
 *
 * @note The fault mode priority is: FAULT_LATCH > FAULT_AUTO_RECOVER > FAULT_NONE
 */
void FAULT_UpdateMode(Fault_TypeDef *pFault, MeasSystem_TypeDef *pMeas)
{
	FAULT_UpdateStatus(pMeas);

	if ((pFault->fault_status) & FAULT_LATCH_BITS) {
		pFault->fault_mode = FAULT_LATCH;
	} else if ((pFault->fault_status) & FAULT_AUTO_RECOVERY_BITS) {
		pFault->fault_mode = FAULT_AUTO_RECOVER;
		if (thisFault.counter >= thisFault.limit) {
			thisFault.fault_mode = FAULT_LATCH;
		}
	} else pFault->fault_mode = FAULT_NONE;
}

void FAULT_LogError(void)
{
	if (thisFault.counter <= (thisFault.limit - 1)) {
		thisFault.fault_save[thisFault.counter] = thisFault.fault_status;
	}
}

void FAULT_Handle(FaultSystem_TypeDef *pFault, MeasSystem_TypeDef *pMeas)
{
	FAULT_UpdateMode(&thisFault, pMeas);

	pFault->mode    = thisFault.fault_mode;
	pFault->counter = thisFault.counter;
	pFault->limit   = thisFault.limit;
}

void FAULT_Reset(void)
{
	thisFault.fault_mode   = FAULT_NONE;
	thisFault.fault_status = FAULT_STT_INIT_VALUE;
	temp_fault_status      = FAULT_STT_INIT_VALUE;
	thisFault.counter++;
}


