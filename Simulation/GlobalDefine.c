/*
 * GlobalDefine.c
 *
 *  Created on: Jul 24, 2025
 *      Author: ADMIN
 */
#include "GlobalDefine.h"
uint32_t begin, end, elapsed;

/**
 * @brief System configuration initialization
 *
 * @details This function initializes and configures the system components:
 *          - Configures STM timer (STMD3) for execution time measurement
 *          - Sets up PIT timer for 10kHz interrupt service routine
 *
 * @return None
 *
 * @note Must be called during system initialization before using any timer
 *       or interrupt functionality
 */
void GDE_Config(void)
{
	// Config timer to measure execute time
	// stm_lld_start(&STMD3,stm2_config);
	// stm_lld_enable(&STMD3);

	// // Config ISR 10kHz
	// pit_lld_start(&PITD1, pit0_config);
	// pit_lld_channel_start(&PITD1,PIT0_CHANNEL_CH1);
}

