/*
 * memory.h
 *
 *  Created on: Jul 29, 2025
 *      Author: Admin
 */

#ifndef MEMORY_MEMORY_H_
#define MEMORY_MEMORY_H_
#include <string.h>
#include <stdint.h>
#include "my_eeprom.h"
#include "GlobalDefine.h"

typedef struct{
	uint16_t x;
	uint16_t y;
	uint16_t z;
} Memory_TypeDef;

const char* convert_Dec_to_Hex(uint32_t a);
void merge_char(const char* a, const char* b, const char* c, char* output, size_t output_size);
uint32_t convert_charHex_to_uint32_t(const char* hex_str);
uint32_t convert_coordinates_into_single_value(uint32_t x,uint32_t y, uint32_t z);
void seperate_char( char* a,  char* b,  char* c, char* input);

void MEM_Read(uint16_t id_recall);
void MEM_Write(uint32_t x,uint32_t y, uint32_t z, uint16_t id_recall);

SeatAxis_TypeDef MEM_GetSeatAxis(uint16_t id_recall);

//extern Memory_TypeDef thisMemory;
#endif /* MEMORY_MEMORY_H_ */
