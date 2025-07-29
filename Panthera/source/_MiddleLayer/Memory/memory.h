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

typedef struct{
	uint32_t x;
	uint32_t y;
	uint32_t z;
} Memory_TypeDef;






const char* convert_Dec_to_Hex(uint32_t a);
void merge_char(const char* a, const char* b, const char* c, char* output, size_t output_size);
uint32_t convert_charHex_to_uint32_t(const char* hex_str);
uint32_t input_EEPROM(uint32_t x,uint32_t y, uint32_t z);
void seperate_char( char* a,  char* b,  char* c, char* input);
void position_Memory(uint32_t output_EEPROM);

extern Memory_TypeDef thisMemory[3];
#endif /* MEMORY_MEMORY_H_ */
