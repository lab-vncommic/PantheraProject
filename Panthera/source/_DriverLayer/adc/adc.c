/*
 * adc.c
 *
 *  Created on: Jul 25, 2025
 *      Author: Admin
 */

# include "adc.h"

SARADCDriver* driverUser;
ADCTypeDef thatADC;

/**
 * @brief  Initializes the ADCTypeDef structure by resetting all ADC-related values.
 * @param  pADC: Pointer to an ADCTypeDef structure containing ADC configuration and data.
 * @retval int: 0 if success, -1 if invalid parameter (NULL pointer)
 * @note   This function checks for null pointer and initializes all buffer elements.
 */
int ADC_Init(ADCTypeDef *pADC) {
    int i;
    if (pADC == NULL) {
        /* Invalid parameter: NULL pointer */
        return -1;
    }

    for (i = 0; i < ADC_BUFF_DEPTH; i++) {
        pADC->BufADC[i] = 0;
    }
    pADC->ADC_MulSens_1   = 0;
    pADC->ADC_MulSens_2   = 0;
    pADC->ADC_MulSens_3   = 0;

    /* change driverUnderTest value to test others SARADC modules. */
    driverUser = &SARADC12DSV;

    /* Start SARADC Driver */
    saradc_lld_start(driverUser, &saradc_config_saradcconfdma);

    /* start SARADC conversion */
    saradc_lld_start_conversion(driverUser);

    return 0;
}

/* conversion callback when using DMA */
void saradcconfdma_conv_cb(SARADCDriver *saradcp, uint16_t* buffer, uint16_t len) {
  thatADC.ADC_MulSens_1 = buffer[0];
  thatADC.ADC_MulSens_2 = buffer[1];
  thatADC.ADC_MulSens_3 = buffer[2];

  thatADC.lenBuffer = len;
  thatADC.channel = saradcp->config->dmacfg.number;
}

/* Dummy callback for configuration not used in DMA test */
void saradcconf_conv_cb(SARADCDriver *saradcp) {
  (void)saradcp;
}
