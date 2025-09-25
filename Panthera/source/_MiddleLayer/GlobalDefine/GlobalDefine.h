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

#define F_SAMPLE	20000.0f
#define T_SAMPLE	1/20000.0f

typedef enum{
	MOTOR_NONE		= 0,
	MOTOR_SLIDER	= 1,
	MOTOR_HEIGHT	= 2,
	MOTOR_RECLINER	= 3
} MotorName_TypeDef;

typedef struct{
	uint32_t	height;
	uint32_t	recliner;
	uint32_t	slider;
} SeatAxis_TypeDef;

typedef enum{
	SOFTSTART  		= 0,
	STEADY 			= 1,
	BLOCK			= 2
} MotorState_TypeDef;

typedef struct{
	MotorState_TypeDef state;
	int32_t			   axis_max;
} motor_system_o;

typedef struct{
	motor_system_o recliner;
	motor_system_o slider;
	motor_system_o height;
} motor_system_t;

void GDE_Config(void);
_Bool GDE_DebounceConditon(_Bool condition, uint32_t timeout_cycles, uint32_t *pCounter);

extern uint32_t begin, end, elapsed;
#endif /* MIDDLELAYER_GLOBALDEFINE_GLOBALDEFINE_H_ */
