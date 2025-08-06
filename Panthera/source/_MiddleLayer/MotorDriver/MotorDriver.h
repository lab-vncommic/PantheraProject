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

// Sensor parameteres
#define TIME_DEBOUNCE_RISING_EDGE     100									// T = 100/10000 = 10ms
#define GAIN_GEAR					  336U
#define RESOLUTION_DISTANCE			  0.25f							 		// Unit: round, 1 axis = 0.25 round
#define NO_PULSE_PER_AXIS			  GAIN_GEAR*RESOLUTION_DISTANCE

// Sensorless parameters
#define PULSE_PER_ROUND				  1000									// 1000 peak current /round
#define WINDOW_SIZE 				  10  									// Size of the sliding window buffer



typedef enum {
	SENSOR 		= 0,
	SENSORLESS	= 1
} MotorTopology_TypeDef;

typedef enum{
	MOTOR_NONE		= 0,
	MOTOR_HEIGHT	= 1,
	MOTOR_RECLINER	= 2,
	MOTOR_SLIDER	= 3
} MotorName_TypeDef;

typedef enum{
	STOP	 = 0,
	BACKWARD = 1,
	FORWARD  = 2
} MotorStatus_TypeDef;

typedef struct{
	GPIO_TypeDef ina;
	GPIO_TypeDef inb;
	GPIO_TypeDef sel0;
	GPIO_TypeDef sel1;
	GPIO_TypeDef mul_en;
	GPIO_TypeDef pwm;
	GPIO_TypeDef hall;
} MotorDriver_TypeDef;

typedef struct {
	uint16_t values[WINDOW_SIZE];  // Buffer in Q15 format

    uint16_t current_value;
    uint16_t previous_value;

    uint16_t cnt_rising;
    uint16_t cnt_falling;
    uint16_t flag_rising;
    uint16_t flag_falling;
} PeakDetector_TypeDef;

typedef struct{
	MotorDriver_TypeDef			driver;
	MotorTopology_TypeDef		topology;

	MotorStatus_TypeDef 		status;
	DriverMeasType_TypeDef		meas_type;
	uint16_t					axis;

	// Parameters when using sensorless option
	MeasMotor_TypeDef			meas_motor;
	uint16_t					current_value;
} Motor_TypeDef;

void MD_Init(void);

void MD_StopAll(void);
void MD_Run(MotorName_TypeDef motor_name, MotorStatus_TypeDef status);

void MD_UpdateMeas(DriverMeasType_TypeDef meas_type);
SeatAxis_TypeDef MD_GetSeatAxis(void);

//extern Motor_TypeDef motor_height, motor_recliner, motor_slider;
#endif /* MIDDLELAYER_MOTORDRIVER_MOTORDRIVER_H_ */
