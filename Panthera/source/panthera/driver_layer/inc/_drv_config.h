/*
 * drv_config.h
 *
 *  Created on: Sep 29, 2025
 *      Author: ADMIN
 */

#ifndef SOURCE_PANTHERA_DRIVER_LAYER_INC__DRV_CONFIG_H_
#define SOURCE_PANTHERA_DRIVER_LAYER_INC__DRV_CONFIG_H_
#include "stdint.h"

#define NUMBER_OF_GPIO			31

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

typedef enum{
	GPIO_RECLINER_1 = 0,
	GPIO_RECLINER_2 = 1,
	GPIO_SLIDER_1   = 2,
	GPIO_SLIDER_2   = 3,
	GPIO_HEIGHT_1   = 4,
	GPIO_HEIGHT_2   = 5,
	GPIO_SAVE 	    = 6,
	GPIO_RECALL_1   = 7,
	GPIO_RECALL_2   = 8,
	GPIO_RECALL_3   = 9,

	GPIO_INA_1    = 10,
	GPIO_INB_1    = 11,
	GPIO_SEL0_1   = 12,
	GPIO_SEL1_1   = 13,
	GPIO_MulEN_1  = 14,
	GPIO_PWM_1    = 15,
	GPIO_HALL_1   = 16,
	
	GPIO_INA_2    = 17,
	GPIO_INB_2    = 18,
	GPIO_SEL0_2   = 19,
	GPIO_SEL1_2   = 20,
	GPIO_MulEN_2  = 21,
	GPIO_PWM_2    = 22,
	GPIO_HALL_2   = 23,
	
	GPIO_INA_3    = 24,
	GPIO_INB_3    = 25,
	GPIO_SEL0_3   = 26,
	GPIO_SEL1_3   = 27,
	GPIO_MulEN_3  = 28,
	GPIO_PWM_3    = 29,
	GPIO_HALL_3   = 30
} gpio_id_e;

typedef struct{
	uint8_t recliner_1;
	uint8_t recliner_2;
	uint8_t slider_1;
	uint8_t slider_2;
	uint8_t height_1;
	uint8_t height_2;
	uint8_t save;
	uint8_t recall_1;
	uint8_t recall_2;
	uint8_t recall_3;
} gpio_btn_value_t;

#endif /* SOURCE_PANTHERA_DRIVER_LAYER_INC__DRV_CONFIG_H_ */
