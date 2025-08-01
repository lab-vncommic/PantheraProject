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
	MD_Init();
	STATE_Init(&thisPTR);
	EEPROM_Start();

	/**************************************Test memory****************************************/
	//ID 1
	EEPROM_Restart();
	thisPTR.seat_axis.height 	= 12;
	thisPTR.seat_axis.recliner 	= 25;
	thisPTR.seat_axis.slider 	= 15;
	MEM_Write(thisPTR.seat_axis.height, thisPTR.seat_axis.recliner, thisPTR.seat_axis.slider, ID_EEPROM_1);
	MEM_Read(ID_EEPROM_1);

////	ID2
//	uint32_t test_id2_a = 60;
//	uint32_t test_id2_b = 62;
//	uint32_t test_id2_c = 64;
//	MEM_Write(test_id2_a, test_id2_b, test_id2_c, ID_EEPROM_2);
//	MEM_Read(ID_EEPROM_2);
//
////	ID3
//	uint32_t test_id3_a = 50;
//	uint32_t test_id3_b = 51;
//	uint32_t test_id3_c = 52;
//	MEM_Write(test_id3_a, test_id3_b, test_id3_c, ID_EEPROM_3);
//	MEM_Read(ID_EEPROM_3);
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

	MEAS_GetValue();
	FAULT_UpdateMode();

	STATE_UpdateState(&thisPTR);

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
