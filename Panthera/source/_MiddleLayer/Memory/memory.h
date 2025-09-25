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

#include "eeprom_user.h"
#include "GlobalDefine.h"

//Address EEPROM 0x00800000
#define ID_MEM_AXIS_MAX				5

typedef struct{
	uint32_t x;
	uint32_t y;
	uint32_t z;
} Memory_TypeDef;

void MEM_Read(uint16_t id_recall);
void MEM_Write(uint32_t x,uint32_t y, uint32_t z, uint16_t id_recall);
void MEM_GetSeatAxis(SeatAxis_TypeDef *pSeat, uint16_t id_recall);
void mem_get_axis_max(motor_system_t *p_sm_motor);

//extern Memory_TypeDef thisMemory;
#endif /* MEMORY_MEMORY_H_ */
