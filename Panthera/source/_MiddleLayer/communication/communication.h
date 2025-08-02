/*
 * communication.h
 *
 *  Created on: Aug 1, 2025
 *      Author: Admin
 */

#include "stdint.h"
#include "string.h"

#ifndef SOURCE__MIDDLELAYER_COMMUNICATION_COMMUNICATION_H_
#define SOURCE__MIDDLELAYER_COMMUNICATION_COMMUNICATION_H_

typedef struct{
	uint16_t ID_Message ;
	uint16_t Write_Read ;
	uint16_t value;
}Communication_TypeDef;


void seperate_char_communication( char* a,  char* b,  char* c, char* input);
uint32_t convert_charHex_to_uint32_t_Communication(const char* hex_str);
void update_Communication(char* string_input);

extern Communication_TypeDef thisCommunication;
#endif /* SOURCE__MIDDLELAYER_COMMUNICATION_COMMUNICATION_H_ */
