/*
 * adc.h
 *
 *  Created on: Jul 25, 2025
 *      Author: Admin
 */

#ifndef SOURCE_ADC_ADC_H_
#define SOURCE_ADC_ADC_H_

#include "saradc_lld_cfg.h"

#define ADC_BUFF_DEPTH	3


typedef struct{
	uint16_t *BufADC[ADC_BUFF_DEPTH];
	uint16_t lenBuffer;
	uint16_t channel;

	uint16_t ADC_MulSens_1;
	uint16_t ADC_MulSens_2;
	uint16_t ADC_MulSens_3;
}ADCTypeDef;

int ADC_Init(ADCTypeDef *pADC);
void saradcconfdma_conv_cb(SARADCDriver *saradcp, uint16_t* buffer, uint16_t len) ;
void saradcconf_conv_cb(SARADCDriver *saradcp);

extern ADCTypeDef thatADC;
extern SARADCDriver* driverUser;
#endif /* SOURCE_ADC_ADC_H_ */
