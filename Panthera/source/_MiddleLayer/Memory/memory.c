/*
 * memory.c
 *
 *  Created on: Jul 29, 2025
 *      Author: Admin
 */
#include "memory.h"
Memory_TypeDef thisMemory;

/**
 * @brief  Stores (x, y, z) coordinates into EEPROM as a single encoded value.
 *
 * @param  x, y, z: Coordinate values to be stored.
 * @param  id_recall: EEPROM storage index/ID.
 */
void MEM_Write(uint32_t x,uint32_t y, uint32_t z, uint16_t id_recall){
	switch (id_recall) {
		case 1:
			EEPROM_Write(x, 1);
			EEPROM_Write(y, 2);
			EEPROM_Write(z, 3);
			break;
		case 2:
			EEPROM_Write(x, 4);
			EEPROM_Write(y, 5);
			EEPROM_Write(z, 6);
			break;
		case 3:
			EEPROM_Write(x, 7);
			EEPROM_Write(y, 8);
			EEPROM_Write(z, 9);
			break;
		case 4:
			EEPROM_Write(x, 10);
			EEPROM_Write(y, 11);
			EEPROM_Write(z, 12);
			break;
		case 5:
			EEPROM_Write(x, 13);
			EEPROM_Write(y, 14);
			EEPROM_Write(z, 15);
			break;
		case 6:
			EEPROM_Write(x, 16);
			EEPROM_Write(y, 17);
			EEPROM_Write(z, 18);
			break;
		default:
			break;
	}
}

/**
 * @brief  Reads encoded coordinate data from EEPROM and decodes it to x, y, z values.
 *
 * @param  id_recall: EEPROM index/ID to read from.
 *
 * @note   Decoded values are stored in global variable `thisMemory`.
 */
void MEM_Read(uint16_t id_recall){
	switch (id_recall) {
		case 1:
		    thisMemory.x = EEPROM_Read(1);
			thisMemory.y = EEPROM_Read(2);
			thisMemory.z = EEPROM_Read(3);
			break;
		case 2:
		    thisMemory.x = EEPROM_Read(4);
			thisMemory.y = EEPROM_Read(5);
			thisMemory.z = EEPROM_Read(6);
			break;
		case 3:
		    thisMemory.x = EEPROM_Read(7);
			thisMemory.y = EEPROM_Read(8);
			thisMemory.z = EEPROM_Read(9);
			break;
		case 4:
		    thisMemory.x = EEPROM_Read(10);
			thisMemory.y = EEPROM_Read(11);
			thisMemory.z = EEPROM_Read(12);
			break;
		case 5:
		    thisMemory.x = EEPROM_Read(13);
			thisMemory.y = EEPROM_Read(14);
			thisMemory.z = EEPROM_Read(15);
			break;

		case 6:
		    thisMemory.x = EEPROM_Read(16);
			thisMemory.y = EEPROM_Read(17);
			thisMemory.z = EEPROM_Read(18);
			break;
		default:
			break;
	}
}

/**
 * @brief Retrieves seat axis data from memory for a specific recall position
 * @details Reads the stored coordinates from EEPROM at the specified recall ID
 *          and converts them to a SeatAxis_TypeDef structure
 * @param[in] id_recall The recall position ID (1-3) to retrieve
 * @return SeatAxis_TypeDef Structure containing the height, recliner, and slider positions
 */
void MEM_GetSeatAxis(SeatAxis_TypeDef *pSeat, uint16_t id_recall)
{
	MEM_Read(id_recall);
	pSeat->recliner = thisMemory.x;
	pSeat->slider   = thisMemory.y;
	pSeat->height   = thisMemory.z;
}

void mem_get_axis_max(motor_system_t *p_sm_motor)
{
	MEM_Read(ID_MEM_AXIS_MAX);
	p_sm_motor->recliner.axis_max = thisMemory.x;
	p_sm_motor->slider.axis_max   = thisMemory.y;
	p_sm_motor->height.axis_max   = thisMemory.z;
}

void mem_get_topology(motor_system_t *p_sm_motor)
{
	MEM_Read(ID_MEM_TOPOLOGY);
	p_sm_motor->recliner.topology = thisMemory.x;
	p_sm_motor->slider.topology   = thisMemory.y;
	p_sm_motor->height.topology   = thisMemory.z;
}














