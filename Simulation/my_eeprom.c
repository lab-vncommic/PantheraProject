/*
 * my_eeprom.c
 *
 *  Created on: Jul 28, 2025
 *      Author: Admin
 */

#include "my_eeprom.h"

/**
 * @brief  Initializes the EEPROM for the first time using the specified configuration.
 *         This function should be called before any read/write operation.
 *
 * @retval None
 */
  void EEPROM_Start(void){
	  //Start eeprom first time
	    //  eeprom_start(&EEPROMD, &eeprom_cfg);
  }

  /**
   * @brief  Erases all data stored in EEPROM and restarts the EEPROM driver.
   *         Use this function when you want to reset the EEPROM content.
   *
   * @retval None
   */
  void EEPROM_Restart(void){
	    // Reset data in eeprom
	    //  eeprom_remove(&EEPROMD);

	    // // Restart eeprom
	    //  eeprom_start(&EEPROMD, &eeprom_cfg);
  }

  /**
   * @brief  Writes a 32-bit value to the EEPROM at the specified ID.
   *
   * @param  adc_value: The 32-bit value to be written.
   * @param  ID_EEPROM: The identifier key where the value will be stored.
   *
   * @retval None
   */
  void EEPROM_Write(uint32_t adc_value, uint16_t ID_EEPROM){
	//    eeprom_write(&EEPROMD, ID_EEPROM, sizeof(adc_value), (uint32_t)&adc_value);
  }

  /**
   * @brief  Reads a 32-bit value from the EEPROM using the specified ID.
   *
   * @param  id: The identifier (address/key) to read the value from.
   * @retval uint32_t: The value read from EEPROM. Returns 0 if not found or invalid.
   */
  uint32_t EEPROM_Read(uint16_t id){
      uint32_t val = 0;
      uint32_t size;
    //   eeprom_read(&EEPROMD, id, &size, (uint32_t)&val) ;

      return val;
  }

