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

typedef enum{
	STATE_IDLE			= 0x00,
	STATE_MANUAL	 	= 0x01,
	STATE_AUTO		 	= 0x02,
	STATE_FAULT     	= 0x03,
	STATE_AUTORECOVERY 	= 0x04,
	STATE_SHUTDOWN
} PTR_State_TypeDef;

typedef struct{
	uint16_t height;
	uint16_t recliner;
	uint16_t slider;
} Axis_TypeDef;

typedef struct{
	PTR_State_TypeDef 		State;
	Axis_TypeDef			Seat;
	Motor_TypeDef			Motor_Active;
	uint8_t  		  		faultCounter;
	uint8_t  		  		faultLimit;
} PTR_TypeDef;

void STATE_Init(PTR_TypeDef *pState);
void STATE_UpdateState(PTR_TypeDef *pState);

void STATE_idleHandle(void);
void STATE_manualHandle(void);
void STATE_autoHandle(void);
void STATE_faultHandle(void);
void STATE_autorecoveryHandle(void);
void STATE_shutdownHandle(void);

extern PTR_TypeDef thisPTR;
#endif /* SOURCE__MIDDLELAYER_STATEMANAGEMENT_STATEMANAGEMENT_H_ */
