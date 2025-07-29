/*
 * MotorDriver.h
 *
 *  Created on: Jul 22, 2025
 *      Author: ADMIN
 */

#ifndef MIDDLELAYER_MOTORDRIVER_MOTORDRIVER_H_
#define MIDDLELAYER_MOTORDRIVER_MOTORDRIVER_H_
// #include "components.h"
#include "Button.h"
#include "gpio.h"

typedef enum {
	SENSOR = 0,
	SENSORLESS
} MotorTopoCtrl_TypeDef;

typedef enum{
	STOP = 0,
	BACKWARD,
	FORWARD
} MotorStatus_TypeDef;

typedef enum{
	CURRENT = 0,
	THERMAL,
	VCC
} MotorMeasType_TypeDef;

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
	MotorTopoCtrl_TypeDef		topo_ctrl;

	MotorStatus_TypeDef 		status;
	MotorStatus_TypeDef 		status_pre;
	MotorMeasType_TypeDef		meas_type;
	uint16_t					axis;

	// Parameters when using hall sensor option
	uint8_t      				hall_pre;
	uint16_t 					hall_counter;

	// Parameters when using sensorless option
} Motor_TypeDef;

/*===========================================================================
 * REUSABLE CODE SECTION
 *===========================================================================*/
void MD_ConfigDriver(Motor_TypeDef *pMotor, GPIO_TypeDef pin_ina, GPIO_TypeDef pin_inb, GPIO_TypeDef pin_sel0,\
					 GPIO_TypeDef pin_sel1, GPIO_TypeDef pin_mul_en, GPIO_TypeDef pin_pwm);
void MD_UpdateDriver(Motor_TypeDef *pMotor);
void MD_UpdateStatusMotor(Motor_TypeDef *pMotor, MotorStatus_TypeDef status);
void MD_UpdateStatusMulsen(Motor_TypeDef *pMotor, MotorMeasType_TypeDef meas_type);

/*===========================================================================
 * PTR CODE SECTION
 *===========================================================================*/
void MD_Init(void);
void MD_StopAll(void);

// API for detecting position with hall sensor
_Bool MD_CheckRisingEdge(uint8_t current_value);
void MD_UpdateAxis(Motor_TypeDef *pMotor);

// API for detecting position without sensor

extern Motor_TypeDef motor_height, motor_recliner, motor_slider;
extern uint16_t counter;
#endif /* MIDDLELAYER_MOTORDRIVER_MOTORDRIVER_H_ */
