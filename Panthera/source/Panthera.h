/*
 * Panthera.h
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */

#ifndef PANTHERA_H_
#define PANTHERA_H_
#include "Communication.h"
#include "GlobalDefine.h"
#include "Measurement.h"
#include "FaultManagement.h"
#include "StateManagement.h"

#include "uart.h"
#include "memory.h"
#include "panthera/driver_layer/inc/adc_drv_handle.h"

#include "panthera/midde_layer/inc/meas_mid_handle.h"

void ptr_system_initialization(void);
void ptr_main_interrupt_service(void);
void ptr_inf_loop_system(void);

#endif /* PANTHERA_H_ */
