/*
 * communication.c
 *
 *  Created on: Aug 1, 2025
 *      Author: Admin
 */

#include "Communication.h"
CommMessage_TypeDef 	thisMessage;
Communication_TypeDef	thisComm;
uint16_t counter;

CAN_RxData_t CAN_Receive_M;
_Bool flag_can_tx;
_Bool flag_update_ic = false;

void UART_DecodeMessage(void);
uint8_t TwoBytetoUint8(const char *buffer, uint16_t offset);
uint8_t ByteToUint8(const char *buffer, uint16_t offset);
void CAN_GetRequest(void);
void CAN_DecodeMessage(void);
void COMM_HandleRead(void);
void UART_Handle(void);
void UART_HandleRead(void);
void COMM_HandleWrite(void);
void CAN_Handle(void);
void CAN_HandleRead(void);
void CAN_HandleWrite(void);
void COMM_UpdateStatus(void);

void COMM_Init(CommMode_Enum Comm)
{
	thisComm.mode = Comm;
	if (thisComm.mode == COMM_CAN) {
		CAN_Init();
	} else UART_Init();
}

void COMM_DecodeMessage(void)
{
	if (thisComm.mode == COMM_CAN) {
		CAN_DecodeMessage();
	} else UART_DecodeMessage();
}

void UART_DecodeMessage(void)
{
	if (SD2.rx_busy == 0) {
		thisMessage.id	  	= TwoBytetoUint8(BufRX, OFFSET_ID);
		thisMessage.request = ByteToUint8(BufRX, OFFSET_REQUEST);
		thisMessage.value	= ByteToUint8(BufRX, OFFSET_DATA);
	}
}

uint8_t TwoBytetoUint8(const char *buffer, uint16_t offset)
{
    uint8_t tens  = buffer[offset] - '0';
    uint8_t units = buffer[offset + 1] - '0';
    return (tens * 10) + units;
}

uint8_t ByteToUint8(const char *buffer, uint16_t offset)
{
	return (uint8_t)(buffer[offset] - '0');
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

void CAN_DecodeMessage(void)
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

void COMM_HandleRequest(void)
{
	if (thisComm.mode == COMM_CAN) {
		CAN_Handle();
	} else UART_Handle();
}

void UART_Handle(void)
{
	if (SD2.rx_busy == 0) {
		if (thisMessage.request == 0) {
			COMM_UpdateStatus();
			UART_HandleRead();
		} else if (thisMessage.request == 1) {
			COMM_HandleWrite();
			flag_update_ic = true;
			flag_rx_success = true;
		}
		counter = 0;
		flag_enable_rx = 1;
	}
	UART_HandleRequest();
}

void UART_HandleRead(void)
{
	COMM_HandleRead();
	if (thisMessage.data_tx == 1) {
		BufTX[0] = '1';
	} else BufTX[0] = '0';
	flag_enable_tx = 1;
}

void COMM_HandleRead(void)
{
	// Update status from System to Communication
	COMM_UpdateStatus();

	switch (thisMessage.id) {
		case 0: // Power ON
			thisMessage.data_tx = 1;
			break;
			
		case 1: // Positioning KEY: Seat Forward
			if (thisComm.button_comm.slider_1_value || thisComm.button.slider_1_value) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;

		case 2: // Positioning KEY: Seat Backward
			if (thisComm.button_comm.slider_2_value || thisComm.button.slider_2_value) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;

		case 3: // Positioning KEY: Seat Upward
			if (thisComm.button_comm.height_1_value || thisComm.button.height_1_value) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;

		case 4: // Positioning KEY: Seat Downward
			if (thisComm.button_comm.height_2_value || thisComm.button.height_2_value) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;

		case 5: // Positioning KEY: Seat Recliner Forward
			if (thisComm.button_comm.recliner_1_value || thisComm.button.recliner_1_value) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;

		case 7: // Positioning KEY: Seat Recliner Backward
			if (thisComm.button_comm.recliner_2_value || thisComm.button.recliner_2_value) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;

		case 10: // Seat Forward: IN1_1
			if (thisComm.motor_comm.motor_slider.ina_value || thisComm.motor.motor_slider.ina_value) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;

		case 11: // Seat Backward: IN2_1
			if (thisComm.motor_comm.motor_slider.inb_value || thisComm.motor.motor_slider.inb_value) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;

		case 12: // Seat Upward: IN1_2
			if (thisComm.motor_comm.motor_height.ina_value || thisComm.motor.motor_height.ina_value) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;

		case 13: // Seat Downward: IN2_2
			if (thisComm.motor_comm.motor_height.inb_value || thisComm.motor.motor_height.inb_value) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;

		case 14: // Seat Recliner Forward: IN1_3
			if (thisComm.motor_comm.motor_recliner.ina_value || thisComm.motor.motor_recliner.ina_value) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;

		case 15: // Seat Recliner Backward: IN2_3
			if (thisComm.motor_comm.motor_recliner.inb_value || thisComm.motor.motor_recliner.inb_value) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;

		case 16: // Memory
			if (thisComm.button_comm.save_value || thisComm.button.save_value) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;

		case 17: // Key A
			if (thisComm.button_comm.recall_1_value || thisComm.button.recall_1_value) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;

		case 20: // Key B
			if (thisComm.button_comm.recall_2_value || thisComm.button.recall_2_value) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;

		case 21: // Key C
			if (thisComm.button_comm.recall_3_value || thisComm.button.recall_3_value) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;

		case 22: // Motor HALL_1
			if (thisComm.motor_comm.motor_slider.topology == SENSOR) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;

		case 23: // Motor HALL_2
			if (thisComm.motor_comm.motor_height.topology == SENSOR) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;

		case 24: // Motor HALL_3
			if (thisComm.motor_comm.motor_recliner.topology == SENSOR) {
				thisMessage.data_tx = 1;
			} else thisMessage.data_tx = 0;
			break;
		default:
			break;
	}
}

void COMM_HandleWrite(void)
{
	switch (thisMessage.id) {
		case 1: // Positioning KEY: Seat Forward
			if (thisMessage.value == 1) {
				thisComm.button_comm.slider_1_value = 1;
			} else thisComm.button_comm.slider_1_value = 0;
			break;

		case 2: // Positioning KEY: Seat Backward
			if (thisMessage.value == 1) {
				thisComm.button_comm.slider_2_value = 1;
			} else thisComm.button_comm.slider_2_value = 0;
			break;

		case 3: // Positioning KEY: Seat Upward
			if (thisMessage.value == 1) {
				thisComm.button_comm.height_1_value = 1;
			} else thisComm.button_comm.height_1_value = 0;
			break;

		case 4: // Positioning KEY: Seat Downward
			if (thisMessage.value == 1) {
				thisComm.button_comm.height_2_value = 1;
			} else thisComm.button_comm.height_2_value = 0;
			break;

		case 5: // Positioning KEY: Seat Recliner Forward
			if (thisMessage.value == 1) {
				thisComm.button_comm.recliner_1_value = 1;
			} else thisComm.button_comm.recliner_1_value = 0;
			break;

		case 7: // Positioning KEY: Seat Recliner Backward
			if (thisMessage.value == 1) {
				thisComm.button_comm.recliner_2_value = 1;
			} else thisComm.button_comm.recliner_2_value = 0;
			break;

		case 16: // Memory
			if (thisMessage.value == 1) {
				thisComm.button_comm.save_value = 1;
			} else thisComm.button_comm.save_value = 0;
			break;

		case 17: // Key A
			if (thisMessage.value == 1) {
				thisComm.button_comm.recall_1_value = 1;
			} else thisComm.button_comm.recall_1_value = 0;
			break;

		case 20: // Key B
			if (thisMessage.value == 1) {
				thisComm.button_comm.recall_2_value = 1;
			} else thisComm.button_comm.recall_2_value = 0;
			break;

		case 21: // Key C
			if (thisMessage.value == 1) {
				thisComm.button_comm.recall_3_value = 1;
			} else thisComm.button_comm.recall_3_value = 0;
			break;
		default:
			break;
	}
}

void CAN_Handle(void)
{
	if (flag_can_tx == true) {
		if (thisMessage.request == 0) {
			COMM_UpdateStatus();
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
	COMM_HandleRead();
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
	COMM_HandleWrite();
	CAN_TX_Set_ID(thisMessage.id);
	CAN_TX_Set_byte_DATA(0xFF);
	CAN_Transmit_Data();
}

void COMM_UpdateStatus(void)
{
	BTN_UpdateToComm(&thisComm.button);
	MD_UpdateToComm(&thisComm.motor);
}

void COMM_CommToState(comm_system_t *pComm)
{
	pComm->button_comm = thisComm.button_comm;
	pComm->motor_comm  = thisComm.motor_comm;
}











