/*
 * my_eeprom.h
 *
 *  Created on: Jul 28, 2025
 *      Author: Admin
 */

#ifndef EEPROM_MY_EEPROM_H_
#define EEPROM_MY_EEPROM_H_
#include <stdint.h>
#include "components.h"

#define ID_EEPROM_1	1 //ID of the record to write
#define ID_EEPROM_2	2
#define ID_EEPROM_3	3

void EEPROM_Start(void);
void EEPROM_Restart(void);
void EEPROM_Write(uint32_t adc_value, uint16_t ID_EEPROM);
uint32_t EEPROM_Read(uint16_t id);

#endif /* EEPROM_MY_EEPROM_H_ */
