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

#define UART_TX_LENGTH 100

void UART_Init(void);
void UART_Monitor(void);

void UART_TX_Callback(SerialDriver *sdp);
void UART_RX_Callback(SerialDriver *sdp);
void *sbrk(ptrdiff_t incr);

extern char BufTX[UART_TX_LENGTH];
extern uint16_t uart_tx_flag;
extern uint32_t uart_test_1;
extern float 	uart_test_2;
#endif /* SOURCE__DRIVERLAYER_UART_UART_H_ */
