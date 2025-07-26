/*
 * adc.c
 *
 *  Created on: Jul 26, 2025
 *      Author: Admin
 */

#include "adc.h"

ADCTypeDef thatADC;
SARADCDriver* driverUser;

uint8_t anp[NUMOFCHANNELS] = {MULSENS_1, MULSENS_2,MULSENS_3};
uint16_t value[NUMOFCHANNELS] = {0U, 0U , 0U};

void saradcconf_conv_cb(SARADCDriver *saradcp) {
    thatADC.ADC_MulSens_1 = saradc_lld_readchannel(saradcp, anp[0]);
    thatADC.ADC_MulSens_2 = saradc_lld_readchannel(saradcp, anp[1]);
    thatADC.ADC_MulSens_3 = saradc_lld_readchannel(saradcp, anp[2]);
}

void saradcconfdma_conv_cb(SARADCDriver *saradcp, uint16_t* buffer, uint16_t len) {
  (void)saradcp;
  (void)buffer;
  (void)len;
}

/**
 * @brief  Initializes the ADCTypeDef structure by resetting all ADC-related values.
 * @param  pADC: Pointer to an ADCTypeDef structure containing ADC configuration and data.
 * @retval int: 0 if success, -1 if invalid parameter (NULL pointer)
 * @note   This function checks for null pointer and initializes all buffer elements.
 */
int ADC_Init(ADCTypeDef *pADC) {
    if (pADC == NULL) {
        /* Invalid parameter: NULL pointer */
        return -1;
    }

    pADC->ADC_MulSens_1   = 0;
    pADC->ADC_MulSens_2   = 0;
    pADC->ADC_MulSens_3   = 0;

    /* change driverUnderTest value to test others SARADC modules. */
    driverUser = &SARADC12DSV;

    /* Start SARADC Driver */
     saradc_lld_start(driverUser, &saradc_config_saradcconf);

     /* start SARADC conversion */
     saradc_lld_start_conversion(driverUser);

    return 0;
}
