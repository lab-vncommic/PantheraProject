/*
 * GlobalDefine.h
 *
 *  Created on: Jul 24, 2025
 *      Author: ADMIN
 */

#ifndef MIDDLELAYER_GLOBALDEFINE_GLOBALDEFINE_H_
#define MIDDLELAYER_GLOBALDEFINE_GLOBALDEFINE_H_
#include "components.h"
#include "stm_lld_cfg.h"
#include "pit_lld_cfg.h"

#define F_SAMPLE	10000.0f
#define T_SAMPLE	1/10000.0f

typedef enum{
	MOTOR_NONE		= 0,
	MOTOR_SLIDER	= 1,
	MOTOR_HEIGHT	= 2,
	MOTOR_RECLINER	= 3
} MotorName_TypeDef;

typedef struct{
	uint16_t	height;
	uint16_t	recliner;
	uint16_t	slider;
} SeatAxis_TypeDef;

void GDE_Config(void);

extern uint32_t begin, end, elapsed;
#endif /* MIDDLELAYER_GLOBALDEFINE_GLOBALDEFINE_H_ */
