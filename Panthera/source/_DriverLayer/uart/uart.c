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

uint16_t uart_tx_flag;
uint16_t uart_rx_flag;
uint16_t uart_process_flag;

uint32_t uart_test_1;
float 	 uart_test_2;

/**
 * @brief Initializes the UART peripheral
 * @details Starts the serial driver with the appropriate configuration
 *          and sets the transmit flag to indicate ready state
 * @return None
 */
void UART_Init(void)
{
	sd_lld_start(&SD2,&serial_config_My_UART);
	uart_tx_flag 		= 1;
	uart_rx_flag 		= 1;
	uart_process_flag   = 0;
}

/**
 * @brief Monitors and sends debug information through UART
 * @details Checks if UART transmission is ready, sends the current buffer,
 *          then formats a new message with test values to be sent in the next cycle
 * @return None
 */
void UART_Monitor(void)
{
	if (uart_tx_flag == 1){
		uart_tx_flag = 0;
		sd_lld_write(&SD2, (uint8_t*) BufTXDebug, UART_TX_DEBUG_LENGTH);
	}
    snprintf(BufTXDebug, sizeof(BufTXDebug), "uart_test_1 = %ld, uart_test_2 = %f aaaa\r\n",
    		uart_test_1,
			uart_test_2
    );
}
/**
 * @brief Initiates UART receive operation
 * @details Checks if the receive flag is set, then starts a new read operation
 *          from the UART peripheral into the receive buffer
 * @return None
 */
void UART_StartRX(void)
{
	if (uart_rx_flag == 1) {
		uart_rx_flag = 0;
		sd_lld_read(&SD2, (uint8_t*) BufRX, UART_RX_LENGTH);
	}
}

/**
 * @brief Callback function for UART transmission completion
 * @details Sets the transmit flag to indicate that the UART is ready for
 *          the next transmission
 * @param[in] sdp Pointer to the serial driver
 * @return None
 */
void UART_TX_Callback(SerialDriver *sdp)
{
	(void)sdp;
	uart_tx_flag = 1;
}

/**
 * @brief Callback function for UART receive events
 * @details Currently empty implementation for handling received data
 * @param[in] sdp Pointer to the serial driver
 * @return None
 */
void UART_RX_Callback(SerialDriver *sdp)
{
	(void)sdp;
	uart_rx_flag = 1;
	uart_process_flag = 1;
}

/**
 * @brief Implementation of sbrk to support sprintf/snprintf functions
 * @details This function provides dynamic memory allocation required by
 *          standard C library functions. It maintains a pointer to the
 *          current heap position and increments it as memory is allocated.
 *          If the heap would exceed its maximum allowed size, an error is returned.
 * @param[in] incr Number of bytes to allocate
 * @return void* Pointer to the allocated memory block, or (void*)-1 if out of memory
 */
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
