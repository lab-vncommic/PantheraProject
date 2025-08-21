/*
 * Panthera.c
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */
#include "Panthera.h"
uint16_t cnt_blink_led;
CommMode_Enum Comm_Mode = COMM_DEBUG;
uint16_t temp;

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
	begin = stm_lld_getcounter(&STMD3);
	// FIXME: Blink led for debug, comment code blink led and uncomment MD_UpdateDriver()
	if (cnt_blink_led++ >= 1000) {
		cnt_blink_led = 0;
		GPIO_TogglePin(&PIN_HALL_2);
	};

//	STATE_UpdateMeas(&thisPTR);								// Measure thermal every 1s

	MD_UpdateMeas(MEAS_CURRENT);
	MEAS_UpdateValue(&thisPTR.measurement, MEAS_CURRENT);

	STATE_UpdateButton(&thisPTR);

	STATE_UpdateState(&thisPTR);


	// FIXME: Test motor driver

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
	} else COMM_TransmitReceiveHandle();
}
