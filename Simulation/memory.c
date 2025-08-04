/*
 * memory.c
 *
 *  Created on: Jul 29, 2025
 *      Author: Admin
 */
#include "memory.h"

Memory_TypeDef thisMemory;

/**
 * @brief  Converts a decimal (base-10) number to a hexadecimal string.
 *
 * @param  a: The input number in decimal format.
 * @retval Pointer to a null-terminated hexadecimal string.
 */
const char* convert_Dec_to_Hex(uint32_t a) {
    static char hex_output[9];
    const char hex_digits[] = "0123456789ABCDEF";

    if (a == 0) {
        return "00";
    }

    int index = 8;
    hex_output[index] = '\0';

    while (a > 0) {
        hex_output[--index] = hex_digits[a % 16];
        a /= 16;
    }
    if (index == 7) {
        hex_output[--index] = '0';
    }
    return &hex_output[index];
}

/**
 * @brief  Merges three strings (a, b, c) into a single output string.
 *
 * @param  a: First string.
 * @param  b: Second string.
 * @param  c: Third string.
 * @param  output: Destination buffer to hold the merged result.
 * @param  output_size: Maximum size of the output buffer.
 * @retval None
 */
void merge_char(const char* a, const char* b, const char* c, char* output, size_t output_size) {
    uint32_t len_a = strlen(a), len_b = strlen(b), len_c = strlen(c);
    if (output_size < len_a + len_b + len_c + 1) {
        if (output_size >= 1) output[0] = '\0';
        return;
    }
    strcpy(output, a);
    strcat(output, b);
    strcat(output, c);
}

/**
 * @brief  Converts a hexadecimal string (up to 8 characters) to a 32-bit unsigned integer.
 *
 * @param  hex_str: Null-terminated string containing hexadecimal characters.
 * @retval Corresponding uint32_t value, or 0 if invalid.
 */
uint32_t convert_charHex_to_uint32_t(const char* hex_str) {
    uint32_t result = 0;
    // size_t len = strlen(hex_str);

    // if (len == 0 || len > 8) {
    //     return 0;
    // }

    // for (size_t i = 0; i < len; i++) {
    //     char c = hex_str[i];
    //     uint32_t digit;
    //     if (c >= '0' && c <= '9') {
    //         digit = c - '0';
    //     } else if (c >= 'A' && c <= 'F') {
    //         digit = c - 'A' + 10;
    //     } else if (c >= 'a' && c <= 'f') {
    //         digit = c - 'a' + 10;
    //     } else {
    //         return 0;
    //     }
    //     result = (result << 4) | digit;
    // }

    return result;
}

/**
 * @brief  Encodes three separate coordinate values (x, y, z) into a single 32-bit number.
 *
 * @param  x, y, z: Input decimal values (expected to be 0�255 for 2-digit hex representation).
 * @retval A 32-bit value representing the merged hex of x, y, and z.
 *
 * @note   Values are converted to 2-digit hex, concatenated, then parsed to uint32_t.
 */
uint32_t convert_coordinates_into_single_value(uint32_t x,uint32_t y, uint32_t z){
    char result[10],a[3],b[3],c[3];
    strcpy(a,convert_Dec_to_Hex(x));
    strcpy(b,convert_Dec_to_Hex(y));
    strcpy(c,convert_Dec_to_Hex(z));
    merge_char(a, b,c, result, sizeof(result));
    uint32_t d = convert_charHex_to_uint32_t(result);
    return d;
}

/**
 * @brief  Stores (x, y, z) coordinates into EEPROM as a single encoded value.
 *
 * @param  x, y, z: Coordinate values to be stored.
 * @param  id_recall: EEPROM storage index/ID.
 */
void MEM_Write(uint32_t x,uint32_t y, uint32_t z, uint16_t id_recall){
	uint32_t coordinates = convert_coordinates_into_single_value(x,y,z);
	EEPROM_Write(coordinates, id_recall);
}

/**
 * @brief  Splits a 6-character hex string into three separate 2-character strings.
 *
 * @param  a, b, c: Output buffers for the 3 parts (each must be at least size 3).
 * @param  input:  Input hex string of at least 6 characters (e.g. "0A1B2C").
 * @retval None
 */
void seperate_char( char* a,  char* b,  char* c, char* input) {
    if(strlen(input)==5){
        a[0] = '0';
        a[1] = input[0];
        a[2] = '\0';

        b[0] = input[1];
        b[1] = input[2];
        b[2] = '\0';

        c[0] = input[3];
        c[1] = input[4];
        c[2] = '\0';
    }else{
        a[0] = input[0];
        a[1] = input[1];
        a[2] = '\0';

        b[0] = input[2];
        b[1] = input[3];
        b[2] = '\0';

        c[0] = input[4];
        c[1] = input[5];
        c[2] = '\0';
    }
}

/**
 * @brief  Reads encoded coordinate data from EEPROM and decodes it to x, y, z values.
 *
 * @param  id_recall: EEPROM index/ID to read from.
 *
 * @note   Decoded values are stored in global variable `thisMemory`.
 */
void MEM_Read(uint16_t id_recall){
	uint32_t output_EEPROM = EEPROM_Read(id_recall);

    char str_source[10], x_axis_str[3],y_axis_str[3],z_axis_str[3];
    strcpy(str_source,convert_Dec_to_Hex(output_EEPROM));
    seperate_char(x_axis_str,y_axis_str,z_axis_str,str_source);
    thisMemory.x = convert_charHex_to_uint32_t(x_axis_str);
    thisMemory.y = convert_charHex_to_uint32_t(y_axis_str);
    thisMemory.z = convert_charHex_to_uint32_t(z_axis_str);
//    thisMemory.x = (uint16_t) convert_charHex_to_uint32_t(x_axis_str);
//    thisMemory.y = (uint16_t) convert_charHex_to_uint32_t(y_axis_str);
//    thisMemory.z = (uint16_t) convert_charHex_to_uint32_t(z_axis_str);
}

/**
 * @brief Retrieves seat axis data from memory for a specific recall position
 * @details Reads the stored coordinates from EEPROM at the specified recall ID
 *          and converts them to a SeatAxis_TypeDef structure
 * @param[in] id_recall The recall position ID (1-3) to retrieve
 * @return SeatAxis_TypeDef Structure containing the height, recliner, and slider positions
 */
SeatAxis_TypeDef MEM_GetSeatAxis(uint16_t id_recall)
{
	MEM_Read(id_recall);
	SeatAxis_TypeDef temp_memory_axis;
	temp_memory_axis.height   = thisMemory.x;
	temp_memory_axis.recliner = thisMemory.y;
	temp_memory_axis.slider   = thisMemory.z;
	return temp_memory_axis;
}












