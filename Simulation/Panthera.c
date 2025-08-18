/*
 * Panthera.c
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */
#include "Panthera.h"
uint16_t cnt_50ms;
CommMode_Enum Comm_Mode = COMM_DEBUG;

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
	motor_height.status = FORWARD;
}
/*====== Execute Time ======*/
/*
 * Time available:        4000 cycles
 * STATE_UpdateMeas:       176 cycles // Non calculate real value - 1466: Cal real value
 * STATE_UpdateFault:	    18 cycles
 * STATE_UpdateButton:	   324 cycles
 * STATE_UpdateState: 	   630 cycles
 * COMM_ReadRequest:	   498 cycles  || COMM_WriteRequest: 103 cycles
 * Sum					  1646 cycles
 */
void PTR_main_interrupt_service(void)
{
	// begin = stm_lld_getcounter(&STMD3);
	// FIXME: Blink led for debug, comment code blink led and uncomment MD_UpdateDriver()
	if (cnt_50ms++ >= 500) {
		cnt_50ms = 0;
		// GPIO_TogglePin(&PIN_SEL0_2);
	}

	STATE_UpdateMeas(&thisPTR);								// Measure thermal every 1s
	MD_UpdateAxis(&motor_height, thisMeas.current_height.value_q15);
////	STATE_UpdateFault(&thisPTR);
//	STATE_UpdateButton(&thisPTR);
//	STATE_UpdateState(&thisPTR);

    elapsed = end - begin;
}

void PTR_inf_loop_system(void)
{
	if (Comm_Mode == COMM_DEBUG) {
		// // UART for debugging
		// if (flag_debug_tx_completed == 1){
		// 	flag_debug_tx_completed = 0;
		// 	sd_lld_write(&SD2, (uint8_t*) BufTXDebug, UART_TX_DEBUG_LENGTH);
		// }
		// snprintf(BufTXDebug, sizeof(BufTXDebug), "state = %d, %ld, %u, %u, %u\r\n",
		// 		thisPTR.state,
		// 		thisPTR.seat_current_axis.height,
		// 		thisPTR.state_home.flag_height_block,
		// 		thisPTR.state_home.flag_slider_block,
		// 		thisPTR.state_home.flag_recliner_block
		// );
	} else COMM_TransmitReceiveHandle();
}
