/*
 * communication.c
 *
 *  Created on: Aug 1, 2025
 *      Author: Admin
 */

#include "communication.h"

Communication_TypeDef thisCommunication;

void convert_BufRX_to_HexString(char *buf, size_t len, char *out) {
    for (size_t i = 0; i < len; i++) {
        sprintf(&out[i * 2], "%02X", buf[i]);
    }
    out[len * 2] = '\0';
}

/**
 * @brief  Splits a 6-character hex string into three separate 2-character strings.
 *
 * @param  a, b, c: Output buffers for the 3 parts (each must be at least size 3).
 * @param  input:  Input hex string of at least 6 characters (e.g. "0A1B2C").
 * @retval None
 */
void seperate_char_communication( char* a,  char* b,  char* c, char* input) {
    a[0] = input[0];
    a[1] = input[1];
    a[2] = input[2];
    a[3] = input[3];
    a[4] = '\0';

    b[0] = input[4];
    b[1] = input[5];
    b[2] = '\0';

    c[0] = input[6];
    c[1] = input[7];
    c[2] = '\0';
}

uint32_t convert_charHex_to_uint32_t_Communication(const char* hex_str) {
    uint32_t result = 0;
    size_t len = strlen(hex_str);

    if (len == 0 || len > 8) {
        return 0;
    }

    for (size_t i = 0; i < len; i++) {
        char c = hex_str[i];
        uint32_t digit;
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'A' && c <= 'F') {
            digit = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'f') {
            digit = c - 'a' + 10;
        } else {
            return 0;
        }
        result = (result << 4) | digit;
    }

    return result;
}

void Communication_Receive(void){
	char string_input[9];
	convert_BufRX_to_HexString(BufRX, UART_RX_LENGTH, string_input);
    char a[5],b[3],c[3];
    seperate_char_communication(a,b,c,string_input);
    thisCommunication.ID_Message = (uint16_t) convert_charHex_to_uint32_t_Communication(a);
    thisCommunication.Write_Read = (uint16_t) convert_charHex_to_uint32_t_Communication(b);
    thisCommunication.value		 = (uint16_t) convert_charHex_to_uint32_t_Communication(c);
}

void Communication_Transmit(void){
	if(thisCommunication.Write_Read == READ_MODE){ // Only read mode
		switch (thisCommunication.ID_Message) {
			case 0:

				break;
			case 1:
				if(thisButtonAll.slider_1.value == 1){
					sprintf(BufTX,"Positioning KEY: Seat Forward: Pressed\r\n");
				}
				else{
					sprintf(BufTX,"Positioning KEY: Seat Forward: Not Pressed\r\n");
				}
				break;
			case 2:
				if(thisButtonAll.slider_2.value == 1){
					sprintf(BufTX,"Positioning KEY: Seat Backward: Pressed\r\n");
				}
				else{
					sprintf(BufTX,"Positioning KEY: Seat Backward: Not Pressed\r\n");
				}
				break;
			case 3:
				if(thisButtonAll.height_1.value == 1){
					sprintf(BufTX,"Positioning KEY: Seat Upward: Pressed\r\n");
				}
				else{
					sprintf(BufTX,"Positioning KEY: Seat Upward: Not Pressed\r\n");
				}
				break;
			case 4:
				if(thisButtonAll.height_2.value == 1){
					sprintf(BufTX,"Positioning KEY: Seat Downward: Pressed\r\n");
				}
				else{
					sprintf(BufTX,"Positioning KEY: Seat Downward: Not Pressed\r\n");
				}
				break;
			case 5:
				if(thisButtonAll.recliner_1.value == 1){
					sprintf(BufTX,"Positioning KEY: Seat Forward: Pressed\r\n");
				}
				else{
					sprintf(BufTX,"Positioning KEY: Seat Forward: Not Pressed\r\n");
				}
				break;
			case 7:
				if(thisButtonAll.recliner_2.value == 1){
					sprintf(BufTX,"Positioning KEY: Seat Backward: Pressed\r\n");
				}
				else{
					sprintf(BufTX,"Positioning KEY: Seat Backward: Not Pressed\r\n");
				}
				break;
			case 10:
				if(motor_slider.status == FORWARD){
					sprintf(BufTX,"Slider motor: Seat Forward\r\n");
				}
				else if(motor_slider.status == STOP){
					sprintf(BufTX,"Slider motor: Stop\r\n");
				}
				break;
			case 11:
				if(motor_slider.status == BACKWARD){
					sprintf(BufTX,"Slider motor: Seat Backward\r\n");
				}
				else if(motor_slider.status == STOP){
					sprintf(BufTX,"Slider motor: Stop\r\n");
				}
				break;
			case 12:
				if(motor_height.status == FORWARD){
					sprintf(BufTX,"Height motor: Seat Upward\r\n");
				}
				else if(motor_height.status == STOP){
					sprintf(BufTX,"Height motor: Stop\r\n");
				}
				break;
			case 13:
				if(motor_height.status == BACKWARD){
					sprintf(BufTX,"Height motor: Seat Downward\r\n");
				}
				else if(motor_height.status == STOP){
					sprintf(BufTX,"Height motor: Stop\r\n");
				}
				break;
			case 14:
				if(motor_recliner.status == FORWARD){
					sprintf(BufTX,"Recline motor: Seat Forward\r\n");
				}
				else if(motor_recliner.status == STOP){
					sprintf(BufTX,"Recline motor: Stop\r\n");
				}
				break;
			case 15:
				if(motor_recliner.status == BACKWARD){
					sprintf(BufTX,"Recline motor: Seat Backward\r\n");
				}
				else if(motor_recliner.status == STOP){
					sprintf(BufTX,"Recline motor: Stop\r\n");
				}
				break;
			case 16:
				if(thisButtonAll.save.value == 1){
					sprintf(BufTX,"Button Memory: Pressed\r\n");
				}
				else{
					sprintf(BufTX,"Button Memory: Not Pressed\r\n");
				}
				break;
			case 17:
				if(thisButtonAll.recall_1.value == 1){
					sprintf(BufTX,"Button Recall_1: Pressed\r\n");
				}
				else{
					sprintf(BufTX,"Button Recall_1: Not Pressed\r\n");
				}
				break;
			case 20:
				if(thisButtonAll.recall_2.value == 1){
					sprintf(BufTX,"Button Recall_2: Pressed\r\n");
				}
				else{
					sprintf(BufTX,"Button Recall_2: Not Pressed\r\n");
				}
				break;
			case 21:
				if(thisButtonAll.recall_3.value == 1){
					sprintf(BufTX,"Button Recall_3: Pressed\r\n");
				}
				else{
					sprintf(BufTX,"Button Recall_3: Not Pressed\r\n");
				}
				break;
			case 22:
				if(motor_slider.topology == SENSOR){
					sprintf(BufTX,"Slider motor: SENSOR \r\n");
				}else{
					sprintf(BufTX,"Slider motor: SENSORLESS \r\n");
				}
				break;
			case 23:
				if(motor_height.topology == SENSOR){
					sprintf(BufTX,"Height motor: SENSOR \r\n");
				}else{
					sprintf(BufTX,"Height motor: SENSORLESS \r\n");
				}
				break;
			case 24:
				if(motor_recliner.topology == SENSOR){
					sprintf(BufTX,"Recline motor: SENSOR \r\n");
				}else{
					sprintf(BufTX,"Recline motor: SENSORLESS \r\n");
				}
				break;

			case 25:

				break;
			default:
				sprintf(BufTX,"Invalid syntax for reading!\r\n");
				break;
		}
		sd_lld_write(&SD2, (uint8_t*) BufTX, UART_TX_LENGTH);
	}
}










