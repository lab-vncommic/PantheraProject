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
	begin = stm_lld_getcounter(&STMD3);

	// Measure thermal every 1s
	STATE_GetMeasValue();

	FAULT_UpdateMode();
	BTN_Update();

	// FIXME: Need to transfer to middleware layer
	if (uart_process_flag) {
		uart_process_flag = 0;
		// Function to decode received UART message

		sd_lld_write(&SD2, (uint8_t*) BufTX, UART_TX_LENGTH);
	}

	STATE_UpdateState(&thisPTR);

	end = stm_lld_getcounter(&STMD3);
    elapsed = end - begin;
}

void PTR_inf_loop_system(void)
{
	UART_StartRX();

	// Uncomment this code for debugging
//	UART_Monitor();
}
