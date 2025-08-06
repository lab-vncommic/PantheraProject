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
char subBufRX[UART_RX_LENGTH];
char bufByte[1];

uint8_t  num = 0;
uint8_t  cnt_rx_success = 0;
uint16_t flag_uart_tx_completed;
uint16_t flag_uart_rx_completed;
uint16_t flag_enable_process_comm;
uint32_t temp_num;

uint32_t uart_test_1;
float 	 uart_test_2;

void UART_AntiDisturbance(void);

void UART_Init(void)
{
	sd_lld_start(&SD2,&serial_config_My_UART);
	
	memset(BufTXDebug, 0, UART_TX_DEBUG_LENGTH);
	memset(BufTX, 0, UART_TX_LENGTH);
	memset(BufRX, 0, UART_RX_LENGTH);
	memset(subBufRX, 0, UART_RX_LENGTH);
	bufByte[0] = 0;
	
	flag_uart_tx_completed 		= 1;
	flag_uart_rx_completed 		= 1;
	flag_enable_process_comm    = 0;
}

void UART_Monitor(void)
{
	if (flag_uart_tx_completed == 1){
		flag_uart_tx_completed = 0;
		sd_lld_write(&SD2, (uint8_t*) BufTXDebug, UART_TX_DEBUG_LENGTH);
	}
	snprintf(BufTXDebug, sizeof(BufTXDebug), "uart_test_1 = %ld, uart_test_2 = %f aaaa\r\n",
			uart_test_1,
			uart_test_2
	);
}

void UART_StartRX(void)
{
	if (flag_uart_rx_completed == 1) {
		flag_uart_rx_completed = 0;
		sd_lld_read(&SD2, (uint8_t*) bufByte, 1);
	}
}

void UART_AntiDisturbance(void)
{
	if (bufByte[0] >= '0' && bufByte[0] <= '9') {
		subBufRX[num] = bufByte[0];
		num++;

		if (num == UART_RX_LENGTH) {
			num = 0;
		}
	}
}

void UART_TX_Callback(SerialDriver *sdp)
{
	(void)sdp;
	flag_uart_tx_completed = 1;
}

void UART_RX_Callback(SerialDriver *sdp)
{
	(void)sdp;
	cnt_rx_success++;
	UART_AntiDisturbance();
	flag_uart_rx_completed = 1;

	if (cnt_rx_success >= 6) {
		memcpy(BufRX, subBufRX, UART_RX_LENGTH);
		flag_enable_process_comm = 1;
	}
}

void *sbrk(ptrdiff_t incr) {
  extern uint8_t __heap_base__;
  extern uint8_t __heap_end__;
  static uint8_t *p = &__heap_base__;
  uint8_t *newp;

  newp = p + incr;
  if (newp > &__heap_end__) {
	errno = ENOMEM;
	return (void *)-1;
  }

  uint8_t *oldp = p;
  p = newp;
  return oldp;
}
