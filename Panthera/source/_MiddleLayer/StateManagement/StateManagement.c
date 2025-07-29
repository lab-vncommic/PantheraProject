/*
 * StateManagement.c
 *
 *  Created on: Jul 25, 2025
 *      Author: ADMIN
 */
#include "StateManagement.h"
PTR_TypeDef thisPTR;

void STATE_Init(PTR_TypeDef *pState)
{
	pState->state = STATE_IDLE;
}

void STATE_UpdateState(PTR_TypeDef *pState)
{
//	if ((thisActiveBT.name == BT_NA) || (thisActiveBT.action ==  ACTION_NA)) {
//		pState->state = STATE_IDLE;
//	} else {
//
//	}

	switch (pState->state) {
		case STATE_IDLE:
			// Processing: handle IDLE state
			STATE_idleHandle();

			// Post-processing: check for faults and state transitions
			if (thisFault.fault_mode != FAULT_NONE) {
				pState->state = STATE_FAULT;
			}

			break;

		case STATE_MANUAL:
			// Processing: handle IDLE state
			STATE_manualHandle();

			// Post-processing: check for faults and state transitions
			if (thisFault.fault_mode != FAULT_NONE) {
				pState->state = STATE_FAULT;
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
	// Stop all motors
	MD_StopAll();

	// Update seat position
	thisPTR.seat_axis.height 	= motor_height.axis;
	thisPTR.seat_axis.recliner 	= motor_recliner.axis;
	thisPTR.seat_axis.slider 	= motor_slider.axis;
}

void STATE_manualHandle(void)
{
	switch (thisPTR.motor_active.name) {
		case MOTOR_ACTIVE_HEIGHT:
			MD_UpdateStatusMotor(&motor_height, thisPTR.motor_active.status);
			MD_UpdateDriver(&motor_height);
			MD_UpdateAxis(&motor_height);
			break;

		case MOTOR_ACTIVE_RECLINER:
			MD_UpdateStatusMotor(&motor_recliner, thisPTR.motor_active.status);
			MD_UpdateDriver(&motor_recliner);
			MD_UpdateAxis(&motor_recliner);
			break;

		case MOTOR_ACTIVE_SLIDER:
			MD_UpdateStatusMotor(&motor_slider, thisPTR.motor_active.status);
			MD_UpdateDriver(&motor_slider);
			MD_UpdateAxis(&motor_slider);
			break;

		default:
			MD_StopAll();
			break;
	}
}

void STATE_faultHandle(void)
{
	MD_StopAll();
}
