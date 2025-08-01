/*
 * MotorDriver.h
 *
 *  Created on: Jul 22, 2025
 *      Author: ADMIN
 */

#ifndef MIDDLELAYER_MOTORDRIVER_MOTORDRIVER_H_
#define MIDDLELAYER_MOTORDRIVER_MOTORDRIVER_H_
#include "components.h"
#include "Button.h"
#include "GlobalDefine.h"

#define TIME_DEBOUNCE_RISING_EDGE     100			// T = 100/10000 = 10ms

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

typedef enum{
	CURRENT = 0,
	THERMAL	= 1,
	VCC		= 2
} DriverMeasType_TypeDef;

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
	MotorDriver_TypeDef			driver;
	MotorTopology_TypeDef		topology;

	MotorStatus_TypeDef 		status;
	DriverMeasType_TypeDef		meas_type;
	uint16_t					axis;

	// Parameters when using hall sensor option

	// Parameters when using sensorless option
} Motor_TypeDef;

void MD_Init(void);

void MD_StopAll(void);
void MD_Run(MotorName_TypeDef motor_name, MotorStatus_TypeDef status);

SeatAxis_TypeDef MD_GetSeatAxis(void);

//extern Motor_TypeDef motor_height, motor_recliner, motor_slider;
#endif /* MIDDLELAYER_MOTORDRIVER_MOTORDRIVER_H_ */
