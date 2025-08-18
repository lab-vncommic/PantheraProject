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

#define HOME_ENABLE_MT				1000
#define HOME_ENABLE_MT1				1
#define HOME_ENABLE_MT2				HOME_ENABLE_MT / 3
#define HOME_ENABLE_MT3				HOME_ENABLE_MT * 2 / 3

#define CURRENT_LIMIT_MT_SLIDER		12.7f
#define CURRENT_LIMIT_MT_HEIGHT		12.7f
#define CURRENT_LIMIT_MT_RECLINER	12.7f

#define CURRENT_LIMIT_MT_SLIDER_Q15		_q15(CURRENT_LIMIT_MT_SLIDER / CURRENT_MT1_HIGH_LIMIT_SOFT)
#define CURRENT_LIMIT_MT_HEIGHT_Q15		_q15(CURRENT_LIMIT_MT_HEIGHT / CURRENT_MT2_HIGH_LIMIT_SOFT)
#define CURRENT_LIMIT_MT_RECLINER_Q15	_q15(CURRENT_LIMIT_MT_RECLINER / CURRENT_MT2_HIGH_LIMIT_SOFT)

#define CURRENT_AUTO_RECOVERY_THRESHOLD			5.0f
#define THERMAL_AUTO_RECOVERY_THRESHOLD			60.0f

#define CURRENT_AUTO_RECOVERY_MT1_THRESHOLD_Q15		_q15(CURRENT_AUTO_RECOVERY_THRESHOLD / CURRENT_MT1_HIGH_LIMIT_SOFT)
#define THERMAL_AUTO_RECOVERY_MT1_THRESHOLD_Q15		(THERMAL_AUTO_RECOVERY_THRESHOLD - THERMAL_MT1_OFFSET_F32)/(2*THERMAL_MT1_GAIN_F32)
#define CURRENT_AUTO_RECOVERY_MT2_THRESHOLD_Q15		_q15(CURRENT_AUTO_RECOVERY_THRESHOLD / CURRENT_MT2_HIGH_LIMIT_SOFT)
#define THERMAL_AUTO_RECOVERY_MT2_THRESHOLD_Q15		(THERMAL_AUTO_RECOVERY_THRESHOLD - THERMAL_MT2_OFFSET_F32)/(2*THERMAL_MT2_GAIN_F32)
#define CURRENT_AUTO_RECOVERY_MT3_THRESHOLD_Q15		_q15(CURRENT_AUTO_RECOVERY_THRESHOLD / CURRENT_MT3_HIGH_LIMIT_SOFT)
#define THERMAL_AUTO_RECOVERY_MT3_THRESHOLD_Q15		(THERMAL_AUTO_RECOVERY_THRESHOLD - THERMAL_MT3_OFFSET_F32)/(2*THERMAL_MT3_GAIN_F32)

#define TIME_WAIT_FOR_AUTO_RECOVERY				5U			// Uint: s
#define TIME_WAIT_FOR_AUTO_RECOVERY_CYCLES		(uint16_t)TIME_WAIT_FOR_AUTO_RECOVERY * F_SAMPLE     // Uint: cycles

typedef enum{
	STATE_IDLE		   = 0,
	STATE_SAVE		   = 1,
	STATE_MANUAL	   = 2,
	STATE_AUTO		   = 3,
	STATE_HOMING	   = 4,
	STATE_FAULT		   = 5,
	STATE_AUTORECOVERY = 6,
	STATE_SHUTDOWN	   = 7
} State_TypeDef;

typedef struct{
	uint8_t		cnt_use_recall;
	_Bool		flag_slider_block;
	_Bool		flag_height_block;
	_Bool		flag_recliner_block;
} StateHomeObject_TypeDef;

typedef struct{
	State_TypeDef 			state;
	SeatAxis_TypeDef		seat_current_axis;
	SeatAxis_TypeDef		seat_previous_axis;
	SeatAxis_TypeDef		seat_memory_axis;
	SeatAxis_TypeDef		cnt_check_end_position;

	StateHomeObject_TypeDef state_home;

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
