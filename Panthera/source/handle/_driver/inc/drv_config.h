/*
 * drv_config.h
 *
 *  Created on: Sep 29, 2025
 *      Author: ADMIN
 */

#ifndef SOURCE_HANDLE__DRIVER_INC_DRV_CONFIG_H_
#define SOURCE_HANDLE__DRIVER_INC_DRV_CONFIG_H_
#include "stdint.h"

#define NUMOFCHANNELS 			4U
#define CHANNEL_MULSENS_1 		90U
#define CHANNEL_MULSENS_2 		82U
#define CHANNEL_MULSENS_3 		63U
#define CHANNEL_FB_5V			58U

typedef struct{
	uint16_t adc_mulsens_1;
	uint16_t adc_mulsens_2;
	uint16_t adc_mulsens_3;
	uint16_t adc_fb_5v;
} adc_value_t;

#endif /* SOURCE_HANDLE__DRIVER_INC_DRV_CONFIG_H_ */
