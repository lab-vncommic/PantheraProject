/*
 * StateManagement.h
 *
 *  Created on: Jul 25, 2025
 *      Author: ADMIN
 */

#ifndef SOURCE__MIDDLELAYER_STATEMANAGEMENT_STATEMANAGEMENT_H_
#define SOURCE__MIDDLELAYER_STATEMANAGEMENT_STATEMANAGEMENT_H_
#include "Communication.h"
#include "MotorDriver.h"
#include "Measurement.h"
#include "FaultManagement.h"
#include "Button.h"
#include "GlobalDefine.h"
#include "memory.h"

#define MANUAL_ENABLE_MT			1000
#define MANUAL_ENABLE_MT1			1
#define MANUAL_ENABLE_MT2			MANUAL_ENABLE_MT / 3
#define MANUAL_ENABLE_MT3			MANUAL_ENABLE_MT * 2 / 3

#define AUTO_ENABLE_MT				1000
#define AUTO_ENABLE_MT1				1
#define AUTO_ENABLE_MT2				AUTO_ENABLE_MT / 3
#define AUTO_ENABLE_MT3				AUTO_ENABLE_MT * 2 / 3

#define CURRENT_LIMIT_MT_SLIDER		12.7f
#define CURRENT_LIMIT_MT_HEIGHT		12.7f
#define CURRENT_LIMIT_MT_RECLINER	12.7f

#define CURRENT_LIMIT_MT_SLIDER_Q15		_q15(CURRENT_LIMIT_MT_SLIDER / CURRENT_MT1_HIGH_LIMIT_SOFT)
#define CURRENT_LIMIT_MT_HEIGHT_Q15		_q15(CURRENT_LIMIT_MT_HEIGHT / CURRENT_MT2_HIGH_LIMIT_SOFT)
#define CURRENT_LIMIT_MT_RECLINER_Q15	_q15(CURRENT_LIMIT_MT_RECLINER / CURRENT_MT2_HIGH_LIMIT_SOFT)

typedef enum{
	STATE_IDLE		   = 0,
	STATE_SAVE		   = 1,
	STATE_MANUAL	   = 2,
	STATE_AUTO		   = 3,
	STATE_FAULT		   = 4,
	STATE_AUTORECOVERY = 5,
	STATE_SHUTDOWN	   = 6
} State_TypeDef;

typedef struct{
	State_TypeDef 			state;
	SeatAxis_TypeDef		seat_current_axis;
	SeatAxis_TypeDef		seat_memory_axis;

	CommSystem_TypeDef	    communication;
	MeasSystem_TypeDef		measurement;
	FaultSystem_TypeDef		fault;
	ButtonSystem_TypeDef 	button;
} PTR_TypeDef;

/*===========================================================================
 * Public API
 *===========================================================================*/
void STATE_Init(PTR_TypeDef *pState);
void STATE_UpdateState(PTR_TypeDef *pState);

void STATE_UpdateMeas(PTR_TypeDef *pState);
void STATE_UpdateFault(PTR_TypeDef *pState);
void STATE_UpdateButton(PTR_TypeDef *pState);

extern PTR_TypeDef thisPTR;
#endif /* SOURCE__MIDDLELAYER_STATEMANAGEMENT_STATEMANAGEMENT_H_ */
