/*
 * communication.h
 *
 *  Created on: Aug 1, 2025
 *      Author: Admin
 */

#include "stdint.h"
#include "string.h"
#include "uart.h"
#include "button.h"
#include "MotorDriver.h"

#ifndef SOURCE__MIDDLELAYER_COMMUNICATION_COMMUNICATION_H_
#define SOURCE__MIDDLELAYER_COMMUNICATION_COMMUNICATION_H_

#define READ_MODE 	0
#define WRITE_MODE 	1

typedef struct{
	uint16_t id;
	uint8_t  access_type;
	uint8_t  value;
}Communication_TypeDef;


void seperate_char_communication( char* a,  char* b,  char* c, char* input);
uint32_t convert_charHex_to_uint32_t_Communication(const char* hex_str);
void COMM_Receive(void);
void COMM_Transmit(void);

void COMM_DecodeMessage(void);

extern Communication_TypeDef thisCommunication;
#endif /* SOURCE__MIDDLELAYER_COMMUNICATION_COMMUNICATION_H_ */
