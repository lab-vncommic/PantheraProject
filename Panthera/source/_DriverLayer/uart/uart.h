/*
 * uart.h
 *
 *  Created on: Jul 25, 2025
 *      Author: ADMIN
 */

#ifndef SOURCE__DRIVERLAYER_UART_UART_H_
#define SOURCE__DRIVERLAYER_UART_UART_H_
#include "components.h"
#include "serial_lld_cfg.h"
#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include "string.h"

#define UART_TX_DEBUG_LENGTH 		100
#define UART_TX_LENGTH				1
#define UART_RX_LENGTH 				6

void UART_Init(void);
void UART_Monitor(void);
void UART_StartRX(void);
void UART_AntiDisturbance(void);

void UART_TX_Callback(SerialDriver *sdp);
void UART_RX_Callback(SerialDriver *sdp);
void *sbrk(ptrdiff_t incr);

extern char BufTXDebug[UART_TX_DEBUG_LENGTH];
extern char BufTX[UART_TX_LENGTH];
extern char BufRX[UART_RX_LENGTH];
extern char subBufRX[UART_RX_LENGTH];
extern uint16_t flag_uart_tx_completed;
extern uint16_t flag_uart_rx_completed;
extern uint16_t flag_enable_process_comm;
extern uint32_t uart_test_1;
extern float 	uart_test_2;
extern char bufByte[1];
extern uint8_t  num;
extern uint8_t  cnt_rx_success;
#endif /* SOURCE__DRIVERLAYER_UART_UART_H_ */
