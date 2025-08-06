/*
 * Panthera.c
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */
#include "Panthera.h"
uint16_t cnt_500ms;
uint16_t cnt;

void PTR_system_initialization(void)
{
	GDE_Config();

	// FIXME: Need to transfer to middleware layer
	UART_Init();

	BTN_Init();
	MEAS_Init();
	FAULT_Init();
	MD_Init();
	STATE_Init(&thisPTR);
	EEPROM_Start();
}
/*====== Execute Time ======*/
/*
 * Time available: 4000 cycles
 * Meas: 118 cycles
 * Fault: 436 cycles
 * Button: 266 cycles
 *
 */
void PTR_main_interrupt_service(void)
{


	// Measure thermal every 1s
	STATE_UpdateMeas(&thisPTR);
//	STATE_UpdateFault(&thisPTR);
	STATE_UpdateButton(&thisPTR);

	// FIXME: Need to transfer to middleware layer
	if (flag_enable_process_comm) {
		begin = stm_lld_getcounter(&STMD3);
//		flag_enable_process_comm = 0;
		// Function to decode received UART message
//		COMM_Receive();
//		COMM_DecodeMessage();
		end = stm_lld_getcounter(&STMD3);
//		COMM_Transmit();
	}

	if (cnt_500ms++ >= 5000) {
		cnt_500ms = 0;
		cnt_rx_success = 0;
	}

	STATE_UpdateState(&thisPTR);


    elapsed = end - begin;


}

void PTR_inf_loop_system(void)
{
	UART_StartRX();

	// Uncomment this code for debugging
//	UART_Monitor();
}
