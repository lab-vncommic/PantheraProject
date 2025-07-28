/*
 * Panthera.c
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */
#include "Panthera.h"
uint16_t cnt_500ms;

void PTR_system_initialization(void)
{
	GDE_Config();
	UART_Init();
	MEAS_Init();
	FAULT_Init();
}

/*====== Execute Time ======*/
/*
 * Total: 4000 cycles
 *
 *
 */
void PTR_main_interrupt_service(void)
{
	begin = stm_lld_getcounter(&STMD3);
	if (cnt_500ms++ >= 5000) {
		cnt_500ms = 0;
		GPIO_TogglePin(&PIN_SEL0_2);
	}

	MEAS_GetValue();
	FAULT_UpdateStatus();

    end = stm_lld_getcounter(&STMD3);
    elapsed = end - begin;
}

void PTR_inf_loop_system(void)
{
	if (uart_tx_flag == 1){
		uart_tx_flag = 0;
		sd_lld_write(&SD2, (uint8_t*) BufTX, UART_TX_LENGTH);
	}
    snprintf(BufTX, sizeof(BufTX), "uart_test_1 = %ld, uart_test_2 = %f \r\n",
    		uart_test_1,
			uart_test_2
    );
}
