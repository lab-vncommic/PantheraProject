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
	MotorName_TypeDef			name;
	MotorDriver_TypeDef			driver;
	MotorTopology_TypeDef		topology;

	MotorStatus_TypeDef 		status;
	MeasType_TypeDef			meas_type;
	uint16_t					axis;
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

extern Motor_TypeDef motor_height, motor_recliner, motor_slider;
#endif /* MIDDLELAYER_MOTORDRIVER_MOTORDRIVER_H_ */
