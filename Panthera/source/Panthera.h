/*
 * Panthera.h
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */

#ifndef PANTHERA_H_
#define PANTHERA_H_
#include "GlobalDefine.h"
#include "Measurement.h"
#include "FaultManagement.h"
#include "StateManagement.h"

#include "uart.h"
#include "memory.h"
#include "communication.h"

void PTR_system_initialization(void);
void PTR_main_interrupt_service(void);
void PTR_inf_loop_system(void);

#endif /* PANTHERA_H_ */
