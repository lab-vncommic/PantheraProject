/*
 * uart.c
 *
 *  Created on: Jul 25, 2025
 *      Author: ADMIN
 */
#include "uart.h"
char BufTXDebug[UART_TX_DEBUG_LENGTH];
char BufTX[UART_TX_LENGTH];
char BufRX[UART_RX_LENGTH];

_Bool flag_debug_tx_completed;
_Bool flag_enable_tx;
_Bool flag_enable_rx;
_Bool flag_rx_fail;
_Bool flag_rx_success;

void UART_Init(void)
{
	// sd_lld_start(&SD2,&serial_config_My_UART);
	
	memset(BufTXDebug, 0, UART_TX_DEBUG_LENGTH);
	BufTX[1] = '\r';
	BufTX[2] = '\n';
	memset(BufRX, '0', UART_RX_LENGTH);

	flag_debug_tx_completed  = true;
	flag_enable_rx 		 	 = true;
}

void UART_Monitor(void)
{
	if (flag_debug_tx_completed == 1){
		flag_debug_tx_completed = 0;
		// sd_lld_write(&SD2, (uint8_t*) BufTXDebug, UART_TX_DEBUG_LENGTH);
	}
	snprintf(BufTXDebug, sizeof(BufTXDebug), "cur_slider = %f, cur_height = %f, cur_recliner = %f\r\n",
			thisMeas.current_slider.value_f32,
			thisMeas.current_height.value_f32,
			thisMeas.current_recliner.value_f32
	);
}

void UART_HandleRequest(void)
{
	if (flag_enable_rx) {
		// sd_lld_read(&SD2, (uint8_t*) BufRX, UART_RX_LENGTH);
		// osalThreadDelayMilliseconds(300);
		flag_enable_rx = 0;
	}

	if (flag_enable_tx == 1) {
		// sd_lld_write(&SD2, (uint8_t*) BufTX, UART_TX_LENGTH);
		flag_enable_tx = 0;
	}

	if (flag_rx_fail == true) {
		// sd_lld_write(&SD2, (uint8_t*) "FAIL\r\n", 6);
		flag_rx_fail = false;
	}

	if (flag_rx_success == true) {
		// sd_lld_write(&SD2, (uint8_t*) "SUCCESS\r\n", 9);
		flag_rx_success = false;
	}
}

// void UART_TX_Callback(SerialDriver *sdp)
// {
// 	(void)sdp;
// 	flag_debug_tx_completed = 1;
// }

// void *sbrk(ptrdiff_t incr) {
//   extern uint8_t __heap_base__;
//   extern uint8_t __heap_end__;
//   static uint8_t *p = &__heap_base__;
//   uint8_t *newp;

//   newp = p + incr;
//   if (newp > &__heap_end__) {
// 	errno = ENOMEM;
// 	return (void *)-1;
//   }

//   uint8_t *oldp = p;
//   p = newp;
//   return oldp;
// }
