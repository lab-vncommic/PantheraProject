/*
 * uart.c
 *
 *  Created on: Jul 25, 2025
 *      Author: ADMIN
 */
#include "uart.h"

/** @brief Buffer for UART transmission data */
char BufTX[UART_TX_LENGTH];

/** @brief Flag indicating UART transmit status (1 = ready to transmit) */
uint16_t uart_tx_flag;

/** @brief Test variable for UART debugging */
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
	uart_tx_flag = 1;
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
