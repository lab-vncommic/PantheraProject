/*
 * MotorDriver.h
 *
 *  Created on: Jul 22, 2025
 *      Author: ADMIN
 */

#ifndef MIDDLELAYER_MOTORDRIVER_MOTORDRIVER_H_
#define MIDDLELAYER_MOTORDRIVER_MOTORDRIVER_H_
#include "Button.h"
#include "GlobalDefine.h"
#include "Measurement.h"

#define RESOLUTION_DISTANCE			  0.25f							 		// Unit: round, 1 axis = 0.25 round

// Sensor parameteres
#define TIME_DEBOUNCE_RISING_EDGE     100									// T = 100/10000 = 10ms
#define NO_PULSE_PER_ROUND			  336U
#define NO_PULSE_PER_UNIT_AXIS		  RESOLUTION_DISTANCE * NO_PULSE_PER_ROUND

// Sensorless parameters
#define WINDOW_SIZE 				  10  									// Size of the sliding window buffer
#define NUM_DEBOUNCE_SSLESS			  2
#define EPSILON						  29									// Equivalent to 2 uint ADC in 12 bit
#define NO_PEAK_PER_ROUND			  6600									// 6600 peak current /round
#define NO_PEAK_PER_UNIT_AXIS	      RESOLUTION_DISTANCE * NO_PEAK_PER_ROUND

#define CURRENT_LIMIT_SOFTSTART_MT_SLIDER		10.0f
#define CURRENT_LIMIT_SOFTSTART_MT_HEIGHT		10.0f
#define CURRENT_LIMIT_SOFTSTART_MT_RECLINER		10.0f

#define CURRENT_LIMIT_LOCKED_MT_SLIDER			5.0f
#define CURRENT_LIMIT_LOCKED_MT_HEIGHT			5.0f
#define CURRENT_LIMIT_LOCKED_MT_RECLINER		5.0f

#define CURRENT_LIMIT_SOFTSTART_MT_SLIDER_Q15		_q15(CURRENT_LIMIT_SOFTSTART_MT_SLIDER / CURRENT_MT1_HIGH_LIMIT_SOFT)
#define CURRENT_LIMIT_SOFTSTART_MT_HEIGHT_Q15		_q15(CURRENT_LIMIT_SOFTSTART_MT_HEIGHT / CURRENT_MT2_HIGH_LIMIT_SOFT)
#define CURRENT_LIMIT_SOFTSTART_MT_RECLINER_Q15		_q15(CURRENT_LIMIT_SOFTSTART_MT_RECLINER / CURRENT_MT2_HIGH_LIMIT_SOFT)

#define CURRENT_LIMIT_LOCKED_MT_SLIDER_Q15			_q15(CURRENT_LIMIT_LOCKED_MT_SLIDER / CURRENT_MT1_HIGH_LIMIT_SOFT)
#define CURRENT_LIMIT_LOCKED_MT_HEIGHT_Q15			_q15(CURRENT_LIMIT_LOCKED_MT_HEIGHT / CURRENT_MT2_HIGH_LIMIT_SOFT)
#define CURRENT_LIMIT_LOCKED_MT_RECLINER_Q15		_q15(CURRENT_LIMIT_LOCKED_MT_RECLINER / CURRENT_MT2_HIGH_LIMIT_SOFT)

typedef enum {
	SENSOR 		= 0,
	SENSORLESS	= 1
} MotorTopology_TypeDef;

typedef enum{
	STOP	 = 0,
	BACKWARD = 1,
	FORWARD  = 2
} MotorStatus_TypeDef;

typedef enum{
	SOFTSTART,
	STEADY
} MotorState_TypeDef;

typedef struct{
	GPIO_TypeDef ina;
	GPIO_TypeDef inb;
	GPIO_TypeDef sel0;
	GPIO_TypeDef sel1;
	GPIO_TypeDef mul_en;
	GPIO_TypeDef pwm;
	GPIO_TypeDef hall;
} MotorDriver_TypeDef;

typedef struct{
	_Bool 	 flag_checking_rising_edge;
	uint8_t  cnt_debounce;
	uint8_t  cnt_pulse;
} MotorSensor_TypeDef;

typedef struct {
	uint16_t values[WINDOW_SIZE];  // Buffer in Q15 format

    uint16_t current_value;
    uint16_t previous_value;

    uint16_t cnt_rising;
    uint16_t cnt_falling;
    uint16_t flag_rising;
    uint16_t flag_falling;

    uint16_t cnt_peak;
} MotorSensorless_TypeDef;

typedef struct{
	MotorName_TypeDef			name;
	MotorDriver_TypeDef			driver;
	MotorTopology_TypeDef		topology;

	MotorState_TypeDef			state;
	uint16_t					cnt_soft_start;
	uint16_t					cnt_available;
	_Bool						flag_available;
	MotorStatus_TypeDef 		status;
	MeasType_TypeDef			meas_type;
	uint16_t					axis;

	MotorSensor_TypeDef			sensor;
	MotorSensorless_TypeDef	 	sensorless;
} Motor_TypeDef;

typedef struct{
	MotorName_TypeDef			name;
	MotorTopology_TypeDef	    topology;
	uint8_t						ina_value;
	uint8_t						inb_value;
} MotorObject_TypeDef;

typedef struct{
	MotorObject_TypeDef			motor_slider;
	MotorObject_TypeDef			motor_height;
	MotorObject_TypeDef			motor_recliner;
} MotorComm_TypeDef;

// Public API
void MD_Init(void);

void MD_StopAll(void);
void MD_Run(MotorName_TypeDef motor_name, MotorStatus_TypeDef status, uint16_t current_value);

void MD_UpdateMeas(MeasType_TypeDef meas_type);
void MD_GetSeatAxis(SeatAxis_TypeDef *pSeat);
void MD_UpdateToComm(MotorComm_TypeDef *pMotor);
void MD_ResetAxis(void);
void MD_UpdateAxis(Motor_TypeDef *pMotor, uq15_t current_value);

extern Motor_TypeDef motor_height, motor_recliner, motor_slider;
#endif /* MIDDLELAYER_MOTORDRIVER_MOTORDRIVER_H_ */
