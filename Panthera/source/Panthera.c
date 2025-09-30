/*
 * Panthera.c
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */
#include "Panthera.h"
CommMode_Enum Comm_Mode = COMM_DEBUG;

void ptr_system_initialization(void)
{
	GDE_Config();
	COMM_Init(Comm_Mode);
	BTN_Init();
	MEAS_Init();
	FAULT_Init();
	MD_Init();
	state_int(&state_machine);
	EEPROM_Start();
}
/*====== Execute Time ======*/
/*
 * Time available:        2000 cycles
 * STATE_UpdateMeas:       208 cycles // Non calculate real value - 1466: Cal real value
 * STATE_UpdateFault:	    81 cycles
 * STATE_UpdateButton:	   340 cycles
 * STATE_UpdateState: 	   630 cycles
 * COMM_ReadRequest:	   498 cycles  || COMM_WriteRequest: 103 cycles
 * Sum					  1646 cycles
 */
void ptr_main_interrupt_service(void)
{
	begin = stm_lld_getcounter(&STMD3);

	// FIXME: New code
	state_update_meas(&state_machine);
	state_update_motor(&state_machine);
	state_update_button(&state_machine);
	state_update_fault(&state_machine);
	state_update_comm(&state_machine);
	state_get_seat_axis(&state_machine);
	state_update_state(&state_machine);

	// Comment these code if run sensorless
//	MD_UpdateAxis(&motor_recliner, thisPTR.measurement.current_recliner_q15);
//	MD_UpdateAxis(&motor_height, thisPTR.measurement.current_height_q15);
//	MD_UpdateAxis(&motor_slider, thisPTR.measurement.current_slider_q15);

	MD_UpdateDriver(&motor_height);
	MD_UpdateDriver(&motor_recliner);
	MD_UpdateDriver(&motor_slider);

	// FIXME: Code to measure execute time
	end = stm_lld_getcounter(&STMD3);
	elapsed = end - begin;
}

void ptr_inf_loop_system(void)
{
	if (Comm_Mode == COMM_DEBUG) {
		// UART for debugging
		if (flag_debug_tx_completed == 1){
			flag_debug_tx_completed = 0;
			sd_lld_write(&SD2, (uint8_t*) BufTXDebug, UART_TX_DEBUG_LENGTH);
		}
		snprintf(BufTXDebug, sizeof(BufTXDebug), "state = %u, fault = %u, %ld, %ld, %ld, %ld, %ld, %ld \r\n",
				state_machine.data.state.current,
				thisFault.fault_status,
				motor_recliner.axis,
				motor_recliner.axis_max,
				motor_slider.axis,
				motor_slider.axis_max,
				motor_height.axis,
				motor_height.axis_max
		);
	} else {
		COMM_DecodeMessage();
		COMM_HandleRequest();
		COMM_CommToState(&state_machine.data.communication);
	}
}

