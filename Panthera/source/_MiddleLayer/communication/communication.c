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
			case 1:

				break;
			case 2:

				break;
			case 3:

				break;
			case 4:

				break;
			case 5:

				break;
			case 6:

				break;
			case 7:

				break;
			case 8:

				break;
			case 9:

				break;

			case 10:

				break;
			case 11:

				break;
			case 12:

				break;
			case 13:

				break;
			case 14:

				break;
			case 15:

				break;
			case 16:

				break;
			case 17:

				break;
			case 18:

				break;
			case 19:

				break;
			case 20:

				break;
			case 21:

				break;
			case 22:

				break;
			case 23:

				break;
			case 24:

				break;

			case 25:

				break;
			default:

				break;
		}
	}
}










