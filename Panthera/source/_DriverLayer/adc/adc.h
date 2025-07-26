/*
 * adc.h
 *
 *  Created on: Jul 26, 2025
 *      Author: Admin
 */

#ifndef SOURCE_ADC_ADC_H_
#define SOURCE_ADC_ADC_H_

#include "saradc_lld_cfg.h"

#define NUMOFCHANNELS 3U
#define MULSENS_1 		90U
#define MULSENS_2 		82U
#define MULSENS_3 		63U

typedef struct{
	uint16_t ADC_MulSens_1;
	uint16_t ADC_MulSens_2;
	uint16_t ADC_MulSens_3;
}ADCTypeDef;

int ADC_Init(ADCTypeDef *pADC);
void ADC_startOfConversion(void);

extern uint8_t anp[NUMOFCHANNELS];
extern uint16_t value[NUMOFCHANNELS];
extern ADCTypeDef thatADC;
extern SARADCDriver* driverUser;
#endif /* SOURCE_ADC_ADC_H_ */
