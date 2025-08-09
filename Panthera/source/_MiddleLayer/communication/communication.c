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

uint8_t TwoBytetoUint8(const char *buffer, uint16_t offset);
uint8_t ByteToUint8(const char *buffer, uint16_t offset);
void COMM_Handle(void);
void COMM_DecodeMessage(void);
void COMM_UpdateStatus(void);
void COMM_Respone(void);
void COMM_HandleWriteRequest(void);

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

void COMM_Init(void)
{
	UART_Init();
	thisComm.mode = COMM_UART;
}

void COMM_Handle(void)
{
	if (SD2.rx_busy == 0) {
		COMM_DecodeMessage();
		if (thisMessage.request == 0) {
			COMM_Respone();
		} else if (thisMessage.request == 1) {
			COMM_HandleWriteRequest();
		}
		counter = 0;
		flag_enable_rx = 1;
	} else if (SD2.rx_len == 5) {
		if (counter++ >= 10) {
			flag_enable_rx = 1;
			flag_rx_fail = true;
			counter = 0;
		}
	}
}

void COMM_DecodeMessage(void)
{
	thisMessage.id	  	= TwoBytetoUint8(BufRX, OFFSET_ID);
	thisMessage.request = ByteToUint8(BufRX, OFFSET_REQUEST);
	thisMessage.value	= ByteToUint8(BufRX, OFFSET_DATA);
}

void COMM_UpdateStatus(void)
{
	thisComm.button = BTN_UpdateToComm();
	thisComm.motor  = MD_UpdateToComm();
}

void COMM_Respone(void)
{
	// Update status from System to Communication
	COMM_UpdateStatus();

	switch (thisMessage.id) {
		case 0: // Power ON

			break;
			
		case 1: // Positioning KEY: Seat Forward
			if (thisComm.button_comm.slider_1_value || thisComm.button.slider_1_value) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = true;
			break;
			
		case 2: // Positioning KEY: Seat Backward
			if (thisComm.button_comm.slider_2_value) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = 1;
			break;
			
		case 3: // Positioning KEY: Seat Upward
			if (thisComm.button_comm.height_1_value || thisComm.button.height_1_value) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = 1;
			break;
			
		case 4: // Positioning KEY: Seat Downward
			if (thisComm.button_comm.height_2_value) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = 1;
			break;
			
		case 5: // Positioning KEY: Seat Recliner Forward
			if (thisComm.button_comm.recliner_1_value) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = 1;
			break;
			
		case 7: // Positioning KEY: Seat Recliner Backward
			if (thisComm.button_comm.recliner_2_value) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = 1;
			break;
			
		case 10: // Seat Forward: IN1_1
			if (thisComm.motor_comm.motor_slider.ina_value) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = 1;
			break;
			
		case 11: // Seat Backward: IN2_1
			if (thisComm.motor_comm.motor_slider.inb_value) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = 1;
			break;
			
		case 12: // Seat Upward: IN1_2
			if (thisComm.motor_comm.motor_height.ina_value) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = 1;
			break;
			
		case 13: // Seat Downward: IN2_2
			if (thisComm.motor_comm.motor_height.inb_value) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = 1;
			break;
			
		case 14: // Seat Recliner Forward: IN1_3
			if (thisComm.motor_comm.motor_recliner.ina_value) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = 1;
			break;
			
		case 15: // Seat Recliner Backward: IN2_3
			if (thisComm.motor_comm.motor_recliner.inb_value) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = 1;
			break;
			
		case 16: // Memory
			if (thisComm.button_comm.save_value) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = 1;
			break;
			
		case 17: // Key A
			if (thisComm.button_comm.recall_1_value) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = 1;
			break;
			
		case 20: // Key B
			if (thisComm.button_comm.recall_2_value) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = 1;
			break;
			
		case 21: // Key C
			if (thisComm.button_comm.recall_3_value) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = 1;
			break;
			
		case 22: // Motor HALL_1
			if (thisComm.motor_comm.motor_slider.topology == SENSOR) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = 1;
			break;
			
		case 23: // Motor HALL_2
			if (thisComm.motor_comm.motor_height.topology == SENSOR) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = 1;
			break;
			
		case 24: // Motor HALL_3
			if (thisComm.motor_comm.motor_recliner.topology == SENSOR) {
				BufTX[0] = '1';
			} else BufTX[0] = '0';
			flag_enable_tx = 1;
			break;
			
		case 25: // Motor HALL_3

			break;
		default:
			break;
	}
}

void COMM_HandleWriteRequest(void)
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

		case 10: // Seat Forward: IN1_1
			if (thisMessage.value == 1) {
				thisComm.motor_comm.motor_slider.ina_value = 1;
			} else thisComm.motor_comm.motor_slider.ina_value = 0;
			break;

		case 11: // Seat Backward: IN2_1
			if (thisMessage.value == 1) {
				thisComm.motor_comm.motor_slider.inb_value = 1;
			} else thisComm.motor_comm.motor_slider.inb_value = 0;
			break;

		case 12: // Seat Upward: IN1_2
			if (thisMessage.value == 1) {
				thisComm.motor_comm.motor_height.ina_value = 1;
			} else thisComm.motor_comm.motor_height.ina_value = 0;
			break;

		case 13: // Seat Downward: IN2_2
			if (thisMessage.value == 1) {
				thisComm.motor_comm.motor_height.inb_value = 1;
			} else thisComm.motor_comm.motor_height.inb_value = 0;
			break;

		case 14: // Seat Recliner Forward: IN1_3
			if (thisMessage.value == 1) {
				thisComm.motor_comm.motor_recliner.ina_value = 1;
			} else thisComm.motor_comm.motor_recliner.ina_value = 0;
			break;

		case 15: // Seat Recliner Backward: IN2_3
			if (thisMessage.value == 1) {
				thisComm.motor_comm.motor_recliner.inb_value = 1;
			} else thisComm.motor_comm.motor_recliner.inb_value = 0;
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

CommSystem_TypeDef COMM_CommToState(void)
{
	COMM_Handle();
	CommSystem_TypeDef temp_comm;
	temp_comm.button_comm = thisComm.button_comm;
	temp_comm.motor_comm  = thisComm.motor_comm;
	return temp_comm;
}

void COMM_TransmitReceiveHandle(void)
{
	switch (thisComm.mode) {
		case COMM_DEBUG:
			UART_Monitor();
			break;

		case COMM_UART:
			UART_HandleRequest();
			break;

		case COMM_CAN:

			break;
		default:
			break;
	}
}







