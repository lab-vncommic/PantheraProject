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

void state_handle_manual_button(Button_TypeDef *pButton, uint16_t comm_value);
void state_handle_recall_button(Button_TypeDef *pButton, uint16_t comm_value);
void STATE_UpdateICasComm(Motor_TypeDef *pMotor, MotorObject_TypeDef *pMotorObj);
_Bool STATE_SystemIsNormal(PTR_TypeDef *pState);
void STATE_GetSeatAxis(PTR_TypeDef *pState);
void STATE_ResetAxis(PTR_TypeDef *pState);
void state_clear_memory(void);

/**
 * @brief Handles the system behavior in IDLE state
 * @details Stops all motors, updates current seat axis position, and manages
 *          save availability timing for memory functions
 * @param[in,out] pState Pointer to the state management structure
 * @return None
 */
void STATE_idleHandle(PTR_TypeDef *pState)
{
	md_stop_all();
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
	md_get_seat_axis(&pState->seat_current_axis);
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
	md_stop_all();
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
	state_handle_manual_button(&pState->button.recliner_1, pState->communication.button_comm.recliner_1_value);
	state_handle_manual_button(&pState->button.recliner_2, pState->communication.button_comm.recliner_2_value);
	state_handle_manual_button(&pState->button.slider_1, pState->communication.button_comm.slider_1_value);
	state_handle_manual_button(&pState->button.slider_2, pState->communication.button_comm.slider_2_value);
	state_handle_manual_button(&pState->button.height_1, pState->communication.button_comm.height_1_value);
	state_handle_manual_button(&pState->button.height_2, pState->communication.button_comm.height_2_value);
	state_handle_manual_button(&pState->button.save, pState->communication.button_comm.save_value);
	state_handle_recall_button(&pState->button.recall_1, pState->communication.button_comm.recall_1_value);
	state_handle_recall_button(&pState->button.recall_2, pState->communication.button_comm.recall_2_value);
	state_handle_recall_button(&pState->button.recall_3, pState->communication.button_comm.recall_3_value);

//	if (flag_update_ic == true) {
//		flag_update_ic = false;
//		STATE_UpdateICasComm(&motor_height, &pState->communication.motor_comm.motor_height);
//		STATE_UpdateICasComm(&motor_recliner, &pState->communication.motor_comm.motor_recliner);
//		STATE_UpdateICasComm(&motor_slider, &pState->communication.motor_comm.motor_slider);
//	}
}

void state_handle_manual_button(Button_TypeDef *pButton, uint16_t comm_value)
{
	if (comm_value == 0 && (pButton->action == ACTION_RELEASED || pButton->action == ACTION_PRESSED)) {
		pButton->action = ACTION_RELEASED;
	} else pButton->action = ACTION_HOLD;
}

void state_handle_recall_button(Button_TypeDef *pButton, uint16_t comm_value)
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
	 	(pState->measurement.current_recliner_q15 <= CURRENT_AUTO_RECOVERY_MT1_THRESHOLD_Q15) &&
	 	(pState->measurement.thermal_slider_q15 >= THERMAL_AUTO_RECOVERY_MT1_THRESHOLD_Q15) &&
	 	(pState->measurement.thermal_height_q15 >= THERMAL_AUTO_RECOVERY_MT2_THRESHOLD_Q15) &&
	 	(pState->measurement.thermal_recliner_q15 >= THERMAL_AUTO_RECOVERY_MT3_THRESHOLD_Q15)) {
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
	md_get_seat_axis(&pState->seat_current_axis);
}



void STATE_ResetAxis(PTR_TypeDef *pState)
{
	// Reset parameters for detecting end of travel
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
					md_enable_all();
			} else if ((thisGPIO.recall_1.value) && (thisGPIO.recall_2.value) && (thisGPIO.recall_3.value)) {
							cnt_debounce_home++;
							if (cnt_debounce_home >= TIME_DEBOUNCE_GOTO_INSTALL_MODE) {
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
				}
			} else if (pState->button.recall_2.action == ACTION_HOLD_LONG) {
				MEM_GetSeatAxis(&pState->seat_memory_axis, 2);
				if (pState->seat_current_axis.recliner != pState->seat_memory_axis.recliner ||
					pState->seat_current_axis.height != pState->seat_memory_axis.height ||
					pState->seat_current_axis.slider != pState->seat_memory_axis.slider) {
						pState->state = STATE_AUTO;
				}
			} else if (pState->button.recall_3.action == ACTION_HOLD_LONG) {
				MEM_GetSeatAxis(&pState->seat_memory_axis, 3);
				if (pState->seat_current_axis.recliner != pState->seat_memory_axis.recliner ||
					pState->seat_current_axis.height != pState->seat_memory_axis.height ||
					pState->seat_current_axis.slider != pState->seat_memory_axis.slider) {
						pState->state = STATE_AUTO;
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
						pState->state 		   = STATE_IDLE;

						flag_save_current_position = true;
						cnt_schedule_motor_auto = 0;
						flag_auto_completed    = false;
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

					   pState->state = STATE_IDLE;

					   flag_save_current_position = true;
					   state_clear_memory();
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
				fault_reset();
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

void state_clear_memory(void)
{
	MEM_Write(0, 0, 0, 1);
	MEM_Write(0, 0, 0, 2);
	MEM_Write(0, 0, 0, 3);
}

void state_save_axis_max(state_machine_t *p_sm)
{
	md_update_axis_max(&p_sm->data.motor);
	MEM_Write(p_sm->data.motor.recliner.axis_max,
			  p_sm->data.motor.slider.axis_max,
			  p_sm->data.motor.height.axis_max, ID_MEM_AXIS_MAX);
}

// FIXME: Code for testing new API update state
void state_idle_entry(state_machine_t *p_sm);
void state_idle_process(state_machine_t *p_sm);
void state_idle_check_transition(state_machine_t *p_sm);
void state_idle_exit(state_machine_t *p_sm);

void state_install_entry(state_machine_t *p_sm);
void state_install_process(state_machine_t *p_sm);
void state_install_check_transition(state_machine_t *p_sm);
void state_install_exit(state_machine_t *p_sm);

void state_manual_entry(state_machine_t *p_sm);
void state_manual_process(state_machine_t *p_sm);
void state_manual_check_transition(state_machine_t *p_sm);
void state_manual_exit(state_machine_t *p_sm);

void state_save_entry(state_machine_t *p_sm);
void state_save_process(state_machine_t *p_sm);
void state_save_check_transition(state_machine_t *p_sm);
void state_save_exit(state_machine_t *p_sm);

void state_auto_entry(state_machine_t *p_sm);
void state_auto_process(state_machine_t *p_sm);
void state_auto_check_transition(state_machine_t *p_sm);
void state_auto_exit(state_machine_t *p_sm);

void state_fault_entry(state_machine_t *p_sm);
void state_fault_process(state_machine_t *p_sm);
void state_fault_check_transition(state_machine_t *p_sm);
void state_fault_exit(state_machine_t *p_sm);

void state_auto_recovery_entry(state_machine_t *p_sm);
void state_auto_recovery_process(state_machine_t *p_sm);
void state_auto_recovery_check_transition(state_machine_t *p_sm);
void state_auto_recovery_exit(state_machine_t *p_sm);

void state_change_state(state_machine_t *p_sm, state_t *p_state);
void state_install_run_all_motor(state_machine_t *p_sm, MotorStatus_TypeDef status);
void state_manual_run_motor(MotorName_TypeDef name, Button_TypeDef button_1, Button_TypeDef button_2, uint16_t current_value);
void state_auto_run_motor(MotorName_TypeDef name, uint32_t axis_current, uint32_t axis_memory, uint16_t current_value);
static inline _Bool manual_button_pressed(state_machine_t *p_sm);
static inline _Bool all_manual_button_release(state_machine_t *p_sm);
static inline _Bool seat_at_wrong_position(state_machine_t *p_sm);
static inline _Bool seat_at_right_position(state_machine_t *p_sm);
static inline _Bool system_is_normal(state_machine_t *p_sm);
static inline _Bool all_motor_blocked(state_machine_t *p_sm);

state_t state_idle = {
		.type = STATE_IDLE,
		.entry = state_idle_entry,
		.process = state_idle_process,
		.exit = state_idle_exit
};

state_t state_install = {
		.type = STATE_INSTALL,
		.entry = state_install_entry,
		.process = state_install_process,
		.exit = state_install_exit
};

state_t state_manual = {
		.type = STATE_MANUAL,
		.entry = state_manual_entry,
		.process = state_manual_process,
		.exit = state_manual_exit
};

state_t state_save = {
		.type = STATE_SAVE,
		.entry = state_save_entry,
		.process = state_save_process,
		.exit = state_save_exit
};

state_t state_auto = {
		.type = STATE_AUTO,
		.entry = state_auto_entry,
		.process = state_auto_process,
		.exit = state_auto_exit
};

state_t state_fault = {
		.type = STATE_FAULT,
		.entry = state_fault_entry,
		.process = state_fault_process,
		.exit = state_fault_exit
};

state_t state_auto_recovery = {
		.type = STATE_AUTORECOVERY,
		.entry = state_auto_recovery_entry,
		.process = state_auto_recovery_process,
		.exit = state_auto_recovery_exit
};

state_t state_shut_down = {
		.type = STATE_SHUTDOWN,
		.entry = NULL,
		.process = NULL,
		.exit = NULL
};

state_machine_t state_machine = {
		.current = &state_idle,
		.cnt_time_on_state = 0,
	    .flag_change_state = false
};


void state_int(state_machine_t *p_sm)
{
	p_sm->data.flag_power_on_mcu = true;
}

void state_update_meas(state_machine_t *p_sm)
{
	ADC_startOfConversion();
	if (p_sm->data.cnt_change_measure_obj >= SAMPLE_TIME_MEAS_THERMAL) {
		p_sm->data.cnt_change_measure_obj = 0;
	} else p_sm->data.cnt_change_measure_obj++;

	if (p_sm->data.cnt_change_measure_obj >= (SAMPLE_TIME_MEAS_THERMAL - 2)) {
		MD_UpdateMeas(MEAS_THERMAL);
		if (p_sm->data.cnt_change_measure_obj == SAMPLE_TIME_MEAS_THERMAL) {
			MEAS_UpdateValue(&p_sm->data.measurement, MEAS_THERMAL);
			p_sm->data.flag_enable_check_fault = true;
		}
	} else {
		MD_UpdateMeas(MEAS_CURRENT);
		if (p_sm->data.cnt_change_measure_obj >= 2) {
			MEAS_UpdateValue(&p_sm->data.measurement, MEAS_CURRENT);
		}
	}
}

void state_update_button(state_machine_t *p_sm)
{
	BTN_Update(&p_sm->data.button);
}

void state_update_motor(state_machine_t *p_sm)
{
	md_update_to_system(&p_sm->data.motor);
}

void state_update_fault(state_machine_t *p_sm)
{
	if ((p_sm->current->type != STATE_FAULT) &&
		(p_sm->current->type != STATE_SHUTDOWN) &&
		(p_sm->current->type != STATE_AUTORECOVERY &&
		 p_sm->data.flag_enable_check_fault)) {
		FAULT_Handle(&p_sm->data.fault, &p_sm->data.measurement);
	}
}

void state_update_comm(state_machine_t *p_sm)
{
	state_handle_manual_button(&p_sm->data.button.recliner_1, p_sm->data.communication.button_comm.recliner_1_value);
	state_handle_manual_button(&p_sm->data.button.recliner_2, p_sm->data.communication.button_comm.recliner_2_value);
	state_handle_manual_button(&p_sm->data.button.slider_1, p_sm->data.communication.button_comm.slider_1_value);
	state_handle_manual_button(&p_sm->data.button.slider_2, p_sm->data.communication.button_comm.slider_2_value);
	state_handle_manual_button(&p_sm->data.button.height_1, p_sm->data.communication.button_comm.height_1_value);
	state_handle_manual_button(&p_sm->data.button.height_2, p_sm->data.communication.button_comm.height_2_value);
	state_handle_manual_button(&p_sm->data.button.save, p_sm->data.communication.button_comm.save_value);
	state_handle_recall_button(&p_sm->data.button.recall_1, p_sm->data.communication.button_comm.recall_1_value);
	state_handle_recall_button(&p_sm->data.button.recall_2, p_sm->data.communication.button_comm.recall_2_value);
	state_handle_recall_button(&p_sm->data.button.recall_3, p_sm->data.communication.button_comm.recall_3_value);
}

void state_get_seat_axis(state_machine_t *p_sm)
{
	md_get_seat_axis(&p_sm->data.state.seat_current_axis);
}

void state_update_state(state_machine_t *p_sm)
{
	if (p_sm->flag_change_state) {
		// Run exit function of old state
		p_sm->current->exit(p_sm);

		p_sm->data.state.previous = p_sm->current->type;
		p_sm->data.state.current  = p_sm->next->type;

		p_sm->current = p_sm->next;
		p_sm->cnt_time_on_state = 0;

		// Run entry function of new state
		p_sm->current->entry(p_sm);
		p_sm->flag_change_state = false;
	} else {
		// Run process function
		p_sm->cnt_time_on_state++;
		p_sm->current->process(p_sm);
	}
}

void state_idle_entry(state_machine_t *p_sm)
{
	if (p_sm->data.flag_save_current_position == true) {
		MEM_Write(p_sm->data.state.seat_current_axis.recliner, p_sm->data.state.seat_current_axis.slider, p_sm->data.state.seat_current_axis.height, 4);
		p_sm->data.flag_save_current_position = false;
	}
	md_stop_all();
}

void state_idle_process(state_machine_t *p_sm)
{
	if (p_sm->data.flag_power_on_mcu == true) {
		MEM_GetSeatAxis(&p_sm->data.state.seat_current_axis, 4);
		motor_recliner.axis = p_sm->data.state.seat_current_axis.recliner;
		motor_slider.axis   = p_sm->data.state.seat_current_axis.slider;
		motor_height.axis   = p_sm->data.state.seat_current_axis.height;

		mem_get_axis_max(&p_sm->data.motor);
		motor_recliner.axis_max = p_sm->data.motor.recliner.axis_max;
		motor_slider.axis_max   = p_sm->data.motor.slider.axis_max;
		motor_height.axis_max   = p_sm->data.motor.height.axis_max;

		p_sm->data.flag_power_on_mcu = false;
	}

	state_idle_check_transition(p_sm);
}

void state_idle_check_transition(state_machine_t *p_sm)
{
	// Check for fault condition first (highest priority)
	if (p_sm->data.fault.mode != FAULT_NONE) {
		state_change_state(p_sm, &state_fault);
		return;
	}
	
	// Check for manual control buttons
	if (manual_button_pressed(p_sm)) {
		state_change_state(p_sm, &state_manual);
		return;
	}
	
	// Check for install request (all recall buttons held)
	if ((thisGPIO.recall_1.value) && (thisGPIO.recall_2.value) && (thisGPIO.recall_3.value)) {
		if (p_sm->data.cnt_debounce_check_install_mode >= TIME_DEBOUNCE_GOTO_INSTALL_MODE) {
			p_sm->data.cnt_debounce_check_install_mode = 0;
				state_change_state(p_sm, &state_install);
		} else p_sm->data.cnt_debounce_check_install_mode++;
		return;
	}
	
	// Check for save button
	if (p_sm->data.button.save.action == ACTION_HOLD) {
		state_change_state(p_sm, &state_save);
		return;
	}
	
	// Check for memory recall requests
	p_sm->data.state.memory_number = 0;
	
	if (p_sm->data.button.recall_1.action == ACTION_HOLD_LONG)
		p_sm->data.state.memory_number = 1;
	else if (p_sm->data.button.recall_2.action == ACTION_HOLD_LONG)
		p_sm->data.state.memory_number = 2;
	else if (p_sm->data.button.recall_3.action == ACTION_HOLD_LONG)
		p_sm->data.state.memory_number = 3;
	
	if (p_sm->data.state.memory_number > 0) {
		MEM_GetSeatAxis(&p_sm->data.state.seat_memory_axis, p_sm->data.state.memory_number);
		if (seat_at_wrong_position(p_sm)) {
			state_change_state(p_sm, &state_auto);
		}
		return;
	}
}

void state_idle_exit(state_machine_t *p_sm)
{
	(void)p_sm;
}

void state_install_entry(state_machine_t *p_sm)
{
	(void)p_sm;
	md_enable_all();
	p_sm->data.flag_system_homing = true;
	p_sm->data.flag_install_completed = false;
}

void state_install_process(state_machine_t *p_sm)
{
	if (p_sm->data.flag_system_homing) {
		state_install_run_all_motor(p_sm, BACKWARD);
		if (all_motor_blocked(p_sm)) {
			p_sm->data.flag_system_homing = false;
			p_sm->data.cnt_delay_startup_motor = 0;
		}
	} else {
		state_install_run_all_motor(p_sm, FORWARD);
		if (all_motor_blocked(p_sm)) {
			p_sm->data.flag_install_completed = true;
		}
	}

	state_install_check_transition(p_sm);
}

void state_install_check_transition(state_machine_t *p_sm)
{
	// Check for fault condition first (highest priority)
	if (p_sm->data.fault.mode != FAULT_NONE) {
		state_change_state(p_sm, &state_fault);
		return;
	}

	if (p_sm->data.flag_install_completed) {
		state_change_state(p_sm, &state_idle);
		return;
	}

}

void state_install_exit(state_machine_t *p_sm)
{
	p_sm->data.flag_save_current_position = true;
	p_sm->data.cnt_delay_startup_motor    = 0;
	state_clear_memory();
	state_save_axis_max(p_sm);
}

void state_manual_entry(state_machine_t *p_sm)
{
	(void)p_sm;
	md_enable_all();
}

void state_manual_process(state_machine_t *p_sm)
{
	if (p_sm->data.cnt_delay_startup_motor >= TIME_ENABLE_MT3) {
		p_sm->data.cnt_delay_startup_motor = TIME_ENABLE_MT3;
	} else p_sm->data.cnt_delay_startup_motor++;

	if (p_sm->data.cnt_delay_startup_motor >= TIME_ENABLE_MT1) {
		state_manual_run_motor(MOTOR_RECLINER, p_sm->data.button.recliner_1, p_sm->data.button.recliner_2, p_sm->data.measurement.current_recliner_q15);
	}

	if (p_sm->data.cnt_delay_startup_motor >= TIME_ENABLE_MT2) {
		state_manual_run_motor(MOTOR_SLIDER, p_sm->data.button.slider_1, p_sm->data.button.slider_2, p_sm->data.measurement.current_slider_q15);
	}

	if (p_sm->data.cnt_delay_startup_motor >= TIME_ENABLE_MT3) {
		state_manual_run_motor(MOTOR_HEIGHT, p_sm->data.button.height_1, p_sm->data.button.height_2, p_sm->data.measurement.current_height_q15);
	}

	state_manual_check_transition(p_sm);
}



void state_manual_check_transition(state_machine_t *p_sm)
{
	if (p_sm->data.fault.mode != FAULT_NONE) {
		state_change_state(p_sm, &state_fault);
	} else if (all_manual_button_release(p_sm)) {
			state_change_state(p_sm, &state_idle);
	}
}

void state_manual_exit(state_machine_t *p_sm)
{
	p_sm->data.flag_save_current_position = true;
	p_sm->data.cnt_delay_startup_motor    = 0;
}

void state_save_entry(state_machine_t *p_sm)
{
	p_sm->data.state.memory_number = 0;
}

void state_save_process(state_machine_t *p_sm)
{
	if (p_sm->data.button.recall_1.action == ACTION_PRESSED ||
		p_sm->data.button.recall_1.action == ACTION_HOLD ||
		p_sm->data.button.recall_1.action == ACTION_HOLD_LONG) {
		p_sm->data.flag_save_memory_position = true;
		p_sm->data.state.memory_number = 1;
	}

	if (p_sm->data.button.recall_2.action == ACTION_PRESSED||
		p_sm->data.button.recall_2.action == ACTION_HOLD ||
		p_sm->data.button.recall_2.action == ACTION_HOLD_LONG) {
		p_sm->data.flag_save_memory_position = true;
		p_sm->data.state.memory_number = 2;
	}

	if (p_sm->data.button.recall_3.action == ACTION_PRESSED ||
		p_sm->data.button.recall_3.action == ACTION_HOLD ||
		p_sm->data.button.recall_3.action == ACTION_HOLD_LONG) {
		p_sm->data.flag_save_memory_position = true;
		p_sm->data.state.memory_number = 3;
	}

	if (p_sm->data.flag_save_memory_position == true) {
		MEM_Write(p_sm->data.state.seat_current_axis.recliner,
				  p_sm->data.state.seat_current_axis.slider,
				  p_sm->data.state.seat_current_axis.height,
				  p_sm->data.state.memory_number);
	}

	state_save_check_transition(p_sm);
}

void state_save_check_transition(state_machine_t *p_sm)
{
	if (p_sm->data.fault.mode != FAULT_NONE) {
		state_change_state(p_sm, &state_fault);
	} else if (p_sm->data.flag_save_memory_position == true) {
			state_change_state(p_sm, &state_idle);
			p_sm->data.flag_save_memory_position = false;
	}
}

void state_save_exit(state_machine_t *p_sm)
{
	p_sm->data.state.memory_number = 0;
}

void state_auto_entry(state_machine_t *p_sm)
{
	(void)p_sm;
	md_enable_all();
}

void state_auto_process(state_machine_t *p_sm)
{
	if (p_sm->data.cnt_delay_startup_motor >= TIME_ENABLE_MT3) {
		p_sm->data.cnt_delay_startup_motor = TIME_ENABLE_MT3;
	} else p_sm->data.cnt_delay_startup_motor++;

	if (p_sm->data.cnt_delay_startup_motor >= TIME_ENABLE_MT1) {
		state_auto_run_motor(MOTOR_RECLINER, p_sm->data.state.seat_current_axis.recliner, p_sm->data.state.seat_memory_axis.recliner, p_sm->data.measurement.current_slider_q15);
	}

	if (p_sm->data.cnt_delay_startup_motor >= TIME_ENABLE_MT2) {
		state_auto_run_motor(MOTOR_SLIDER, p_sm->data.state.seat_current_axis.slider, p_sm->data.state.seat_memory_axis.slider, p_sm->data.measurement.current_slider_q15);
	}

	if (p_sm->data.cnt_delay_startup_motor >= TIME_ENABLE_MT3) {
		state_auto_run_motor(MOTOR_HEIGHT, p_sm->data.state.seat_current_axis.height, p_sm->data.state.seat_memory_axis.height, p_sm->data.measurement.current_recliner_q15);
	}

	state_auto_check_transition(p_sm);
}

void state_auto_check_transition(state_machine_t *p_sm)
{
	if (p_sm->data.fault.mode != FAULT_NONE) {
		state_change_state(p_sm, &state_fault);
		return;
	}

	if (seat_at_right_position(p_sm)) {
		state_change_state(p_sm, &state_idle);
		return;
	}
}

void state_auto_exit(state_machine_t *p_sm)
{
	p_sm->data.flag_save_current_position = true;
	p_sm->data.cnt_delay_startup_motor    = 0;
}

void state_fault_entry(state_machine_t *p_sm)
{
	(void)p_sm;
	md_stop_all();
}

void state_fault_process(state_machine_t *p_sm)
{
	state_fault_check_transition(p_sm);
}

void state_fault_check_transition(state_machine_t *p_sm)
{
	if (p_sm->data.fault.mode == FAULT_LATCH) {
		state_change_state(p_sm, &state_shut_down);
		return;
	}

	if (p_sm->data.fault.mode == FAULT_AUTO_RECOVER) {
		state_change_state(p_sm, &state_auto_recovery);
		return;
	}
}

void state_fault_exit(state_machine_t *p_sm)
{
	(void)p_sm;
}

void state_auto_recovery_entry(state_machine_t *p_sm)
{
	(void)p_sm;
}

void state_auto_recovery_process(state_machine_t *p_sm)
{

	state_auto_recovery_check_transition(p_sm);
}

void state_auto_recovery_check_transition(state_machine_t *p_sm)
{
	if (GDE_DebounceConditon(system_is_normal(p_sm), TIME_WAIT_FOR_AUTO_RECOVERY_CYCLES, &p_sm->data.cnt_wait_for_auto_recovery)) {
		state_change_state(p_sm, &state_idle);
	}
}

void state_auto_recovery_exit(state_machine_t *p_sm)
{
	(void)p_sm;
	fault_reset();
}

/* ============================================================
 * Sub API
 * ============================================================ */
void state_change_state(state_machine_t *p_sm, state_t *p_state)
{
	p_sm->flag_change_state = true;
	p_sm->next = p_state;
}

void state_install_run_all_motor(state_machine_t *p_sm, MotorStatus_TypeDef status)
{
	if (p_sm->data.cnt_delay_startup_motor >= TIME_ENABLE_MT3) {
		p_sm->data.cnt_delay_startup_motor = TIME_ENABLE_MT3;
	} else p_sm->data.cnt_delay_startup_motor++;

	if (p_sm->data.cnt_delay_startup_motor >= TIME_ENABLE_MT1) {
		if (p_sm->data.motor.recliner.state != BLOCK) {
			MD_Run(MOTOR_RECLINER, status, p_sm->data.measurement.current_recliner_q15);
		}
	}

	if (p_sm->data.cnt_delay_startup_motor >= TIME_ENABLE_MT2) {
		if (p_sm->data.motor.slider.state != BLOCK) {
			MD_Run(MOTOR_SLIDER, status, p_sm->data.measurement.current_slider_q15);
		}
	}

	if (p_sm->data.cnt_delay_startup_motor >= TIME_ENABLE_MT3) {
		if (p_sm->data.motor.height.state != BLOCK) {
			MD_Run(MOTOR_HEIGHT, status, p_sm->data.measurement.current_height_q15);
		}
	}
}

void state_manual_run_motor(MotorName_TypeDef name, Button_TypeDef button_1, Button_TypeDef button_2, uint16_t current_value)
{
	if ((button_1.action == ACTION_HOLD) &&
		(button_2.action == ACTION_RELEASED)){
		MD_Run(name, FORWARD, current_value);
	} else if ((button_2.action == ACTION_HOLD) &&
			   (button_1.action == ACTION_RELEASED)){
				MD_Run(name, BACKWARD, current_value);
	} else MD_Run(name, STOP, current_value);
}

void state_auto_run_motor(MotorName_TypeDef name, uint32_t axis_current, uint32_t axis_memory, uint16_t current_value)
{
	if (axis_current > axis_memory) {
		MD_Run(name, BACKWARD, current_value);
	} else if (axis_current < axis_memory) {
		MD_Run(name, FORWARD, current_value);
	} else MD_Run(name, STOP, current_value);
}

static inline _Bool manual_button_pressed(state_machine_t *p_sm)
{
	return (p_sm->data.button.height_1.action == ACTION_HOLD ||
			p_sm->data.button.height_2.action == ACTION_HOLD ||
			p_sm->data.button.recliner_1.action == ACTION_HOLD ||
			p_sm->data.button.recliner_2.action == ACTION_HOLD ||
			p_sm->data.button.slider_1.action == ACTION_HOLD ||
			p_sm->data.button.slider_2.action == ACTION_HOLD);
}

static inline _Bool all_manual_button_release(state_machine_t *p_sm)
{
	return (p_sm->data.button.height_1.action == ACTION_RELEASED &&
			p_sm->data.button.height_2.action == ACTION_RELEASED &&
			p_sm->data.button.recliner_1.action == ACTION_RELEASED &&
			p_sm->data.button.recliner_2.action == ACTION_RELEASED &&
			p_sm->data.button.slider_1.action == ACTION_RELEASED &&
			p_sm->data.button.slider_2.action == ACTION_RELEASED);
}

static inline _Bool seat_at_wrong_position(state_machine_t *p_sm)
{
	return (p_sm->data.state.seat_current_axis.recliner != p_sm->data.state.seat_memory_axis.recliner ||
			p_sm->data.state.seat_current_axis.height != p_sm->data.state.seat_memory_axis.height ||
			p_sm->data.state.seat_current_axis.slider != p_sm->data.state.seat_memory_axis.slider);
}

static inline _Bool seat_at_right_position(state_machine_t *p_sm)
{
	return (p_sm->data.state.seat_current_axis.recliner == p_sm->data.state.seat_memory_axis.recliner &&
			p_sm->data.state.seat_current_axis.height == p_sm->data.state.seat_memory_axis.height &&
			p_sm->data.state.seat_current_axis.slider == p_sm->data.state.seat_memory_axis.slider);
}

static inline _Bool system_is_normal(state_machine_t *p_sm)
{
	return ((p_sm->data.measurement.current_recliner_q15 <= CURRENT_AUTO_RECOVERY_MT1_THRESHOLD_Q15) &&
			(p_sm->data.measurement.current_slider_q15 <= CURRENT_AUTO_RECOVERY_MT2_THRESHOLD_Q15) &&
		 	(p_sm->data.measurement.current_height_q15 <= CURRENT_AUTO_RECOVERY_MT3_THRESHOLD_Q15) &&
			(p_sm->data.measurement.thermal_recliner_q15 >= THERMAL_AUTO_RECOVERY_MT1_THRESHOLD_Q15) &&
		 	(p_sm->data.measurement.thermal_slider_q15 >= THERMAL_AUTO_RECOVERY_MT2_THRESHOLD_Q15) &&
		 	(p_sm->data.measurement.thermal_height_q15 >= THERMAL_AUTO_RECOVERY_MT3_THRESHOLD_Q15));
}

static inline _Bool all_motor_blocked(state_machine_t *p_sm)
{
	return ((p_sm->data.motor.recliner.state == BLOCK) &&
			(p_sm->data.motor.slider.state == BLOCK) &&
			(p_sm->data.motor.height.state == BLOCK));
}
