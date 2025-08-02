/*
 * StateManagement.c
 *
 *  Created on: Jul 25, 2025
 *      Author: ADMIN
 */
#include "StateManagement.h"
PTR_TypeDef thisPTR;
uint16_t index_memory = 1;
uint16_t save_available = 1;
uint16_t cnt_delay_btw_save = 0;

// List API
void STATE_idleHandle(PTR_TypeDef *pState);
void STATE_manualHandle(PTR_TypeDef *pState);
void STATE_autoHandle(PTR_TypeDef *pState);
void STATE_faultHandle(void);
//void STATE_autorecoveryHandle(PTR_TypeDef *pState);
//void STATE_shutdownHandle(PTR_TypeDef *pState);

void STATE_HandleButton(PTR_TypeDef *pState);
void STATE_GetSeatAxis(PTR_TypeDef *pState);

// Defination API
void STATE_Init(PTR_TypeDef *pState)
{
	pState->state = STATE_IDLE;
//	save_available 		= 1;
//	index_memory 		= 1;
//	cnt_delay_btw_save 	= 0;
}

void STATE_UpdateState(PTR_TypeDef *pState)
{
	// Determine state machine for system
	if ((pState->state != STATE_FAULT) && (pState->state != STATE_SHUTDOWN) && (pState->state != STATE_AUTORECOVERY)) {
		STATE_HandleButton(pState);
	}

	switch (pState->state) {
		case STATE_IDLE:
			// Processing: handle IDLE state
			STATE_idleHandle(pState);

			// Post-processing: check for faults and state transitions
			if (thisFault.fault_mode != FAULT_NONE) {
				pState->state = STATE_FAULT;
			}

			break;

		case STATE_MANUAL:
			// Processing: handle MANUAL state
			STATE_manualHandle(pState);

			// Post-processing: check for faults and state transitions
			if (thisFault.fault_mode != FAULT_NONE) {
				pState->state = STATE_FAULT;
			}

			break;

		case STATE_AUTO:
			// Processing: handle AUTO state
			STATE_autoHandle(pState);

			// Post-processing: check for faults and state transitions
			if (thisFault.fault_mode != FAULT_NONE) {
				pState->state = STATE_FAULT;
			}

			break;

		case STATE_FAULT:
			// Processing: handle FAULT state
			STATE_faultHandle();

			// Post-processing: check for faults and state transitions

			break;

		default:
			break;
	}
}

void STATE_idleHandle(PTR_TypeDef *pState)
{
	MD_StopAll();
	STATE_GetSeatAxis(pState);
	if (save_available == 0) {
		if (cnt_delay_btw_save++ >= TIMEOUT_SAVE_AXIS) {
			cnt_delay_btw_save = 0;
			save_available 	   = 1;
		}
	}
}

void STATE_manualHandle(PTR_TypeDef *pState)
{
	MD_Run(pState->motor.motor_name, pState->motor.status);
}

void STATE_autoHandle(PTR_TypeDef *pState)
{
	if (pState->seat_current_axis.height > pState->seat_memory_axis.height) {
		MD_Run(MOTOR_HEIGHT, BACKWARD);
	} else if (pState->seat_current_axis.height < pState->seat_memory_axis.height) {
		MD_Run(MOTOR_HEIGHT, FORWARD);
	} else if (pState->seat_current_axis.recliner > pState->seat_memory_axis.recliner) {
		MD_Run(MOTOR_RECLINER, BACKWARD);
	} else if (pState->seat_current_axis.recliner < pState->seat_memory_axis.recliner) {
		MD_Run(MOTOR_RECLINER, FORWARD);
	} else if (pState->seat_current_axis.slider > pState->seat_memory_axis.slider) {
		MD_Run(MOTOR_SLIDER, BACKWARD);
	} else if (pState->seat_current_axis.slider < pState->seat_memory_axis.slider) {
		MD_Run(MOTOR_SLIDER, FORWARD);
	} else {
		MD_StopAll();
	}
}

void STATE_faultHandle(void)
{
	MD_StopAll();
}

//void STATE_autorecoveryHandle(PTR_TypeDef *pState)
//{
//
//}
//
//void STATE_shutdownHandle(PTR_TypeDef *pState)
//{
//
//}

void STATE_HandleButton(PTR_TypeDef *pState)
{
	pState->button = BTN_GetStatus();
	switch (pState->button.name) {
		case BTN_HEIGHT_1:
			if (pState->button.action == ACTION_HOLD) {
				thisPTR.motor.motor_name = MOTOR_HEIGHT;
				thisPTR.motor.status 	 = FORWARD;
			}
			pState->state = STATE_MANUAL;
			break;

		case BTN_HEIGHT_2:
			if (pState->button.action == ACTION_HOLD) {
				thisPTR.motor.motor_name = MOTOR_HEIGHT;
				thisPTR.motor.status 	 = BACKWARD;
			}
			pState->state = STATE_MANUAL;
			break;

		case BTN_RECLINER_1:
			if (pState->button.action == ACTION_HOLD) {
				thisPTR.motor.motor_name = MOTOR_RECLINER;
				thisPTR.motor.status 	 = FORWARD;
			}
			pState->state = STATE_MANUAL;
			break;

		case BTN_RECLINER_2:
			if (pState->button.action == ACTION_HOLD) {
				thisPTR.motor.motor_name = MOTOR_RECLINER;
				thisPTR.motor.status 	 = BACKWARD;
			}
			pState->state = STATE_MANUAL;
			break;

		case BTN_SLIDER_1:
			if (pState->button.action == ACTION_HOLD) {
				thisPTR.motor.motor_name = MOTOR_SLIDER;
				thisPTR.motor.status 	 = FORWARD;
			}
			pState->state = STATE_MANUAL;
			break;

		case BTN_SLIDER_2:
			if (pState->button.action == ACTION_HOLD) {
				thisPTR.motor.motor_name = MOTOR_SLIDER;
				thisPTR.motor.status 	 = BACKWARD;
			}
			pState->state = STATE_MANUAL;
			break;

		case BTN_SAVE:
			if (pState->button.action == ACTION_PRESSED) {
				if (save_available == 1) {
					MEM_Write(pState->seat_current_axis.height, pState->seat_current_axis.recliner, pState->seat_current_axis.slider, index_memory);
					index_memory++;
					if (index_memory > 3) {
						index_memory = 1;
					}
					save_available = 0;
				}
			}
			break;

		case BTN_RECALL_1:
			if (pState->button.action == ACTION_HOLD) {
				pState->seat_memory_axis = MEM_GetSeatAxis(1);

			}
			pState->state = STATE_AUTO;
			break;

		case BTN_RECALL_2:
			if (pState->button.action == ACTION_HOLD) {
				pState->seat_memory_axis = MEM_GetSeatAxis(2);
			}
			pState->state = STATE_AUTO;
			break;

		case BTN_RECALL_3:
			if (pState->button.action == ACTION_HOLD) {
				pState->seat_memory_axis = MEM_GetSeatAxis(3);
			}
			pState->state = STATE_AUTO;
			break;

		default:
			pState->state = STATE_IDLE;
			thisPTR.motor.motor_name = MOTOR_NONE;
			thisPTR.motor.status 	 = STOP;
			break;
	}
}

void STATE_GetSeatAxis(PTR_TypeDef *pState)
{
	pState->seat_current_axis = MD_GetSeatAxis();
}
