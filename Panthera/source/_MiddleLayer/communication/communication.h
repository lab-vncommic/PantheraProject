/*
 * communication.h
 *
 *  Created on: Aug 1, 2025
 *      Author: Admin
 */

#ifndef SOURCE__MIDDLELAYER_COMMUNICATION_COMMUNICATION_H_
#define SOURCE__MIDDLELAYER_COMMUNICATION_COMMUNICATION_H_

#include "stdint.h"
#include "string.h"
#include "uart.h"
#include "can.h"
#include "Button.h"
#include "MotorDriver.h"

#define READ_MODE 	0
#define WRITE_MODE 	1

// UART parameters
#define OFFSET_ID		2
#define OFFSET_REQUEST	4
#define OFFSET_DATA		5

typedef struct{
	uint8_t id;
	uint8_t request;		// request value: 1 = write, 0 = read
	uint8_t value;
	uint8_t data_tx;
} CommMessage_TypeDef;

typedef enum{
	COMM_DEBUG  = 0,
	COMM_UART	= 1,
	COMM_CAN	= 2
} CommMode_Enum;

typedef struct{
	CommMode_Enum			mode;
	ButtonComm_TypeDef 		button;
	ButtonComm_TypeDef   	button_comm;
	MotorComm_TypeDef		motor;
	MotorComm_TypeDef		motor_comm;
} Communication_TypeDef;

typedef struct{
	ButtonComm_TypeDef   	button_comm;
	MotorComm_TypeDef		motor_comm;
} comm_system_t;

void COMM_Init(CommMode_Enum Comm);
void COMM_DecodeMessage(void);
void COMM_HandleRequest(void);
void COMM_CommToState(comm_system_t *pComm);

extern _Bool flag_update_ic;
extern CommMessage_TypeDef thisMessage;
#endif /* SOURCE__MIDDLELAYER_COMMUNICATION_COMMUNICATION_H_ */
