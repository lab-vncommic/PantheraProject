/*
 * uart.h
 *
 *  Created on: Jul 25, 2025
 *      Author: ADMIN
 */

#ifndef SOURCE__DRIVERLAYER_UART_UART_H_
#define SOURCE__DRIVERLAYER_UART_UART_H_
// #include "components.h"
// #include "serial_lld_cfg.h"
#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include "string.h"

#define UART_TX_DEBUG_LENGTH 		100
#define UART_TX_LENGTH				3
#define UART_RX_LENGTH 				6

void UART_Init(void);
void UART_Monitor(void);
void UART_HandleRequest(void);

// void UART_TX_Callback(SerialDriver *sdp);

extern char BufTXDebug[UART_TX_DEBUG_LENGTH];
extern char BufTX[UART_TX_LENGTH];
extern char BufRX[UART_RX_LENGTH];
extern _Bool flag_debug_tx_completed;
extern _Bool flag_enable_tx;
extern _Bool flag_enable_rx;
extern _Bool flag_rx_fail;
extern _Bool flag_rx_success;
#endif /* SOURCE__DRIVERLAYER_UART_UART_H_ */
