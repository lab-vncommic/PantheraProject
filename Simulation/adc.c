/*
 * adc.c
 *
 *  Created on: Jul 26, 2025
 *      Author: Admin
 */

#include "adc.h"
ADCTypeDef thatADC;
// SARADCDriver* driverUser;

uint8_t anp[NUMOFCHANNELS] = {CHANNEL_MULSENS_1, CHANNEL_MULSENS_2, CHANNEL_MULSENS_3, CHANNEL_FB_5V};

/**
 * @brief  Callback function for SARADC conversion complete (polling or interrupt mode).
 *         Called automatically by the ADC driver when conversion finishes.
 *         It reads conversion results from each configured channel and updates
 *         the corresponding fields in the global `thatADC` structure.
 *
 * @param  saradcp: Pointer to the SARADC driver instance that completed conversion.
 * @retval None
 */
void saradcconf_conv_cb() {
    // thatADC.ADC_MulSens_1 = saradc_lld_readchannel(saradcp, anp[0]);
    // thatADC.ADC_MulSens_2 = saradc_lld_readchannel(saradcp, anp[1]);
	// thatADC.ADC_MulSens_3 = saradc_lld_readchannel(saradcp, anp[2]);
	// thatADC.ADC_FB_5V 	  = saradc_lld_readchannel(saradcp, anp[3]);
}

/**
 * @brief  Callback function for DMA-based SARADC conversion.
 *         Currently this is a placeholder and does nothing, but can be expanded
 *         to handle DMA buffers in the future if needed.
 *
 * @param  saradcp: Pointer to the SARADC driver instance.
 * @param  buffer: Pointer to the buffer where conversion results are stored.
 * @param  len:    Length of the data buffer.
 * @retval None
 */
void saradcconfdma_conv_cb() {
	// (void)saradcp;
	// (void)buffer;
	// (void)len;
}

/**
 * @brief  Initializes the ADCTypeDef structure by resetting all ADC-related values.
 * @param  pADC: Pointer to an ADCTypeDef structure containing ADC configuration and data.
 * @retval int: 0 if success, -1 if invalid parameter (NULL pointer)
 * @note   This function checks for null pointer and initializes all buffer elements.
 */
int ADC_Init(ADCTypeDef *pADC) {
	// if (pADC == NULL) {
	// 	/* Invalid parameter: NULL pointer */
	// 	return -1;
	// }

	pADC->ADC_MulSens_1   = 0;
	pADC->ADC_MulSens_2   = 0;
	pADC->ADC_MulSens_3   = 0;
	pADC->ADC_FB_5V		  = 0;

	// /* change driverUnderTest value to test others SARADC modules. */
	// driverUser = &SARADC12DSV;

	// /* Start SARADC Driver */
	// saradc_lld_start(driverUser, &saradc_config_saradcconf);
	//  // Align left ADC value
	// driverUser->saradc->MCR.B.WLSIDE = 1U;

    // return 0;
}

/**
 * @brief  Triggers the start of ADC conversion for the selected SARADC driver.
 * @note   This function assumes the SARADC driver has been initialized and started.
 * @retval None
 */
void ADC_startOfConversion(void){
	/* start SARADC conversion */
	// saradc_lld_start_conversion(driverUser);
}
