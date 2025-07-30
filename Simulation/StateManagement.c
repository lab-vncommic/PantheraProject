/*
 * StateManagement.c
 *
 *  Created on: Jul 25, 2025
 *      Author: ADMIN
 */
#include "StateManagement.h"
PTR_TypeDef thisPTR;
uint16_t counter = 0;

void STATE_Init(PTR_TypeDef *pState)
{
	pState->state = STATE_IDLE;
}

void STATE_UpdateState(PTR_TypeDef *pState)
{	
	STATE_HandleButton(pState, &thisButton);
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
			// Processing: handle MANUAL state
			STATE_manualHandle();

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

void STATE_HandleButton(PTR_TypeDef *pState, Button_TypeDef *pButton)
{
	switch (pButton->name) {
		case BTN_HEIGHT_1:
			if (pButton->action == ACTION_HOLD) {
				thisPTR.motor_active.name   = MOTOR_ACTIVE_HEIGHT;
				thisPTR.motor_active.status = FORWARD;
			}
			pState->state = STATE_MANUAL;
			break;

		case BTN_HEIGHT_2:
			if (pButton->action == ACTION_HOLD) {
				thisPTR.motor_active.name   = MOTOR_ACTIVE_HEIGHT;
				thisPTR.motor_active.status = BACKWARD;
			}
			pState->state = STATE_MANUAL;
			break;

		case BTN_RECLINER_1:
			if (pButton->action == ACTION_HOLD) {
				thisPTR.motor_active.name   = MOTOR_ACTIVE_RECLINER;
				thisPTR.motor_active.status = FORWARD;
			}
			pState->state = STATE_MANUAL;
			break;

		case BTN_RECLINER_2:
			if (pButton->action == ACTION_HOLD) {
				thisPTR.motor_active.name   = MOTOR_ACTIVE_RECLINER;
				thisPTR.motor_active.status = BACKWARD;
			}
			pState->state = STATE_MANUAL;
			break;

		case BTN_SLIDER_1:
			if (pButton->action == ACTION_HOLD) {
				thisPTR.motor_active.name   = MOTOR_ACTIVE_SLIDER;
				thisPTR.motor_active.status = FORWARD;
			}
			pState->state = STATE_MANUAL;
			break;

		case BTN_SLIDER_2:
			if (pButton->action == ACTION_HOLD) {
				thisPTR.motor_active.name   = MOTOR_ACTIVE_SLIDER;
				thisPTR.motor_active.status = BACKWARD;
			}
			pState->state = STATE_MANUAL;
			break;

		case BTN_SAVE:
			if (pButton->action == ACTION_PRESSED) {
				// Save logic
			}
			break;

		case BTN_RECALL_1:
			if (pButton->action == ACTION_PRESSED) {
				// Recall 1 logic
			}
			pState->state = STATE_AUTO;
			break;

		case BTN_RECALL_2:
			if (pButton->action == ACTION_PRESSED) {
				// Recall 2 logic
			}
			pState->state = STATE_AUTO;
			break;

		case BTN_RECALL_3:
			if (pButton->action == ACTION_PRESSED) {
				// Recall 3 logic
			}
			pState->state = STATE_AUTO;
			break;

		default:
			pState->state = STATE_IDLE;
			thisPTR.motor_active.name = MOTOR_ACTIVE_NONE;
			thisPTR.motor_active.status = STOP;
			break;
	}
}