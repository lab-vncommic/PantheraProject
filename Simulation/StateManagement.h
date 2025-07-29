/*
 * StateManagement.h
 *
 *  Created on: Jul 25, 2025
 *      Author: ADMIN
 */

#ifndef SOURCE__MIDDLELAYER_STATEMANAGEMENT_STATEMANAGEMENT_H_
#define SOURCE__MIDDLELAYER_STATEMANAGEMENT_STATEMANAGEMENT_H_
// #include "components.h"
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

typedef enum{
	MOTOR_ACTIVE_NONE		= 0x00,
	MOTOR_ACTIVE_HEIGHT		= 0x01,
	MOTOR_ACTIVE_RECLINER	= 0x02,
	MOTOR_ACTIVE_SLIDER		= 0x03
} Motor_Active_Name_TypeDef;

typedef struct{
	Motor_Active_Name_TypeDef		name;
	MotorStatus_TypeDef				status;
} Motor_Active_TypeDef;

typedef struct{
	uint16_t height;
	uint16_t recliner;
	uint16_t slider;
} SeatAxis_TypeDef;

typedef struct{
	PTR_State_TypeDef 			state;
	SeatAxis_TypeDef			seat_axis;
	Motor_Active_TypeDef		motor_active;
	uint8_t  		  			faultCounter;
	uint8_t  		  			faultLimit;
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
