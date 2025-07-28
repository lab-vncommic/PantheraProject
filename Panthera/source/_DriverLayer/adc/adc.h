/*
 * adc.h
 *
 *  Created on: Jul 26, 2025
 *      Author: Admin
 */

#ifndef SOURCE_ADC_ADC_H_
#define SOURCE_ADC_ADC_H_

#include "saradc_lld_cfg.h"

#define NUMOFCHANNELS 4U
#define CHANNEL_MULSENS_1 		90U
#define CHANNEL_MULSENS_2 		82U
#define CHANNEL_MULSENS_3 		63U
#define CHANNEL_FB_5V			58U

typedef struct{
	uint16_t ADC_MulSens_1;
	uint16_t ADC_MulSens_2;
	uint16_t ADC_MulSens_3;
	uint16_t ADC_FB_5V;
}ADC_TypeDef;

int ADC_Init(ADC_TypeDef *pADC);
void ADC_startOfConversion(void);

extern uint8_t anp[NUMOFCHANNELS];
extern uint16_t value[NUMOFCHANNELS];
extern ADC_TypeDef thatADC;
extern SARADCDriver* driverUser;
#endif /* SOURCE_ADC_ADC_H_ */
