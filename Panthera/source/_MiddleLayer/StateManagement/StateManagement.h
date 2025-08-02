/*
 * StateManagement.h
 *
 *  Created on: Jul 25, 2025
 *      Author: ADMIN
 */

#ifndef SOURCE__MIDDLELAYER_STATEMANAGEMENT_STATEMANAGEMENT_H_
#define SOURCE__MIDDLELAYER_STATEMANAGEMENT_STATEMANAGEMENT_H_
#include "components.h"
#include "MotorDriver.h"
#include "Measurement.h"
#include "FaultManagement.h"
#include "Button.h"
#include "GlobalDefine.h"
#include "memory.h"

#define TIMEOUT_SAVE_AXIS        50000   // Delay 5s between t position saves

typedef enum{
	STATE_IDLE		   = 0,
	STATE_MANUAL	   = 1,
	STATE_AUTO		   = 2,
	STATE_FAULT		   = 3,
	STATE_AUTORECOVERY = 4,
	STATE_SHUTDOWN
} State_TypeDef;

typedef struct{
	MotorName_TypeDef		motor_name;
	MotorStatus_TypeDef		status;
} StateMotor_TypeDef;

typedef struct{
	State_TypeDef 			state;

	StateMotor_TypeDef		motor;
	SeatAxis_TypeDef		seat_current_axis;
	SeatAxis_TypeDef		seat_memory_axis;

	ButtonState_TypeDef		button;

	uint8_t  		  		faultCounter;
	uint8_t  		  		faultLimit;
} PTR_TypeDef;

void STATE_Init(PTR_TypeDef *pState);
void STATE_UpdateState(PTR_TypeDef *pState);

extern PTR_TypeDef thisPTR;
extern uint16_t index_memory;
#endif /* SOURCE__MIDDLELAYER_STATEMANAGEMENT_STATEMANAGEMENT_H_ */
