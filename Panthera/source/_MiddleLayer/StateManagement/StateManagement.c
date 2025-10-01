/*
 * StateManagement.c
 *
 *  Created on: Jul 25, 2025
 *      Author: ADMIN
 */
#include "StateManagement.h"

/* ============================================================
 * IDLE State Functions
 * ============================================================ */
void state_idle_entry(state_machine_t *p_sm);
void state_idle_process(state_machine_t *p_sm);
void state_idle_check_transition(state_machine_t *p_sm);
void state_idle_exit(state_machine_t *p_sm);

/* ============================================================
 * INSTALL State Functions
 * ============================================================ */
void state_install_entry(state_machine_t *p_sm);
void state_install_process(state_machine_t *p_sm);
void state_install_check_transition(state_machine_t *p_sm);
void state_install_exit(state_machine_t *p_sm);

/* ============================================================
 * MANUAL State Functions
 * ============================================================ */
void state_manual_entry(state_machine_t *p_sm);
void state_manual_process(state_machine_t *p_sm);
void state_manual_check_transition(state_machine_t *p_sm);
void state_manual_exit(state_machine_t *p_sm);

/* ============================================================
 * SAVE State Functions
 * ============================================================ */
void state_save_entry(state_machine_t *p_sm);
void state_save_process(state_machine_t *p_sm);
void state_save_check_transition(state_machine_t *p_sm);
void state_save_exit(state_machine_t *p_sm);

/* ============================================================
 * AUTO State Functions
 * ============================================================ */
void state_auto_entry(state_machine_t *p_sm);
void state_auto_process(state_machine_t *p_sm);
void state_auto_check_transition(state_machine_t *p_sm);
void state_auto_exit(state_machine_t *p_sm);

/* ============================================================
 * FAULT State Functions
 * ============================================================ */
void state_fault_entry(state_machine_t *p_sm);
void state_fault_process(state_machine_t *p_sm);
void state_fault_check_transition(state_machine_t *p_sm);
void state_fault_exit(state_machine_t *p_sm);

/* ============================================================
 * AUTO RECOVERY State Functions
 * ============================================================ */
void state_auto_recovery_entry(state_machine_t *p_sm);
void state_auto_recovery_process(state_machine_t *p_sm);
void state_auto_recovery_check_transition(state_machine_t *p_sm);
void state_auto_recovery_exit(state_machine_t *p_sm);

/* ============================================================
 * State Management Utility Functions
 * ============================================================ */
void state_change_state(state_machine_t *p_sm, state_t *p_state);
void state_install_run_all_motor(state_machine_t *p_sm, MotorStatus_TypeDef status);
void state_manual_run_motor(MotorName_TypeDef name, Button_TypeDef button_1, Button_TypeDef button_2, uint16_t current_value);
void state_auto_run_motor(MotorName_TypeDef name, uint32_t axis_current, uint32_t axis_memory, uint16_t current_value);
void state_clear_memory(void);
void state_save_axis_max(state_machine_t *p_sm);
void state_handle_manual_button(Button_TypeDef *pButton, uint16_t comm_value);
void state_handle_recall_button(Button_TypeDef *pButton, uint16_t comm_value);

/* ============================================================
 * Helper Functions
 * ============================================================ */
static inline _Bool manual_button_pressed(state_machine_t *p_sm);
static inline _Bool all_manual_button_release(state_machine_t *p_sm);
static inline _Bool all_recall_button_hold(state_machine_t *p_sm);
static inline _Bool seat_at_wrong_position(state_machine_t *p_sm);
static inline _Bool seat_at_right_position(state_machine_t *p_sm);
static inline _Bool system_is_normal(state_machine_t *p_sm);
static inline _Bool all_motor_blocked(state_machine_t *p_sm);
static inline _Bool recall_button_pressed(Button_TypeDef *p_button);
static inline _Bool state_time_out(state_machine_t *p_sm);

static state_t state_idle = {
		.type = STATE_IDLE,
		.time_out = 0,
		.entry = state_idle_entry,
		.process = state_idle_process,
		.exit = state_idle_exit
};

static state_t state_install = {
		.type = STATE_INSTALL,
		.time_out = TIME_OUT_INSTALL,
		.entry = state_install_entry,
		.process = state_install_process,
		.exit = state_install_exit
};

static state_t state_manual = {
		.type = STATE_MANUAL,
		.time_out = 0,
		.entry = state_manual_entry,
		.process = state_manual_process,
		.exit = state_manual_exit
};

static state_t state_save = {
		.type = STATE_SAVE,
		.time_out = TIME_OUT_SAVE,
		.entry = state_save_entry,
		.process = state_save_process,
		.exit = state_save_exit
};

static state_t state_auto = {
		.type = STATE_AUTO,
		.time_out = TIME_OUT_AUTO,
		.entry = state_auto_entry,
		.process = state_auto_process,
		.exit = state_auto_exit
};

static state_t state_fault = {
		.type = STATE_FAULT,
		.time_out = 0,
		.entry = state_fault_entry,
		.process = state_fault_process,
		.exit = state_fault_exit
};

static state_t state_auto_recovery = {
		.type = STATE_AUTORECOVERY,
		.time_out = 0,
		.entry = state_auto_recovery_entry,
		.process = state_auto_recovery_process,
		.exit = state_auto_recovery_exit
};

static state_t state_shut_down = {
		.type = STATE_SHUTDOWN,
		.time_out = 0,
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

	if (p_sm->data.cnt_change_measure_obj >= (SAMPLE_TIME_MEAS_THERMAL - MIN_MEASUREMENT_SAMPLES)) {
		MD_UpdateMeas(MEAS_THERMAL);
		if (p_sm->data.cnt_change_measure_obj == SAMPLE_TIME_MEAS_THERMAL) {
			MEAS_UpdateValue(&p_sm->data.measurement, MEAS_THERMAL);
			p_sm->data.flag_enable_check_fault = true;
		}
	} else {
		MD_UpdateMeas(MEAS_CURRENT);
		if (p_sm->data.cnt_change_measure_obj >= MIN_MEASUREMENT_SAMPLES) {
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
        if (p_sm->current->exit != NULL) {
            p_sm->current->exit(p_sm);
        }
        
        p_sm->data.state.previous = p_sm->current->type;
		p_sm->data.state.current  = p_sm->next->type;

		p_sm->current = p_sm->next;
		p_sm->cnt_time_on_state = 0;

        // Run entry function of new state
        if (p_sm->current->entry != NULL) {
            p_sm->current->entry(p_sm);
        }
        p_sm->flag_change_state = false;
    } else {
        // Run process function
        p_sm->cnt_time_on_state++;
        if (p_sm->current->process != NULL) {
            p_sm->current->process(p_sm);
        }
    }
}

void state_idle_entry(state_machine_t *p_sm)
{
	if (p_sm->data.flag_save_current_position == true) {
		MEM_Write(p_sm->data.state.seat_current_axis.recliner,
				  p_sm->data.state.seat_current_axis.slider,
				  p_sm->data.state.seat_current_axis.height, ID_MEM_AXIS_CURRENT);
		p_sm->data.flag_save_current_position = false;
	}
	md_stop_all();
}

void state_idle_process(state_machine_t *p_sm)
{
	if (p_sm->data.flag_power_on_mcu == true) {
		MEM_GetSeatAxis(&p_sm->data.state.seat_current_axis, ID_MEM_AXIS_CURRENT);
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
	if (all_recall_button_hold(p_sm)) {
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
		p_sm->data.state.memory_number = ID_MEM_AXIS_RECALL_1;
	else if (p_sm->data.button.recall_2.action == ACTION_HOLD_LONG)
		p_sm->data.state.memory_number = ID_MEM_AXIS_RECALL_2;
	else if (p_sm->data.button.recall_3.action == ACTION_HOLD_LONG)
		p_sm->data.state.memory_number = ID_MEM_AXIS_RECALL_3;
	
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
	MD_ResetToInstall();
	p_sm->data.flag_system_homing = true;
	p_sm->data.flag_install_completed = false;
}

void state_install_process(state_machine_t *p_sm)
{
	if (p_sm->data.flag_system_homing) {
		state_install_run_all_motor(p_sm, BACKWARD);
		if (all_motor_blocked(p_sm)) {
			p_sm->data.flag_system_homing = false;
			md_enable_all();
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

	if (state_time_out(p_sm)) {
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
	if (recall_button_pressed(&p_sm->data.button.recall_1)) {
		p_sm->data.flag_save_memory_position = true;
		p_sm->data.state.memory_number = ID_MEM_AXIS_RECALL_1;
	}

	if (recall_button_pressed(&p_sm->data.button.recall_2)) {
		p_sm->data.flag_save_memory_position = true;
		p_sm->data.state.memory_number = ID_MEM_AXIS_RECALL_2;
	}

	if (recall_button_pressed(&p_sm->data.button.recall_3)) {
		p_sm->data.flag_save_memory_position = true;
		p_sm->data.state.memory_number = ID_MEM_AXIS_RECALL_3;
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

	if (state_time_out(p_sm)) {
		state_change_state(p_sm, &state_idle);
		return;
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
		state_auto_run_motor(MOTOR_RECLINER, p_sm->data.state.seat_current_axis.recliner, p_sm->data.state.seat_memory_axis.recliner, p_sm->data.measurement.current_recliner_q15);
	}

	if (p_sm->data.cnt_delay_startup_motor >= TIME_ENABLE_MT2) {
		state_auto_run_motor(MOTOR_SLIDER, p_sm->data.state.seat_current_axis.slider, p_sm->data.state.seat_memory_axis.slider, p_sm->data.measurement.current_slider_q15);
	}

	if (p_sm->data.cnt_delay_startup_motor >= TIME_ENABLE_MT3) {
		state_auto_run_motor(MOTOR_HEIGHT, p_sm->data.state.seat_current_axis.height, p_sm->data.state.seat_memory_axis.height, p_sm->data.measurement.current_height_q15);
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

	if (state_time_out(p_sm)) {
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

void state_clear_memory(void)
{
	MEM_Write(0, 0, 0, ID_MEM_AXIS_RECALL_1);
	MEM_Write(0, 0, 0, ID_MEM_AXIS_RECALL_2);
	MEM_Write(0, 0, 0, ID_MEM_AXIS_RECALL_3);
}

void state_save_axis_max(state_machine_t *p_sm)
{
	md_update_axis_max(&p_sm->data.motor);
	MEM_Write(p_sm->data.motor.recliner.axis_max,
			  p_sm->data.motor.slider.axis_max,
			  p_sm->data.motor.height.axis_max, ID_MEM_AXIS_MAX);
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

static inline _Bool all_recall_button_hold(state_machine_t *p_sm)
{
	(void)p_sm;
	return ((thisGPIO.recall_1.value) &&
            (thisGPIO.recall_2.value) &&
            (thisGPIO.recall_3.value));
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

static inline _Bool recall_button_pressed(Button_TypeDef *p_button)
{
	return ((p_button->action == ACTION_PRESSED) ||
			(p_button->action == ACTION_HOLD) ||
			(p_button->action == ACTION_HOLD_LONG));
}

static inline _Bool state_time_out(state_machine_t *p_sm)
{
	return (p_sm->cnt_time_on_state >= p_sm->current->time_out);
}
