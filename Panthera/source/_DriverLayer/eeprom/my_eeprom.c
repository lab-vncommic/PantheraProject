/*
 * my_eeprom.c
 *
 *  Created on: Jul 28, 2025
 *      Author: Admin
 */

#include "my_eeprom.h"


  void EEPROM_Start(void){
	  //Start eeprom first time
	     eeprom_start(&EEPROMD, &eeprom_cfg);
  }

  void EEPROM_Restart(void){
	    // Reset data in eeprom
	     eeprom_remove(&EEPROMD);

	    // Restart eeprom
	     eeprom_start(&EEPROMD, &eeprom_cfg);
  }

  void EEPROM_Write(uint32_t adc_value, uint16_t ID_EEPROM){
	   eeprom_write(&EEPROMD, ID_EEPROM, sizeof(adc_value), (uint32_t)&adc_value);
  }

  uint32_t EEPROM_Read(uint16_t id){
      uint32_t val = 0;
      uint32_t size;
      eeprom_read(&EEPROMD, id, &size, (uint32_t)&val) ;

      return val;
  }

