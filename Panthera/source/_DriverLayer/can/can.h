#ifndef SOURCE__DRIVERLAYER_CAN_CAN_H_
#define SOURCE__DRIVERLAYER_CAN_CAN_H_

#include "components.h"
#include "can_lld_cfg.h"
#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "string.h"


void CAN_Init(void);
void CAN_HandleRequest(void);
void CAN_TX_Callback(CANDriver *canp);
void CAN_Transmit_Data(void);
_Bool CAN_Receive_Data(CANRxFrame *rxframe);
void CAN_TX_Set_ID(uint32_t ID);
//void CAN_TX_Set_byte_RW(uint8_t data);
void CAN_TX_Set_byte_DATA(uint8_t data);

#define CAN_DRIVER &CAND3

// CAN receive data structure
typedef struct {
    CANRxFrame frame;
    _Bool data_available;
} CAN_RxData_t;

extern CAN_RxData_t can_rx_data;


#endif /* SOURCE__DRIVERLAYER_CAN_CAN_H_ */
