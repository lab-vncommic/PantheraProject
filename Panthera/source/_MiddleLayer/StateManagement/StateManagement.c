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
uint16_t cnt_meas_type;

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
/**
 * @brief Initializes the state management system
 * @details Sets the initial state to IDLE to ensure the system starts in a safe mode
 * @param[out] pState Pointer to the state management structure to be initialized
 * @return None
 */
void STATE_Init(PTR_TypeDef *pState)
{
	pState->state = STATE_IDLE;
	save_available 		= 1;
	index_memory 		= 1;
	cnt_delay_btw_save 	= 0;
}

/**
 * @brief Updates the system state based on current conditions
 * @details Processes button inputs if not in fault, shutdown, or auto-recovery states,
 *          then executes the appropriate handler for the current state
 * @param[in,out] pState Pointer to the state management structure
 * @return None
 */
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
			if (pState->fault.mode != FAULT_NONE) {
				pState->state = STATE_FAULT;
			}

			break;

		case STATE_MANUAL:
			// Processing: handle MANUAL state
			STATE_manualHandle(pState);

			// Post-processing: check for faults and state transitions
			if (pState->fault.mode != FAULT_NONE) {
				pState->state = STATE_FAULT;
			}

			break;

		case STATE_AUTO:
			// Processing: handle AUTO state
			STATE_autoHandle(pState);

			// Post-processing: check for faults and state transitions
			if (pState->fault.mode != FAULT_NONE) {
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

/**
 * @brief Handles the system behavior in IDLE state
 * @details Stops all motors, updates current seat axis position, and manages
 *          save availability timing for memory functions
 * @param[in,out] pState Pointer to the state management structure
 * @return None
 */
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

/**
 * @brief Handles the system behavior in MANUAL state
 * @details Runs the selected motor with the specified status (forward/backward)
 * @param[in] pState Pointer to the state management structure with motor settings
 * @return None
 */
void STATE_manualHandle(PTR_TypeDef *pState)
{
	MD_Run(pState->motor_manual.name, pState->motor_manual.status, &pState->measurement);
}

/**
 * @brief Handles the system behavior in AUTO state
 * @details Compares current seat axis positions with memory positions and
 *          runs appropriate motors to move the seat to the target position
 * @param[in] pState Pointer to the state management structure
 * @return None
 */
void STATE_autoHandle(PTR_TypeDef *pState)
{
	if (pState->seat_current_axis.height > pState->seat_memory_axis.height) {
		MD_Run(MOTOR_HEIGHT, BACKWARD, &pState->measurement);
	} else if (pState->seat_current_axis.height < pState->seat_memory_axis.height) {
		MD_Run(MOTOR_HEIGHT, FORWARD, &pState->measurement);
	} else if (pState->seat_current_axis.recliner > pState->seat_memory_axis.recliner) {
		MD_Run(MOTOR_RECLINER, BACKWARD, &pState->measurement);
	} else if (pState->seat_current_axis.recliner < pState->seat_memory_axis.recliner) {
		MD_Run(MOTOR_RECLINER, FORWARD, &pState->measurement);
	} else if (pState->seat_current_axis.slider > pState->seat_memory_axis.slider) {
		MD_Run(MOTOR_SLIDER, BACKWARD, &pState->measurement);
	} else if (pState->seat_current_axis.slider < pState->seat_memory_axis.slider) {
		MD_Run(MOTOR_SLIDER, FORWARD, &pState->measurement);
	} else {
		MD_StopAll();
	}
}

/**
 * @brief Handles the system behavior in FAULT state
 * @details Stops all motors to ensure safe operation when a fault is detected
 * @return None
 */
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

/**
 * @brief Processes button inputs and updates system state accordingly
 * @details Gets the current button status and determines the appropriate
 *          state and motor actions based on which button is pressed
 * @param[in,out] pState Pointer to the state management structure to update
 * @return None
 */
void STATE_HandleButton(PTR_TypeDef *pState)
{
	pState->button = BTN_GetStatus();
	switch (pState->button.name) {
		case BTN_HEIGHT_1:
			if (pState->button.action == ACTION_HOLD) {
				thisPTR.motor_manual.name = MOTOR_HEIGHT;
				thisPTR.motor_manual.status 	 = FORWARD;
			}
			pState->state = STATE_MANUAL;
			break;

		case BTN_HEIGHT_2:
			if (pState->button.action == ACTION_HOLD) {
				thisPTR.motor_manual.name = MOTOR_HEIGHT;
				thisPTR.motor_manual.status 	 = BACKWARD;
			}
			pState->state = STATE_MANUAL;
			break;

		case BTN_RECLINER_1:
			if (pState->button.action == ACTION_HOLD) {
				thisPTR.motor_manual.name = MOTOR_RECLINER;
				thisPTR.motor_manual.status 	 = FORWARD;
			}
			pState->state = STATE_MANUAL;
			break;

		case BTN_RECLINER_2:
			if (pState->button.action == ACTION_HOLD) {
				thisPTR.motor_manual.name = MOTOR_RECLINER;
				thisPTR.motor_manual.status 	 = BACKWARD;
			}
			pState->state = STATE_MANUAL;
			break;

		case BTN_SLIDER_1:
			if (pState->button.action == ACTION_HOLD) {
				thisPTR.motor_manual.name = MOTOR_SLIDER;
				thisPTR.motor_manual.status 	 = FORWARD;
			}
			pState->state = STATE_MANUAL;
			break;

		case BTN_SLIDER_2:
			if (pState->button.action == ACTION_HOLD) {
				thisPTR.motor_manual.name = MOTOR_SLIDER;
				thisPTR.motor_manual.status 	 = BACKWARD;
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
			thisPTR.motor_manual.name = MOTOR_NONE;
			thisPTR.motor_manual.status 	 = STOP;
			break;
	}
}

void STATE_UpdateMeas(PTR_TypeDef *pState)
{
	cnt_meas_type++;
	if (cnt_meas_type >= (SAMPLE_TIME_MEAS_THERMAL - 5)) {
		if (cnt_meas_type >= (SAMPLE_TIME_MEAS_THERMAL - 2))
		{
			MD_UpdateMeas(MEAS_THERMAL);
			pState->measurement = MEAS_UpdateValue(MEAS_THERMAL);

		}
		if (cnt_meas_type >= SAMPLE_TIME_MEAS_THERMAL) {
			cnt_meas_type = 0;
		}
	} else {
		MD_UpdateMeas(MEAS_CURRENT);
		pState->measurement = MEAS_UpdateValue(MEAS_CURRENT);
	}
}

void STATE_UpdateFault(PTR_TypeDef *pState)
{
	pState->fault.mode = FAULT_UpdateMode(&pState->measurement);
}

void STATE_UpdateButton(PTR_TypeDef *pState)
{
	pState->button	   = BTN_GetStatus();
}

/**
 * @brief Updates the current seat axis positions in the state structure
 * @details Retrieves the current position values for all seat axes from
 *          the motor driver and stores them in the state structure
 * @param[out] pState Pointer to the state management structure to update
 * @return None
 */
void STATE_GetSeatAxis(PTR_TypeDef *pState)
{
	pState->seat_current_axis = MD_GetSeatAxis();
}
