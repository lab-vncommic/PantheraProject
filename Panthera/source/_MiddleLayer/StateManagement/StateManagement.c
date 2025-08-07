/*
 * StateManagement.c
 *
 *  Created on: Jul 25, 2025
 *      Author: ADMIN
 */
#include "StateManagement.h"
PTR_TypeDef thisPTR;
uint16_t cnt_meas_type;
_Bool flag_auto_completed = false;
_Bool flag_save_to_idle   = false;
uint16_t cnt_delay_500ms;
uint16_t cnt_schedule_motor_auto;
uint16_t cnt_schedule_motor_manual;

// List API
void STATE_idleHandle(PTR_TypeDef *pState);
void STATE_saveHandle(PTR_TypeDef *pState);
void STATE_manualHandle(PTR_TypeDef *pState);
void STATE_autoHandle(PTR_TypeDef *pState);
void STATE_faultHandle(void);
//void STATE_autorecoveryHandle(PTR_TypeDef *pState);
//void STATE_shutdownHandle(PTR_TypeDef *pState);

void STATE_GetSeatAxis(PTR_TypeDef *pState);

/**
 * @brief Initializes the state management system
 * @details Sets the initial state to IDLE to ensure the system starts in a safe mode
 * @param[out] pState Pointer to the state management structure to be initialized
 * @return None
 */
void STATE_Init(PTR_TypeDef *pState)
{
	pState->state = STATE_IDLE;
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
	switch (pState->state) {
		case STATE_IDLE:
			// Processing: handle IDLE state
			STATE_idleHandle(pState);

			// Post-processing: check for faults and state transitions
			if (pState->button.height_1.action == ACTION_HOLD || 
				pState->button.height_2.action == ACTION_HOLD ||
				pState->button.recliner_1.action == ACTION_HOLD ||
				pState->button.recliner_2.action == ACTION_HOLD ||
				pState->button.slider_1.action == ACTION_HOLD ||
				pState->button.slider_2.action == ACTION_HOLD) {
					pState->state = STATE_MANUAL;
			} else if (pState->button.save.action == ACTION_PRESSED ||pState->button.save.action == ACTION_HOLD) {
				pState->state = STATE_SAVE;
			} else if (pState->button.recall_1.action == ACTION_PRESSED || pState->button.recall_1.action == ACTION_HOLD) {
				pState->seat_memory_axis = MEM_GetSeatAxis(1);
				pState->state = STATE_AUTO;
			} else if (pState->button.recall_2.action == ACTION_PRESSED || pState->button.recall_2.action == ACTION_HOLD) {
				pState->seat_memory_axis = MEM_GetSeatAxis(2);
				pState->state = STATE_AUTO;
			} else if (pState->button.recall_3.action == ACTION_PRESSED || pState->button.recall_3.action == ACTION_HOLD) {
				pState->seat_memory_axis = MEM_GetSeatAxis(3);
				pState->state = STATE_AUTO;
			}

			if (pState->fault.mode != FAULT_NONE) {
				pState->state = STATE_FAULT;
			}

			break;

		case STATE_SAVE:
			// Processing: handle AUTO state
			STATE_saveHandle(pState);

			// Post-processing: check for faults and state transitions
			if (pState->button.recall_1.action == ACTION_PRESSED ||
				pState->button.recall_1.action == ACTION_HOLD ||
				pState->button.recall_2.action == ACTION_PRESSED ||
				pState->button.recall_2.action == ACTION_HOLD ||
				pState->button.recall_3.action == ACTION_PRESSED ||
				pState->button.recall_3.action == ACTION_HOLD) {
					flag_save_to_idle = true;
			}

			if (flag_save_to_idle) {
				if (cnt_delay_500ms++ >= 2500) {
					cnt_delay_500ms = 0;
					pState->state = STATE_IDLE;
					flag_save_to_idle = false;
				}
			}

			if (pState->fault.mode != FAULT_NONE) {
				pState->state = STATE_FAULT;
			}

			break;

		case STATE_MANUAL:
			// Processing: handle MANUAL state
			STATE_manualHandle(pState);

			// Post-processing: check for faults and state transitions
			if (pState->button.height_1.action == ACTION_NA &&
				pState->button.height_2.action == ACTION_NA &&
				pState->button.recliner_1.action == ACTION_NA &&
				pState->button.recliner_2.action == ACTION_NA &&
				pState->button.slider_1.action == ACTION_NA &&
				pState->button.slider_2.action == ACTION_NA) {
				pState->state = STATE_IDLE;
			}

			if (pState->fault.mode != FAULT_NONE) {
				pState->state = STATE_FAULT;
			}

			break;

		case STATE_AUTO:
			// Processing: handle AUTO state
			STATE_autoHandle(pState);

			// Post-processing: check for faults and state transitions
			if (flag_auto_completed == true) {
				cnt_schedule_motor_auto = 0;
				flag_auto_completed    = false;
				pState->state 		   = STATE_IDLE;
			}

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
}

void STATE_saveHandle(PTR_TypeDef *pState)
{
	if (pState->button.recall_1.action == ACTION_PRESSED || pState->button.recall_1.action == ACTION_HOLD) {
		MEM_Write(pState->seat_current_axis.height, pState->seat_current_axis.recliner, pState->seat_current_axis.slider, 1);
	}

	if (pState->button.recall_2.action == ACTION_PRESSED || pState->button.recall_2.action == ACTION_HOLD) {
		MEM_Write(pState->seat_current_axis.height, pState->seat_current_axis.recliner, pState->seat_current_axis.slider, 2);
	}

	if (pState->button.recall_3.action == ACTION_PRESSED || pState->button.recall_3.action == ACTION_HOLD) {
		MEM_Write(pState->seat_current_axis.height, pState->seat_current_axis.recliner, pState->seat_current_axis.slider, 3);
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
	if (cnt_schedule_motor_manual >= MANUAL_ENABLE_MT) {
		cnt_schedule_motor_manual = MANUAL_ENABLE_MT;
	} else cnt_schedule_motor_manual++;

	if (cnt_schedule_motor_manual >= MANUAL_ENABLE_MT1) {
		// Handle height motor
		if ((pState->button.height_1.action == ACTION_HOLD) && (pState->button.height_2.action == ACTION_NA))  {
			MD_Run(MOTOR_HEIGHT, FORWARD, pState->measurement.current_height_q15);
		} else if ((pState->button.height_2.action == ACTION_HOLD) && (pState->button.height_1.action == ACTION_NA)) {
			MD_Run(MOTOR_HEIGHT, BACKWARD, pState->measurement.current_height_q15);
		} else MD_Run(MOTOR_HEIGHT, STOP, pState->measurement.current_height_q15);
	}

	if (cnt_schedule_motor_manual >= MANUAL_ENABLE_MT2) {
		// Handle recliner motor
		if ((pState->button.recliner_1.action == ACTION_HOLD) && (pState->button.recliner_2.action == ACTION_NA))  {
			MD_Run(MOTOR_RECLINER, FORWARD, pState->measurement.current_recliner_q15);
		} else if ((pState->button.recliner_2.action == ACTION_HOLD) && (pState->button.recliner_1.action == ACTION_NA)) {
			MD_Run(MOTOR_RECLINER, BACKWARD, pState->measurement.current_recliner_q15);
		} else MD_Run(MOTOR_RECLINER, STOP, pState->measurement.current_recliner_q15);
	}
	
	if (cnt_schedule_motor_manual >= MANUAL_ENABLE_MT3) {
		// Handle slider motor
		if ((pState->button.slider_1.action == ACTION_HOLD) && (pState->button.slider_2.action == ACTION_NA))  {
			MD_Run(MOTOR_SLIDER, FORWARD, pState->measurement.current_slider_q15);
		} else if ((pState->button.slider_2.action == ACTION_HOLD) && (pState->button.slider_1.action == ACTION_NA)) {
			MD_Run(MOTOR_SLIDER, BACKWARD, pState->measurement.current_slider_q15);
		} else MD_Run(MOTOR_SLIDER, STOP, pState->measurement.current_slider_q15);
	}
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
	if (pState->button.recall_1.action == ACTION_PRESSED || pState->button.recall_1.action == ACTION_HOLD) {
		pState->seat_memory_axis = MEM_GetSeatAxis(1);
	} else if (pState->button.recall_2.action == ACTION_PRESSED || pState->button.recall_2.action == ACTION_HOLD) {
		pState->seat_memory_axis = MEM_GetSeatAxis(2);
	} else if (pState->button.recall_3.action == ACTION_PRESSED || pState->button.recall_3.action == ACTION_HOLD) {
		pState->seat_memory_axis = MEM_GetSeatAxis(3);
	}

	if (cnt_schedule_motor_auto >= AUTO_ENABLE_MT) {
		cnt_schedule_motor_auto = AUTO_ENABLE_MT;
	} else cnt_schedule_motor_auto++;

	// Height motor - starts first (1ms)
	if (cnt_schedule_motor_auto >= AUTO_ENABLE_MT1) {
		if (pState->seat_current_axis.height > pState->seat_memory_axis.height) {
			MD_Run(MOTOR_HEIGHT, BACKWARD, pState->measurement.current_height_q15);
		} else if (pState->seat_current_axis.height < pState->seat_memory_axis.height) {
			MD_Run(MOTOR_HEIGHT, FORWARD, pState->measurement.current_height_q15);
		} else MD_Run(MOTOR_HEIGHT, STOP, pState->measurement.current_height_q15);
	}

	// Recliner motor - starts second (333ms)
	if (cnt_schedule_motor_auto >= AUTO_ENABLE_MT2) {
		if (pState->seat_current_axis.recliner > pState->seat_memory_axis.recliner) {
			MD_Run(MOTOR_RECLINER, BACKWARD, pState->measurement.current_recliner_q15);
		} else if (pState->seat_current_axis.recliner < pState->seat_memory_axis.recliner) {
			MD_Run(MOTOR_RECLINER, FORWARD, pState->measurement.current_recliner_q15);
		} else MD_Run(MOTOR_RECLINER, STOP, pState->measurement.current_recliner_q15);
	}

	// Slider motor - starts last (666ms)
	if (cnt_schedule_motor_auto >= AUTO_ENABLE_MT3) {
		if (pState->seat_current_axis.slider > pState->seat_memory_axis.slider) {
			MD_Run(MOTOR_SLIDER, BACKWARD, pState->measurement.current_slider_q15);
		} else if (pState->seat_current_axis.slider < pState->seat_memory_axis.slider) {
			MD_Run(MOTOR_SLIDER, FORWARD, pState->measurement.current_slider_q15);
		} else MD_Run(MOTOR_SLIDER, STOP, pState->measurement.current_slider_q15);
	}

	// Check if all motors have reached their target positions
	if ((pState->seat_current_axis.height == pState->seat_memory_axis.height) &&
		(pState->seat_current_axis.recliner == pState->seat_memory_axis.recliner) &&
		(pState->seat_current_axis.slider == pState->seat_memory_axis.slider)) {
		flag_auto_completed = true;
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
	pState->button = BTN_Update();
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
