/*
 * can_communication.c
 *
 *  Created on: Sep 9, 2025
 *      Author: ADMIN
 */

#include "can_communication.h"
// how to using this library
// first call funcation CAN_COMM_INIT
// next call function CAN_Handle_mess in loop
CAN_RxData_t CAN_Receive_M;
_Bool flag_can_tx;
_Bool flag_update_ic = false;

void CAN_GetRequest(void);
void CAN_HandleRead(void);
void CAN_HandleWrite(void);

void CAN_COMM_Init(void)
{
	CAN_Init();
}
void CAN_GetRequest(void)
{
	if (CAN_Receive_M.frame.data8[0] == 0) {
		thisMessage.request = 0;
	} else if (CAN_Receive_M.frame.data8[0] == 1) {
		thisMessage.request = 1;
	}

	if (CAN_Receive_M.frame.data8[1] == 0) {
		thisMessage.value = 0;
	} else if (CAN_Receive_M.frame.data8[1] == 1) {
		thisMessage.value = 1;
	}
}

void CAN_Decode_Mess(void)
{
	if(CAN_Receive_Data(&CAN_Receive_M.frame) == true){
    // data_byte[0] = 0 ===> read write
    // data_byte[1] = 0 ===> data
        if(CAN_Receive_M.frame.DLC == 2){
        	flag_can_tx = true;
			CAN_GetRequest();
			switch (CAN_Receive_M.frame.ID) {
				case 0x00: // Power ON
					thisMessage.id = 0;
					break;

				case 0x01: // Positioning KEY: Seat Backward
					thisMessage.id = 1;
					break;

				case 0x02: // Positioning KEY: Seat Backward
					thisMessage.id = 2;
					break;

				case 0x03: // Positioning KEY: Seat Upward
					thisMessage.id = 3;
					break;

				case 0x04: // Positioning KEY: Seat Downward
					thisMessage.id = 4;
					break;

				case 0x05: // Positioning KEY: Seat Recliner Forward
					thisMessage.id = 5;
					break;

				case 0x07: // Positioning KEY: Seat Recliner Backward
					thisMessage.id = 7;
					break;

				case 0x0A: // Seat Forward: IN1_1
					thisMessage.id = 10;
					break;

				case 0x0B: // Seat Backward: IN2_1
					thisMessage.id = 11;
					break;

				case 0x0C: // Seat Upward: IN1_2
					thisMessage.id = 12;
					break;

				case 0x0D: // Seat Downward: IN2_2
					thisMessage.id = 13;
					break;

				case 0x0E: // Seat Recliner Forward: IN1_3
					thisMessage.id = 14;
					break;

				case 0x0F: // Seat Recliner Backward: IN2_3
					thisMessage.id = 15;
					break;

				case 0x10: // Memory
					thisMessage.id = 16;
					break;

				case 0x11: // Key A
					thisMessage.id = 17;
					break;

				case 0x14: // Key B
					thisMessage.id = 20;
					break;

				case 0x15: // Key C
					thisMessage.id = 21;
					break;

				case 0x16: // Motor HALL_1
					thisMessage.id = 22;
					break;

				case 0x17: // Motor HALL_2
					thisMessage.id = 23;
					break;

				case 0x18: // Motor HALL_3
					thisMessage.id = 24;
					break;

				default:
					break;
			}
        }
	}
}

void CAN_Handle(void)
{
	COMM_UpdateStatus();
	if (flag_can_tx == true) {
		if (thisMessage.request == 0) {
			CAN_HandleRead();
			flag_can_tx = false;
		} else if (thisMessage.request == 1) {
			CAN_HandleWrite();
			flag_update_ic = true;
			flag_can_tx = false;
		}
	}
}

void CAN_HandleRead(void)
{
	COMM_Respone();
	CAN_TX_Set_ID(thisMessage.id);
	if (thisMessage.data_tx == 0) {
		CAN_TX_Set_byte_DATA(0x00);
	} else if (thisMessage.data_tx == 1) {
		CAN_TX_Set_byte_DATA(0x01);
	}
	CAN_Transmit_Data();
}

void CAN_HandleWrite(void)
{
	COMM_HandleWriteRequest();
	CAN_TX_Set_ID(thisMessage.id);
	CAN_TX_Set_byte_DATA(0xFF);
	CAN_Transmit_Data();
}

