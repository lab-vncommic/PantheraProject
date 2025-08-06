/*
 * communication.c
 *
 *  Created on: Aug 1, 2025
 *      Author: Admin
 */

#include "Communication.h"

Communication_TypeDef thisCommunication;

/**
 * @brief Converts a buffer of bytes into a hexadecimal string.
 *
 * Example:
 * Input: buf = {0x1A, 0x2B}, len = 2
 * Output: out = "1A2B"
 *
 * @param buf  Input buffer containing raw bytes.
 * @param len  Length of input buffer.
 * @param out  Output buffer for hex string
 */
void convert_BufRX_to_HexString(char *buf, size_t len, char *out) {
    for (size_t i = 0; i < len; i++) {
        sprintf(&out[i * 2], "%02X", buf[i]);
    }
    out[len * 2] = '\0';
}

/**
 * @brief Separates a hex string into three parts: ID, mode, and value.
 * @param a Output buffer for ID (at least 5 bytes).
 * @param b Output buffer for mode (at least 3 bytes).
 * @param c Output buffer for value (at least 3 bytes).
 * @param input Input hex string (must be at least 8 characters).
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

/**
 * @brief Converts a hexadecimal string to a uint32_t integer.
 *
 * @param hex_str A null-terminated hex string (max 8 characters).
 * @return uint32_t Converted integer value.
 */
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

/**
 * @brief Parses received UART data and stores extracted values in the global communication structure.
 */
void Communication_Receive(void){
	char string_input[9];
	convert_BufRX_to_HexString(BufRX, UART_RX_LENGTH, string_input);
    char a[5],b[3],c[3];
    seperate_char_communication(a,b,c,string_input);
    thisCommunication.ID_Message = (uint16_t) convert_charHex_to_uint32_t_Communication(a);
    thisCommunication.Write_Read = (uint16_t) convert_charHex_to_uint32_t_Communication(b);
    thisCommunication.value		 = (uint16_t) convert_charHex_to_uint32_t_Communication(c);
}

/**
 * @brief Sends a UART message with the current state of a button.
 *
 * @param value  1 if pressed, 0 if not pressed.
 * @param name   Name of the button.
 */
void Comm_SendButtonState(uint8_t value, const char* name) {
    if (value)
        snprintf(BufTX, UART_TX_LENGTH, "%s: Pressed\r\n", name);
    else
        snprintf(BufTX, UART_TX_LENGTH, "%s: Not Pressed\r\n", name);
}

/**
 * @brief Sends the current state of a motor (forward, backward, stop).
 *
 * @param ina   INA pin state.
 * @param inb   INB pin state.
 * @param stop  Whether the motor is stopped.
 * @param name  Motor name for display.
 */
void Comm_SendMotorState(uint8_t ina, uint8_t inb, uint8_t stop, const char* name) {
    if (ina)
        snprintf(BufTX, UART_TX_LENGTH, "%s motor: Seat Forward\r\n", name);
    else if (inb)
        snprintf(BufTX, UART_TX_LENGTH, "%s motor: Seat Backward\r\n", name);
    else if (stop)
        snprintf(BufTX, UART_TX_LENGTH, "%s motor: Stop\r\n", name);
    else
        snprintf(BufTX, UART_TX_LENGTH, "%s motor: Unknown State\r\n", name);
}

/**
 * @brief Sends the topology (sensor or sensorless) of a motor.
 *
 * @param topology Value indicating topology type.
 * @param name     Name of the motor.
 */
void Comm_SendTopology(uint8_t topology, const char* name) {
    snprintf(BufTX, UART_TX_LENGTH, "%s motor: %s\r\n", name, (topology == SENSOR) ? "SENSOR" : "SENSORLESS");
}

/**
 * @brief Sends appropriate data based on the received message ID and read/write flag.
 *
 * This function only sends data if the mode is READ_MODE.
 */
void Communication_Transmit(void){
	if (thisCommunication.Write_Read != READ_MODE) return;

	memset(BufTX, 0, sizeof(BufTX));

	switch (thisCommunication.ID_Message) {
		case 1 : Comm_SendButtonState(thisButtonAll.slider_1.value,   "Positioning KEY: Seat Slider Forward");   break;
		case 2 : Comm_SendButtonState(thisButtonAll.slider_2.value,   "Positioning KEY: Seat Slider Backward");  break;
		case 3 : Comm_SendButtonState(thisButtonAll.height_1.value,   "Positioning KEY: Seat Height Upward");    break;
		case 4 : Comm_SendButtonState(thisButtonAll.height_2.value,   "Positioning KEY: Seat Height Downward");  break;
		case 5 : Comm_SendButtonState(thisButtonAll.recliner_1.value, "Positioning KEY: Seat Recline Forward");  break;
		case 7 : Comm_SendButtonState(thisButtonAll.recliner_2.value, "Positioning KEY: Seat Recline Backward"); break;
		case 16: Comm_SendButtonState(thisButtonAll.save.value,       "Button Memory");                          break;
		case 17: Comm_SendButtonState(thisButtonAll.recall_1.value,   "Button Recall_1");                        break;
		case 20: Comm_SendButtonState(thisButtonAll.recall_2.value,   "Button Recall_2");                        break;
		case 21: Comm_SendButtonState(thisButtonAll.recall_3.value,   "Button Recall_3");                        break;

		case 10: Comm_SendMotorState(motor_slider.driver.ina.value,   motor_slider.driver.inb.value,   motor_slider.status   == STOP, "Slider"); break;
		case 11: Comm_SendMotorState(motor_slider.driver.ina.value,   motor_slider.driver.inb.value,   motor_slider.status   == STOP, "Slider"); break;
		case 12: Comm_SendMotorState(motor_height.driver.ina.value,   motor_height.driver.inb.value,   motor_height.status   == STOP, "Height"); break;
		case 13: Comm_SendMotorState(motor_height.driver.ina.value,   motor_height.driver.inb.value,   motor_height.status   == STOP, "Height"); break;
		case 14: Comm_SendMotorState(motor_recliner.driver.ina.value, motor_recliner.driver.inb.value, motor_recliner.status == STOP, "Recline"); break;
		case 15: Comm_SendMotorState(motor_recliner.driver.ina.value, motor_recliner.driver.inb.value, motor_recliner.status == STOP, "Recline"); break;

		case 22: Comm_SendTopology(motor_slider.topology,   "Slider");  break;
		case 23: Comm_SendTopology(motor_height.topology,   "Height");  break;
		case 24: Comm_SendTopology(motor_recliner.topology, "Recline"); break;

		case 0:

			break;
		case 25:

			break;
		default:
			snprintf(BufTX, UART_TX_LENGTH, "Invalid syntax for reading!\r\n");
			break;
	}
	// Transmit result over UART
	sd_lld_write(&SD2, (uint8_t*) BufTX, UART_TX_LENGTH);
}










