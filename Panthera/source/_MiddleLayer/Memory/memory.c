/*
 * memory.c
 *
 *  Created on: Jul 29, 2025
 *      Author: Admin
 */
#include "memory.h"

Memory_TypeDef thisMemory[3];

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
 * @brief  Encodes three decimal values into a single 32-bit hexadecimal representation.
 *
 * @param  x, y, z: Input decimal values (expected to be <= 255).
 * @retval A 32-bit unsigned integer encoding the 3 values as 6 hex digits.
 *
 * @note   Each value is converted to 2-digit hex, concatenated, and then parsed back to uint32.
 */
uint32_t input_EEPROM(uint32_t x,uint32_t y, uint32_t z){
    char result[10],a[3],b[3],c[3];
    strcpy(a,convert_Dec_to_Hex(x));
    strcpy(b,convert_Dec_to_Hex(y));
    strcpy(c,convert_Dec_to_Hex(z));
    merge_char(a, b,c, result, sizeof(result));
    uint32_t d = convert_charHex_to_uint32_t(result);
    return d;
}

/**
 * @brief  Splits a 6-character hex string into three separate 2-character strings.
 *
 * @param  a, b, c: Output buffers for the 3 parts (each must be at least size 3).
 * @param  input:  Input hex string of at least 6 characters (e.g. "0A1B2C").
 * @retval None
 */
void seperate_char( char* a,  char* b,  char* c, char* input) {
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

/**
 * @brief  Decodes a single 32-bit EEPROM-encoded value and stores it into the memory array.
 *
 * @param  output_EEPROM: 32-bit value previously encoded by input_EEPROM().
 * @retval None
 *
 * @note   The function uses a rotating index (0→1→2→0) to fill `thisMemory[3]`.
 */
void position_Memory(uint32_t output_EEPROM){
    char a[10], b[3],c[3],e[3];
    strcpy(a,convert_Dec_to_Hex(output_EEPROM));
    seperate_char(b,c,e,a);
    static uint32_t index_memory = 0;
    thisMemory[index_memory].x = convert_charHex_to_uint32_t(b);
    thisMemory[index_memory].y = convert_charHex_to_uint32_t(c);
    thisMemory[index_memory].z = convert_charHex_to_uint32_t(e);
    index_memory ++;
    if(index_memory == 3){
    	index_memory = 0;
    }
}












