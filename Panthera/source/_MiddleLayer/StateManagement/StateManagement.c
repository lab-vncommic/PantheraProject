/*
 * StateManagement.c
 *
 *  Created on: Jul 25, 2025
 *      Author: ADMIN
 */
#include "StateManagement.h"
PTR_TypeDef thisPTR;
uint32_t cnt_meas_type;
_Bool flag_auto_completed = false;
_Bool flag_save_to_idle   = false;
_Bool flag_power_on_first_time = false;
_Bool flag_change_to_state_home;
_Bool flag_save_current_position;
_Bool flag_enable_check_fault;

uint16_t cnt_delay_500ms;
uint16_t cnt_schedule_motor_auto;
uint16_t cnt_schedule_motor_manual;
uint16_t cnt_schedule_motor_home;
uint32_t cnt_debounce_home;

State_TypeDef temp_state;
uint8_t counter_comm;

uint16_t debug_var[30];
uint16_t debug_var_cur[10];
uint16_t i, j;
uint16_t temp;

// List API
void STATE_idleHandle(PTR_TypeDef *pState);
void STATE_saveHandle(PTR_TypeDef *pState);
void STATE_manualHandle(PTR_TypeDef *pState);
void STATE_autoHandle(PTR_TypeDef *pState);
void STATE_faultHandle(void);
void STATE_autorecoveryHandle(void);
void STATE_shutdownHandle(void);
void STATE_homingHandle(PTR_TypeDef *pState);

void STATE_HandleManualButton(Button_TypeDef *pButton, uint16_t comm_value);
void STATE_HandleRecallButton(Button_TypeDef *pButton, uint16_t comm_value);
void STATE_UpdateICasComm(Motor_TypeDef *pMotor, MotorObject_TypeDef *pMotorObj);
_Bool STATE_SystemIsNormal(PTR_TypeDef *pState);
void STATE_GetSeatAxis(PTR_TypeDef *pState);
void STATE_ResetAxis(PTR_TypeDef *pState);
void STATE_ClearMemoryData(void);

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
//	STATE_GetSeatAxis(pState);
	if (flag_power_on_first_time == true) {
		MEM_GetSeatAxis(&pState->seat_current_axis, 4);
		motor_recliner.axis = pState->seat_current_axis.recliner;
		motor_slider.axis   = pState->seat_current_axis.slider;
		motor_height.axis   = pState->seat_current_axis.height;
		flag_power_on_first_time = false;
	}

	if (flag_save_current_position == true) {
		MEM_Write(pState->seat_current_axis.height, pState->seat_current_axis.recliner, pState->seat_current_axis.slider, 4);
		flag_save_current_position = false;
	}
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
	if (cnt_schedule_motor_manual >= MANUAL_ENABLE_MT3) {
		cnt_schedule_motor_manual = MANUAL_ENABLE_MT3;
	} else cnt_schedule_motor_manual++;

	if (cnt_schedule_motor_manual >= MANUAL_ENABLE_MT1) {
		if ((pState->button.recliner_1.action == ACTION_HOLD) &&
			(pState->button.recliner_2.action == ACTION_RELEASED))  {
			MD_Run(MOTOR_RECLINER, FORWARD, pState->measurement.current_recliner_q15);
		} else if ((pState->button.recliner_2.action == ACTION_HOLD) &&
				   (pState->button.recliner_1.action == ACTION_RELEASED)) {
				   MD_Run(MOTOR_RECLINER, BACKWARD, pState->measurement.current_recliner_q15);
		} else MD_Run(MOTOR_RECLINER, STOP, pState->measurement.current_recliner_q15);
	}

	if (cnt_schedule_motor_manual >= MANUAL_ENABLE_MT2) {
		if ((pState->button.slider_1.action == ACTION_HOLD) &&
			(pState->button.slider_2.action == ACTION_RELEASED))  {
			MD_Run(MOTOR_SLIDER, FORWARD, pState->measurement.current_slider_q15);
		} else if ((pState->button.slider_2.action == ACTION_HOLD) &&
				   (pState->button.slider_1.action == ACTION_RELEASED)) {
					MD_Run(MOTOR_SLIDER, BACKWARD, pState->measurement.current_slider_q15);
		} else MD_Run(MOTOR_SLIDER, STOP, pState->measurement.current_slider_q15);
	}

	if (cnt_schedule_motor_manual >= MANUAL_ENABLE_MT3) {
		if ((pState->button.height_1.action == ACTION_HOLD) &&
			(pState->button.height_2.action == ACTION_RELEASED))  {
			MD_Run(MOTOR_HEIGHT, FORWARD, pState->measurement.current_height_q15);
		} else if ((pState->button.height_2.action == ACTION_HOLD) &&
				   (pState->button.height_1.action == ACTION_RELEASED)) {
				   MD_Run(MOTOR_HEIGHT, BACKWARD, pState->measurement.current_height_q15);
		} else MD_Run(MOTOR_HEIGHT, STOP, pState->measurement.current_height_q15);
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
	if (cnt_schedule_motor_auto >= AUTO_ENABLE_MT3) {
		cnt_schedule_motor_auto = AUTO_ENABLE_MT3;
	} else cnt_schedule_motor_auto++;

	if (cnt_schedule_motor_auto >= AUTO_ENABLE_MT1) {
		if (pState->seat_current_axis.recliner > pState->seat_memory_axis.recliner) {
			MD_Run(MOTOR_RECLINER, BACKWARD, pState->measurement.current_recliner_q15);
		} else if (pState->seat_current_axis.recliner < pState->seat_memory_axis.recliner) {
			MD_Run(MOTOR_RECLINER, FORWARD, pState->measurement.current_recliner_q15);
		} else MD_Run(MOTOR_RECLINER, STOP, pState->measurement.current_recliner_q15);
	}

	if (cnt_schedule_motor_auto >= AUTO_ENABLE_MT2) {
		if (pState->seat_current_axis.slider > pState->seat_memory_axis.slider) {
			MD_Run(MOTOR_SLIDER, BACKWARD, pState->measurement.current_slider_q15);
		} else if (pState->seat_current_axis.slider < pState->seat_memory_axis.slider) {
			MD_Run(MOTOR_SLIDER, FORWARD, pState->measurement.current_slider_q15);
		} else MD_Run(MOTOR_SLIDER, STOP, pState->measurement.current_slider_q15);
	}

	if (cnt_schedule_motor_auto >= AUTO_ENABLE_MT3) {
		if (pState->seat_current_axis.height > pState->seat_memory_axis.height) {
			MD_Run(MOTOR_HEIGHT, BACKWARD, pState->measurement.current_height_q15);
		} else if (pState->seat_current_axis.height < pState->seat_memory_axis.height) {
			MD_Run(MOTOR_HEIGHT, FORWARD, pState->measurement.current_height_q15);
		} else MD_Run(MOTOR_HEIGHT, STOP, pState->measurement.current_height_q15);
	}

	// Check if all motors have reached their target positions
	MD_GetSeatAxis(&pState->seat_current_axis);
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

void STATE_autorecoveryHandle(void)
{
	FAULT_LogError();
}

void STATE_shutdownHandle(void)
{
	// Do nothing
}

void STATE_homingHandle(PTR_TypeDef *pState)
{
	if (cnt_schedule_motor_home >= HOME_ENABLE_MT3) {
		cnt_schedule_motor_home = HOME_ENABLE_MT3;
	} else cnt_schedule_motor_home++;

	if (cnt_schedule_motor_home >= HOME_ENABLE_MT1) {
		if (motor_recliner.state != BLOCK) {
			pState->state_home.flag_recliner_block = false;
			MD_Run(MOTOR_RECLINER, BACKWARD, pState->measurement.current_recliner_q15);
		} else pState->state_home.flag_recliner_block = true;
	}

	if (cnt_schedule_motor_home >= HOME_ENABLE_MT2) {
		if (motor_slider.state != BLOCK) {
			pState->state_home.flag_slider_block = false;
			MD_Run(MOTOR_SLIDER, BACKWARD, pState->measurement.current_slider_q15);
		} else pState->state_home.flag_slider_block = true;
	}

	if (cnt_schedule_motor_home >= HOME_ENABLE_MT3) {
		if (motor_height.state != BLOCK) {
			pState->state_home.flag_height_block = false;
			MD_Run(MOTOR_HEIGHT, BACKWARD, pState->measurement.current_height_q15);
		} else pState->state_home.flag_height_block = true;
	}
}

void STATE_ProcessInputSignals(PTR_TypeDef *pState)
{
	// Using the helper function to handle all manual operation buttons
	STATE_HandleManualButton(&pState->button.recliner_1, pState->communication.button_comm.recliner_1_value);
	STATE_HandleManualButton(&pState->button.recliner_2, pState->communication.button_comm.recliner_2_value);
	STATE_HandleManualButton(&pState->button.slider_1, pState->communication.button_comm.slider_1_value);
	STATE_HandleManualButton(&pState->button.slider_2, pState->communication.button_comm.slider_2_value);
	STATE_HandleManualButton(&pState->button.height_1, pState->communication.button_comm.height_1_value);
	STATE_HandleManualButton(&pState->button.height_2, pState->communication.button_comm.height_2_value);
	STATE_HandleManualButton(&pState->button.save, pState->communication.button_comm.save_value);
	STATE_HandleRecallButton(&pState->button.recall_1, pState->communication.button_comm.recall_1_value);
	STATE_HandleRecallButton(&pState->button.recall_2, pState->communication.button_comm.recall_2_value);
	STATE_HandleRecallButton(&pState->button.recall_3, pState->communication.button_comm.recall_3_value);

//	if (flag_update_ic == true) {
//		flag_update_ic = false;
//		STATE_UpdateICasComm(&motor_height, &pState->communication.motor_comm.motor_height);
//		STATE_UpdateICasComm(&motor_recliner, &pState->communication.motor_comm.motor_recliner);
//		STATE_UpdateICasComm(&motor_slider, &pState->communication.motor_comm.motor_slider);
//	}
}

void STATE_HandleManualButton(Button_TypeDef *pButton, uint16_t comm_value)
{
	if (comm_value == 0 && (pButton->action == ACTION_RELEASED || pButton->action == ACTION_PRESSED)) {
		pButton->action = ACTION_RELEASED;
	} else pButton->action = ACTION_HOLD;
}

void STATE_HandleRecallButton(Button_TypeDef *pButton, uint16_t comm_value)
{
	if (comm_value == 1) {
		pButton->action = ACTION_HOLD_LONG;
	} else if (pButton->action == ACTION_RELEASED) {
		pButton->action = ACTION_RELEASED;
	}
}

void STATE_UpdateICasComm(Motor_TypeDef *pMotor, MotorObject_TypeDef *pMotorObj)
{
	if (pMotorObj->ina_value == 1) {
		pMotor->driver.ina.value = 1;
	} else pMotor->driver.ina.value = 0;

	if (pMotorObj->inb_value == 1) {
		pMotor->driver.inb.value = 1;
	} else pMotor->driver.inb.value = 0;
}

_Bool STATE_SystemIsNormal(PTR_TypeDef *pState)
{
	if ((pState->measurement.current_slider_q15 <= CURRENT_AUTO_RECOVERY_MT2_THRESHOLD_Q15) &&
	 	(pState->measurement.current_height_q15 <= CURRENT_AUTO_RECOVERY_MT3_THRESHOLD_Q15) &&
	 	(pState->measurement.current_recliner_q15 <= CURRENT_AUTO_RECOVERY_MT1_THRESHOLD_Q15)/* &&
	 	(pState->measurement.thermal_slider_q15 >= THERMAL_AUTO_RECOVERY_MT1_THRESHOLD_Q15) &&
	 	(pState->measurement.thermal_height_q15 >= THERMAL_AUTO_RECOVERY_MT2_THRESHOLD_Q15) &&
	 	(pState->measurement.thermal_recliner_q15 >= THERMAL_AUTO_RECOVERY_MT3_THRESHOLD_Q15)*/) {
			return true;
	}
	return false;
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
	MD_GetSeatAxis(&pState->seat_current_axis);
}

void STATE_ResetAxis(PTR_TypeDef *pState)
{
	// Reset parameters for detecting end of travel
	pState->state_home.cnt_use_recall       = 0;
	pState->state_home.flag_slider_block    = false;
	pState->state_home.flag_height_block    = false;
	pState->state_home.flag_recliner_block  = false;

	// Reset axis
	MD_ResetAxis();
}

/**
 * @brief Initializes the state management system
 * @details Sets the initial state to IDLE to ensure the system starts in a safe mode
 * @param[out] pState Pointer to the state management structure to be initialized
 * @return None
 */
void STATE_Init(PTR_TypeDef *pState)
{
	pState->state = STATE_IDLE;
	flag_power_on_first_time = true;
	flag_enable_check_fault = false;
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
	STATE_ProcessInputSignals(pState);
	STATE_GetSeatAxis(pState);

	switch (pState->state) {
		case STATE_IDLE:
			// Processing: handle IDLE state
			STATE_idleHandle(pState);

			// Post-processing: check for faults and state transitions
			if (pState->fault.mode != FAULT_NONE) {
				pState->state = STATE_FAULT;
			} else if (pState->button.height_1.action == ACTION_HOLD ||
				pState->button.height_2.action == ACTION_HOLD ||
				pState->button.recliner_1.action == ACTION_HOLD ||
				pState->button.recliner_2.action == ACTION_HOLD ||
				pState->button.slider_1.action == ACTION_HOLD ||
				pState->button.slider_2.action == ACTION_HOLD) {
					pState->state = STATE_MANUAL;
					MD_EnableAll();
			} else if ((thisGPIO.recall_1.value) && (thisGPIO.recall_2.value) && (thisGPIO.recall_3.value)) {
							cnt_debounce_home++;
							if (cnt_debounce_home >= HOME_HOLD_BTN_TIME) {
								cnt_debounce_home = 0;
								pState->state = STATE_HOMING;
							}
			} else if (pState->button.save.action == ACTION_HOLD) {
				pState->state = STATE_SAVE;
			} else if (pState->button.recall_1.action == ACTION_HOLD_LONG) {
				MEM_GetSeatAxis(&pState->seat_memory_axis, 1);
				if (pState->seat_current_axis.recliner != pState->seat_memory_axis.recliner ||
					pState->seat_current_axis.height != pState->seat_memory_axis.height ||
					pState->seat_current_axis.slider != pState->seat_memory_axis.slider) {
						pState->state = STATE_AUTO;
						pState->state_home.cnt_use_recall++;
				}
			} else if (pState->button.recall_2.action == ACTION_PRESSED || pState->button.recall_2.action == ACTION_HOLD) {
				MEM_GetSeatAxis(&pState->seat_memory_axis, 2);
				if (pState->seat_current_axis.recliner != pState->seat_memory_axis.recliner ||
					pState->seat_current_axis.height != pState->seat_memory_axis.height ||
					pState->seat_current_axis.slider != pState->seat_memory_axis.slider) {
						pState->state = STATE_AUTO;
						pState->state_home.cnt_use_recall++;
				}
			} else if (pState->button.recall_3.action == ACTION_PRESSED || pState->button.recall_3.action == ACTION_HOLD) {
				MEM_GetSeatAxis(&pState->seat_memory_axis, 3);
				if (pState->seat_current_axis.recliner != pState->seat_memory_axis.recliner ||
					pState->seat_current_axis.height != pState->seat_memory_axis.height ||
					pState->seat_current_axis.slider != pState->seat_memory_axis.slider) {
						pState->state = STATE_AUTO;
						pState->state_home.cnt_use_recall++;
				}
			}

			break;

		case STATE_SAVE:
			// Processing: handle AUTO state
			STATE_saveHandle(pState);

			// Post-processing: check for faults and state transitions
			if (pState->fault.mode != FAULT_NONE) {
				pState->state = STATE_FAULT;
			} else if (pState->button.recall_1.action == ACTION_PRESSED ||
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
					flag_save_current_position = true;
					flag_save_to_idle = false;
				}
			}

			break;

		case STATE_MANUAL:
			// Processing: handle MANUAL state
			STATE_manualHandle(pState);

			// Post-processing: check for faults and state transitions
			if (pState->fault.mode != FAULT_NONE) {
				pState->state = STATE_FAULT;
			} else if (pState->button.height_1.action == ACTION_RELEASED &&
				pState->button.height_2.action == ACTION_RELEASED &&
				pState->button.recliner_1.action == ACTION_RELEASED &&
				pState->button.recliner_2.action == ACTION_RELEASED &&
				pState->button.slider_1.action == ACTION_RELEASED &&
				pState->button.slider_2.action == ACTION_RELEASED) {
					pState->state = STATE_IDLE;
					flag_save_current_position = true;
					cnt_schedule_motor_manual = 0;
			}

			break;

		case STATE_AUTO:
			// Processing: handle AUTO state
			STATE_autoHandle(pState);

			// Post-processing: check for faults and state transitions
			if (pState->fault.mode != FAULT_NONE) {
				pState->state = STATE_FAULT;
			} else if (flag_auto_completed == true) {
						cnt_schedule_motor_auto = 0;
						flag_auto_completed    = false;
						pState->state 		   = STATE_IDLE;
						flag_save_current_position = true;
					}

			break;

		case STATE_HOMING:
			// Processing: handle AUTO state
			STATE_homingHandle(pState);

			// Post-processing: check for faults and state transitions
			if (pState->fault.mode != FAULT_NONE) {
				pState->state = STATE_FAULT;
			} else if (pState->state_home.flag_recliner_block &&
					   pState->state_home.flag_slider_block &&
					   pState->state_home.flag_height_block) {
					   STATE_ClearMemoryData();
					   pState->state = STATE_IDLE;
					   flag_save_current_position = true;
					}

			break;

		case STATE_FAULT:
			// Processing: handle FAULT state
			STATE_faultHandle();

			// Post-processing: check for faults and state transitions
			if (pState->fault.mode == FAULT_LATCH) {
				pState->state = STATE_SHUTDOWN;
			} else if (pState->fault.mode == FAULT_AUTO_RECOVER) {
				pState->state = STATE_AUTORECOVERY;
			}
			break;

		case STATE_AUTORECOVERY:
			// Processing: handle FAULT state
			STATE_autorecoveryHandle();

			// Post-processing: check for faults and state transitions
			if (GDE_DebounceConditon(STATE_SystemIsNormal(pState), TIME_WAIT_FOR_AUTO_RECOVERY_CYCLES, &pState->fault.cnt_debounce_auto_recovery)) {
				pState->state = STATE_IDLE;
				flag_save_current_position = true;
				pState->fault.mode = FAULT_NONE;
				FAULT_Reset();
			}
			break;

		case STATE_SHUTDOWN:
			// Processing: handle FAULT state
			STATE_shutdownHandle();

			break;

		default:
			break;
	}
}

void STATE_UpdateMeas(PTR_TypeDef *pState)
{
	ADC_startOfConversion();
	if (cnt_meas_type >= SAMPLE_TIME_MEAS_THERMAL) {
		cnt_meas_type = 0;
	} else cnt_meas_type++;

	if (cnt_meas_type >= (SAMPLE_TIME_MEAS_THERMAL - 2)) {
		MD_UpdateMeas(MEAS_THERMAL);
		if (cnt_meas_type == SAMPLE_TIME_MEAS_THERMAL) {
			MEAS_UpdateValue(&pState->measurement, MEAS_THERMAL);
			flag_enable_check_fault = true;
		}
	} else {
		MD_UpdateMeas(MEAS_CURRENT);
		if (cnt_meas_type >= 2) {
			MEAS_UpdateValue(&pState->measurement, MEAS_CURRENT);
		}
	}
}

void STATE_UpdateFault(PTR_TypeDef *pState)
{
	if ((pState->state != STATE_FAULT) &&
		(pState->state != STATE_SHUTDOWN) &&
		(pState->state != STATE_AUTORECOVERY)) {
		FAULT_Handle(&pState->fault, &pState->measurement);
	}
}

void STATE_UpdateButton(PTR_TypeDef *pState)
{
	BTN_Update(&pState->button);
}

void STATE_ClearMemoryData(void)
{
	MEM_Write(0, 0, 0, 1);
	MEM_Write(0, 0, 0, 2);
	MEM_Write(0, 0, 0, 3);
}


