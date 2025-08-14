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
#include <stdio.h>

#include "../../_DriverLayer/eeprom/eeprom_user.h"
#include "GlobalDefine.h"

//Address EEPROM 0x00800000

typedef struct{
	uint16_t x;
	uint16_t y;
	uint16_t z;
} Memory_TypeDef;

void MEM_Read(uint16_t id_recall);
void MEM_Write(uint32_t x,uint32_t y, uint32_t z, uint16_t id_recall);

SeatAxis_TypeDef MEM_GetSeatAxis(uint16_t id_recall);

//extern Memory_TypeDef thisMemory;
#endif /* MEMORY_MEMORY_H_ */
