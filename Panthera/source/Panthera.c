/*
 * Panthera.c
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */
#include "Panthera.h"
uint16_t cnt_50ms;

void PTR_system_initialization(void)
{
	GDE_Config();
	COMM_Init();
	BTN_Init();
	MEAS_Init();
	FAULT_Init();
	MD_Init();
	STATE_Init(&thisPTR);
	EEPROM_Start();
}
/*====== Execute Time ======*/
/*
 * Time available:        8000 cycles
 * STATE_UpdateMeas:       500 cycles
 * STATE_UpdateFault:	   592 cycles
 * STATE_UpdateButton:	   781 cycles
 * STATE_UpdateState: 	  1400 cycles
 * COMM_ReadRequest:	   498 cycles  || COMM_WriteRequest: 103 cycles
 * Sum					  3671 cycles
 */
void PTR_main_interrupt_service(void)
{
	// FIXME: Blink led for debug, comment code blink led and uncomment MD_UpdateDriver()
	if (cnt_50ms++ >= 500) {
		cnt_50ms = 0;
		GPIO_TogglePin(&PIN_SEL0_2);
	}

	begin = stm_lld_getcounter(&STMD3);
	STATE_UpdateMeas(&thisPTR);								// Measure thermal every 1s
//	STATE_UpdateFault(&thisPTR);
	STATE_UpdateButton(&thisPTR);
	STATE_UpdateState(&thisPTR);
	end = stm_lld_getcounter(&STMD3);
    elapsed = end - begin;
}

void PTR_inf_loop_system(void)
{
	COMM_TransmitReceiveHandle();

	// UART for debugging
//	if (flag_debug_tx_completed == 1){
//		flag_debug_tx_completed = 0;
//		sd_lld_write(&SD2, (uint8_t*) BufTXDebug, UART_TX_DEBUG_LENGTH);
//	}
//	snprintf(BufTXDebug, sizeof(BufTXDebug), "elapsed = %d, state = %d, %u, cur_slider = %d, cur_height = %d, cur_recliner = %d\r\n",
//			elapsed,
//			thisPTR.state,
//			thisPTR.seat_current_axis.height,
//			thisPTR.state_home.flag_height_block,
//			thisPTR.state_home.flag_slider_block,
//			thisPTR.state_home.flag_recliner_block
//	);
}
