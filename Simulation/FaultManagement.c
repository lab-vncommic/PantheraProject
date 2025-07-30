/*
 * FaultManagement.c
 *
 *  Created on: Jul 23, 2025
 *      Author: ADMIN
 */
#include "FaultManagement.h"
Fault_TypeDef 	thisFault;
uint8_t 		temp_fault_status = 0x00;

void FAULT_Init(void)
{
	thisFault.fault_status = 0x00;
	// for (uint8_t i = 0; i < FAULT_SAVE_MEMORY; i++) {
	// 	thisFault.fault_save[i] = 0x00;
	// }
}

void FAULT_UpdateStatus(void){
//    if (FAULT_IS_MT1_OCP(&thisMeas.current_mt1)) {
//    	temp_fault_status |= FAULT_STT_0;
//    }

//    if (FAULT_IS_MT2_OCP(&thisMeas.current_mt2) ) {
//    	temp_fault_status |= FAULT_STT_1;
//    }

//    if (FAULT_IS_MT3_OCP(&thisMeas.current_mt3) ) {
//    	temp_fault_status |= FAULT_STT_2;
//    }

//    if (FAULT_IS_MT1_OTP(&thisMeas.thermal_mt1)) {
//    	temp_fault_status |= FAULT_STT_3;
//    }

//    if (FAULT_IS_MT2_OTP(&thisMeas.thermal_mt2) ) {
//    	temp_fault_status |= FAULT_STT_4;
//    }

//    if (FAULT_IS_MT3_OTP(&thisMeas.thermal_mt3) ) {
//    	temp_fault_status |= FAULT_STT_5;
//    }
    thisFault.fault_status = temp_fault_status;
}

void FAULT_UpdateMode(void)
{
	FAULT_UpdateStatus();

	if ((thisFault.fault_status) & FAULT_LATCH_BITS) {
		thisFault.fault_mode = FAULT_LATCH;
	} else if ((thisFault.fault_status) & FAULT_AUTO_RECOVERY_BITS) {
		thisFault.fault_mode = FAULT_AUTO_RECOVER;
	} else thisFault.fault_mode = FAULT_NONE;
}

