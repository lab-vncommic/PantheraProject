/*
 * Panthera.c
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */
#include "Panthera.h"
uint16_t cnt_blink_led;
CommMode_Enum Comm_Mode = COMM_UART;

void PTR_system_initialization(void)
{
	GDE_Config();
	COMM_Init(Comm_Mode);
	BTN_Init();
	MEAS_Init();
	FAULT_Init();
	MD_Init();
	STATE_Init(&thisPTR);
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
void PTR_main_interrupt_service(void)
{
	begin = stm_lld_getcounter(&STMD3);
	if (thisPTR.state == STATE_IDLE) {
		// Turn off mulsense
		MD_UpdateMeas(MEAS_NONE);
	} else {
		MD_UpdateMeas(MEAS_CURRENT);
		MEAS_UpdateValue(&thisPTR.measurement, MEAS_CURRENT);
	}


//	STATE_UpdateMeas(&thisPTR);								// Measure thermal every 1s
	STATE_UpdateButton(&thisPTR);
	STATE_UpdateFault(&thisPTR);
	STATE_UpdateState(&thisPTR);

	MD_UpdateAxis(&motor_recliner, thisPTR.measurement.current_recliner_q15);
	MD_UpdateAxis(&motor_height, thisPTR.measurement.current_height_q15);
	MD_UpdateAxis(&motor_slider, thisPTR.measurement.current_slider_q15);

	MD_UpdateDriver(&motor_height);
	MD_UpdateDriver(&motor_recliner);
	MD_UpdateDriver(&motor_slider);

	end = stm_lld_getcounter(&STMD3);
    elapsed = end - begin;
}

void PTR_inf_loop_system(void)
{
	if (Comm_Mode == COMM_DEBUG) {
		// UART for debugging
		if (flag_debug_tx_completed == 1){
			flag_debug_tx_completed = 0;
			sd_lld_write(&SD2, (uint8_t*) BufTXDebug, UART_TX_DEBUG_LENGTH);
		}
		snprintf(BufTXDebug, sizeof(BufTXDebug), "state = %u, state_motor = %u, flag = %u \r\n",
				thisPTR.state,
				motor_recliner.state,
				motor_recliner.flag_available
		);
	} else {
		COMM_DecodeMessage();
		COMM_HandleRequest();
		COMM_CommToState(&thisPTR.communication);
	}
}

