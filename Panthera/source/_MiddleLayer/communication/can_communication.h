/*
 * can_communication.h
 *
 *  Created on: Sep 9, 2025
 *      Author: ADMIN
 */

#ifndef SOURCE__MIDDLELAYER_COMMUNICATION_CAN_COMMUNICATION_H_
#define SOURCE__MIDDLELAYER_COMMUNICATION_CAN_COMMUNICATION_H_
#include "can.h"
#include "communication.h"

void CAN_Decode_Mess(void);
void CAN_Handle(void);

extern _Bool flag_update_ic;
#endif /* SOURCE__MIDDLELAYER_COMMUNICATION_CAN_COMMUNICATION_H_ */
