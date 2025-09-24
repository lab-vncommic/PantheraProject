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

#define HOME_HOLD_BTN_TIME			60000

#define MANUAL_ENABLE_MT			1000						// Delay cycle between 2 motor startup, approximately 100ms
#define MANUAL_ENABLE_MT1			1
#define MANUAL_ENABLE_MT2			MANUAL_ENABLE_MT
#define MANUAL_ENABLE_MT3			MANUAL_ENABLE_MT * 2

#define AUTO_ENABLE_MT				1000
#define AUTO_ENABLE_MT1				1
#define AUTO_ENABLE_MT2				AUTO_ENABLE_MT
#define AUTO_ENABLE_MT3				AUTO_ENABLE_MT * 2

#define HOME_ENABLE_MT				1000
#define HOME_ENABLE_MT1				1
#define HOME_ENABLE_MT2				HOME_ENABLE_MT
#define HOME_ENABLE_MT3				HOME_ENABLE_MT * 2

#define CURRENT_AUTO_RECOVERY_THRESHOLD			 2.0f
#define THERMAL_AUTO_RECOVERY_THRESHOLD			60.0f

#define CURRENT_AUTO_RECOVERY_MT1_THRESHOLD_Q15		_q15(CURRENT_AUTO_RECOVERY_THRESHOLD / CURRENT_MT1_HIGH_LIMIT_SOFT)
#define THERMAL_AUTO_RECOVERY_MT1_THRESHOLD_Q15		(uq31_t)(THERMAL_AUTO_RECOVERY_THRESHOLD - THERMAL_MT1_OFFSET_F32)/(2*THERMAL_MT1_GAIN_F32)
#define CURRENT_AUTO_RECOVERY_MT2_THRESHOLD_Q15		_q15(CURRENT_AUTO_RECOVERY_THRESHOLD / CURRENT_MT2_HIGH_LIMIT_SOFT)
#define THERMAL_AUTO_RECOVERY_MT2_THRESHOLD_Q15		(uq31_t)(THERMAL_AUTO_RECOVERY_THRESHOLD - THERMAL_MT2_OFFSET_F32)/(2*THERMAL_MT2_GAIN_F32)
#define CURRENT_AUTO_RECOVERY_MT3_THRESHOLD_Q15		_q15(CURRENT_AUTO_RECOVERY_THRESHOLD / CURRENT_MT3_HIGH_LIMIT_SOFT)
#define THERMAL_AUTO_RECOVERY_MT3_THRESHOLD_Q15		(uq31_t)(THERMAL_AUTO_RECOVERY_THRESHOLD - THERMAL_MT3_OFFSET_F32)/(2*THERMAL_MT3_GAIN_F32)

#define TIME_WAIT_FOR_AUTO_RECOVERY				10U			// Uint: s
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
	_Bool		flag_slider_block;
	_Bool		flag_height_block;
	_Bool		flag_recliner_block;
} StateHomeObject_TypeDef;

typedef struct{
	State_TypeDef 			state;
	State_TypeDef 			state_prev;
	SeatAxis_TypeDef		seat_current_axis;
	SeatAxis_TypeDef		seat_memory_axis;
	SeatAxis_TypeDef		cnt_check_end_position;

	StateHomeObject_TypeDef state_home;

	comm_system_t	    communication;
	meas_system_t		measurement;
	fault_system_t		fault;
	button_system_t 	button;
} PTR_TypeDef;

// FIXME: New implementation
#define TIME_ENABLE_MT1		1
#define TIME_ENABLE_MT2		1000
#define TIME_ENABLE_MT3		2000

typedef struct state_machine_s state_machine_t;

typedef struct{
	SeatAxis_TypeDef	seat_current_axis;
	SeatAxis_TypeDef	seat_memory_axis;
	State_TypeDef 		current;
	State_TypeDef 		previous;
} state_system_t;

typedef struct{
	state_system_t		state;
	comm_system_t	    communication;
	meas_system_t		measurement;
	fault_system_t		fault;
	button_system_t 	button;

	uint16_t			cnt_delay_startup_motor;

	_Bool				flag_power_on_mcu;
	_Bool				flag_save_current_position;
} state_data_t;

typedef struct{
	State_TypeDef	type;

	void (*entry)(state_machine_t *p_sm);
	void (*process)(state_machine_t *p_sm);
	void (*exit)(state_machine_t *p_sm);
} state_t;

struct state_machine_s {
    state_t      	*current;
    state_t      	*previous;
    state_t      	*next;
	uint32_t		cnt_time_on_state;
    _Bool		 	flag_change_state;
    state_data_t 	data;
};

/*===========================================================================
 * Public API
 *===========================================================================*/
void STATE_Init(PTR_TypeDef *pState);
void STATE_UpdateState(PTR_TypeDef *pState);

void STATE_UpdateMeas(PTR_TypeDef *pState);
void STATE_UpdateFault(PTR_TypeDef *pState);
void STATE_UpdateButton(PTR_TypeDef *pState);

void STATE_ProcessInputSignals(PTR_TypeDef *pState);

void state_int(state_machine_t *p_sm);
void state_update_comm(state_machine_t *p_sm);
void state_get_seat_axis(state_machine_t *p_sm);
void state_update(state_machine_t *p_state);

extern PTR_TypeDef thisPTR;
extern _Bool flag_enable_check_fault;

extern state_machine_t state_machine;
#endif /* SOURCE__MIDDLELAYER_STATEMANAGEMENT_STATEMANAGEMENT_H_ */
