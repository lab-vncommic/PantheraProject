/*
 * StateManagement.c
 *
 *  Created on: Jul 25, 2025
 *      Author: ADMIN
 */
#include "StateManagement.h"
PTR_TypeDef thisPTR;
uint16_t cnt_delay_1s;;

void STATE_Init(PTR_TypeDef *pState)
{
	pState->State = STATE_IDLE;
}

void STATE_UpdateState(PTR_TypeDef *pState)
{
	switch (pState->State) {
		case STATE_IDLE:
			// Processing: handle IDLE state
			STATE_idleHandle();

			// Post-processing: check for faults and state transitions
			if (thisFault.fault_mode != FAULT_NONE) {
				pState->State = STATE_FAULT;
			} else if (cnt_delay_1s++ >= 10000)
			{
				pState->State = STATE_MANUAL;
			}
			
			break;

		case STATE_MANUAL:
			// Processing: handle IDLE state
			STATE_manualHandle();

			// Post-processing: check for faults and state transitions
			if (thisFault.fault_mode != FAULT_NONE) {
				pState->State = STATE_FAULT;
			}

			break;

		case STATE_FAULT:
			// Processing: handle IDLE state
			STATE_faultHandle();

			// Post-processing: check for faults and state transitions

			break;

		default:
			break;
	}
}

void STATE_idleHandle(void)
{
	// thisPTR.Motor_Active = motor_height;
	// thisPTR.Motor_Active.status = BACKWARD;
}

void STATE_manualHandle(void)
{
	MD_UpdateStatusMotor(&motor_height, BACKWARD);
	MD_UpdateDriver(&motor_height);
	MD_CntHallSignal(&motor_height);
}

void STATE_faultHandle(void)
{
	MD_StopAll();
}
