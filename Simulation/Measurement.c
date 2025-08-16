/*
 * Measurement.c
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */
#include "Measurement.h"
MeasAll_TypeDef thisMeas;

void MEAS_Config(Meas_TypeDef *pMeas, q15_t offset, uq31_t scale_factor, uq15_t alpha, uq15_t high_limit_soft, float gain_f32, float offset_f32, uint8_t calc_method);
void MEAS_GetRawValue(Meas_TypeDef *pMeas, q15_t raw_value);
void MEAS_GetValueQ15(Meas_TypeDef *pMeas);
void MEAS_GetValueF32(Meas_TypeDef *pMeas);
void MEAS_GetValue(MeasType_TypeDef meas_type);
void MEAS_GetRealValue(MeasType_TypeDef meas_type);
uint16_t MEAS_LPF(uint16_t new_value, uint16_t prev_value, uint16_t alpha);

/**
 * @brief  Configures a measurement object with specified parameters
 * @details This function initializes and configures a measurement object with the provided
 *          parameters including offset, scale factor, filter constant, limits and calculation method.
 *          Raw and calculated values are reset to zero during configuration.
 *
 * @param[out] pMeas            Pointer to measurement object to be configured
 * @param[in]  offset           Offset value in q15 format
 * @param[in]  scale_factor     Scale factor in uq31 format
 * @param[in]  alpha            Filter constant in uq15 format
 * @param[in]  high_limit_soft  High limit threshold in uq15 format
 * @param[in]  gain_f32         Gain value in floating point format
 * @param[in]  offset_f32       Offset value in floating point format
 * @param[in]  calc_method      Calculation method identifier
 *
 * @return None
 *
 * @note Function returns immediately if pMeas is NULL
 */
void MEAS_Config(Meas_TypeDef *pMeas, q15_t offset, uq31_t scale_factor, uq15_t alpha, uq15_t high_limit_soft, float gain_f32, float offset_f32, uint8_t calc_method)
{
    if(pMeas == NULL)
        return;

    pMeas->raw 			   = 0U;
    pMeas->value_f32       = 0.0f;
    pMeas->value_q15	   = 0U;
    pMeas->offset 		   = offset;
    pMeas->scale_factor    = scale_factor;
    pMeas->alpha 		   = alpha;
    pMeas->high_limit_soft = high_limit_soft;
    pMeas->gain_f32        = gain_f32;
    pMeas->offset_f32	   = offset_f32;
    pMeas->calc_method	   = calc_method;
}

/**
 * @brief Sets the raw value in a measurement structure
 * 
 * @param pMeas Pointer to the measurement structure
 * @param raw_value Raw value to be stored in the measurement structure
 * 
 * @return None
 * 
 * @note If pMeas is NULL, the function returns without performing any operation
 */
void MEAS_GetRawValue(Meas_TypeDef *pMeas, q15_t raw_value)
{
    if(pMeas == NULL)
        return;

    pMeas->raw = raw_value;
}

/**
 * @brief Calculates the Q15 representation of a measurement value
 *
 * @details Converts the raw measurement value to Q15 fixed-point representation.
 * If the calculation method is CALC_METHOD_Q15, it applies the scale factor
 * to the raw value. Otherwise, it converts the raw value to Q15 format without
 * scaling (using scale factor of 1.0).
 *
 * @param[in,out] pMeas Pointer to the measurement structure
 *                      The function will update the value_q15 field
 *
 * @note The raw value is right-shifted by 1 bit before conversion to prevent overflow
 * @return None
 */
void MEAS_GetValueQ15(Meas_TypeDef *pMeas)
{
    if(pMeas == NULL)
    	return;
	pMeas->prev_value = pMeas->value_q15;
    if (pMeas->calc_method == CALC_METHOD_Q15) {
        pMeas->value_q15 = _q15_mul(pMeas->raw >> 1 , pMeas->scale_factor);
	} else pMeas->value_q15 = _q15_mul(pMeas->raw >>1 , _q15(1));

	pMeas->value_q15 = MEAS_LPF(pMeas->value_q15, pMeas->prev_value, 400); // 15666
}

uint16_t MEAS_LPF(uint16_t new_value, uint16_t prev_value, uint16_t alpha)
{
    int32_t temp1 = ((int32_t)alpha * new_value) >> 15;
    int16_t one_minus_alpha = 32767 - alpha;
    int32_t temp2 = ((int32_t)one_minus_alpha * prev_value) >> 15;
    return (uint16_t)(temp1 + temp2);
}

/**
 * @brief Converts measurement values to floating-point format
 *
 * This function calculates the floating-point representation (value_f32) of a measurement
 * based on the calculation method defined in the measurement structure.
 * - For CALC_METHOD_Q15: Converts from Q15 fixed-point value using the soft high limit
 * - For other methods: Applies gain and offset to the raw value
 *
 * @param[in,out] pMeas Pointer to the measurement structure to process
 *                      On output, the value_f32 field will be updated
 *
 * @return None
 *
 * @note The function does nothing if pMeas is NULL
 */
void MEAS_GetValueF32(Meas_TypeDef *pMeas)
{
    if(pMeas == NULL)
        return;

    if (pMeas->calc_method == CALC_METHOD_Q15) {
    	pMeas->value_f32 = _q15_i2f((pMeas->value_q15) * pMeas->high_limit_soft);
	} else pMeas->value_f32 = pMeas->raw * pMeas->gain_f32 + pMeas->offset_f32;
}

/**
 * @brief Initialize measurement module
 *
 * @details This function initializes the ADC driver and configures measurement
 * parameters for current and thermal measurement objects. It sets up scale factors,
 * offsets, and limits for each measurement channel.
 *
 * The function configures:
 * - Current measurements for motors 1-3 using Q15 calculation method
 * - Thermal measurements for motors 1-3 using F32 calculation method
 *
 * @return None
 *
 * @note Must be called before using any measurement functions
 */
void MEAS_Init(void)
{
	// Config driver ADC
	ADC_Init(&thatADC);

	// Config parameters for measurement objects
    MEAS_Config(&thisMeas.current_slider, CURRENT_MT1_OFFSET, CURRENT_MT1_SCALE_FACTOR, 0U, CURRENT_MT1_HIGH_LIMIT_SOFT, 0U, 0U, CALC_METHOD_Q15);
    MEAS_Config(&thisMeas.current_height, CURRENT_MT2_OFFSET, CURRENT_MT2_SCALE_FACTOR, 0U, CURRENT_MT2_HIGH_LIMIT_SOFT, 0U, 0U, CALC_METHOD_Q15);
    MEAS_Config(&thisMeas.current_recliner, CURRENT_MT3_OFFSET, CURRENT_MT3_SCALE_FACTOR, 0U, CURRENT_MT3_HIGH_LIMIT_SOFT, 0U, 0U, CALC_METHOD_Q15);
    MEAS_Config(&thisMeas.thermal_slider, 0U, 0U, 0U, 0U, THERMAL_MT1_GAIN_F32, THERMAL_MT1_OFFSET_F32, CALC_METHOD_F32);
    MEAS_Config(&thisMeas.thermal_height, 0U, 0U, 0U, 0U, THERMAL_MT2_GAIN_F32, THERMAL_MT2_OFFSET_F32, CALC_METHOD_F32);
    MEAS_Config(&thisMeas.thermal_recliner, 0U, 0U, 0U, 0U, THERMAL_MT3_GAIN_F32, THERMAL_MT3_OFFSET_F32, CALC_METHOD_F32);
}

/**
 * @brief Retrieves and processes measurement values from ADC
 *
 * @details This function performs the following operations:
 *          1. Initiates ADC conversion process
 *          2. Retrieves raw values from ADC driver for motor current sensors
 *          3. Converts the raw measurements to Q15 fixed-point format for both
 *             current and thermal measurements for all three motors
 *
 * @note The function depends on ADC_startOfConversion() to be properly initialized
 *
 * @see MEAS_GetRawValue
 * @see MEAS_GetValueQ15
 */
void MEAS_GetValue(MeasType_TypeDef meas_type)
{
	// Get adc value
	ADC_startOfConversion();

	if (meas_type) {
		// Get raw value from adc driver
	    MEAS_GetRawValue(&thisMeas.thermal_slider, thatADC.ADC_MulSens_1);
	    MEAS_GetRawValue(&thisMeas.thermal_height, thatADC.ADC_MulSens_2);
	    MEAS_GetRawValue(&thisMeas.thermal_recliner, thatADC.ADC_MulSens_3);

		// Get value in Q15 concept
	    MEAS_GetValueQ15(&thisMeas.thermal_slider);
	    MEAS_GetValueQ15(&thisMeas.thermal_height);
	    MEAS_GetValueQ15(&thisMeas.thermal_recliner);
	} else {
		// Get raw value from adc driver
	    MEAS_GetRawValue(&thisMeas.current_slider, thatADC.ADC_MulSens_1);
	    MEAS_GetRawValue(&thisMeas.current_height, thatADC.ADC_MulSens_2);
	    MEAS_GetRawValue(&thisMeas.current_recliner, thatADC.ADC_MulSens_3);

		// Get value in Q15 concept
		MEAS_GetValueQ15(&thisMeas.current_slider);
		MEAS_GetValueQ15(&thisMeas.current_height);
		MEAS_GetValueQ15(&thisMeas.current_recliner);
	}

	// FIXME: Only use for debugging
	MEAS_GetRealValue(meas_type);
}

/**
 * @brief Retrieves real measurement values for all motor sensors
 *
 * @details This function updates the current and thermal measurement values
 * for all three motors by calling MEAS_GetValueF32() on each sensor value.
 * The measurements include:
 * - Current values for motors 1, 2, and 3
 * - Thermal (temperature) values for motors 1, 2, and 3
 *
 * All values are stored in the thisMeas global structure.
 *
 * @return None
 */
void MEAS_GetRealValue(MeasType_TypeDef meas_type)
{
	if (meas_type) {
	    MEAS_GetValueF32(&thisMeas.thermal_slider);
	    MEAS_GetValueF32(&thisMeas.thermal_height);
	    MEAS_GetValueF32(&thisMeas.thermal_recliner);
	} else {
		MEAS_GetValueF32(&thisMeas.current_slider);
		MEAS_GetValueF32(&thisMeas.current_height);
		MEAS_GetValueF32(&thisMeas.current_recliner);
	}
}

void MEAS_UpdateValue(MeasSystem_TypeDef *pMeas, MeasType_TypeDef meas_type)
{
	MEAS_GetValue(meas_type);

	pMeas->current_slider_q15   = thisMeas.current_slider.value_q15;
	pMeas->current_height_q15   = thisMeas.current_height.value_q15;
	pMeas->current_recliner_q15 = thisMeas.current_recliner.value_q15;
	pMeas->thermal_slider_q15   = thisMeas.thermal_slider.value_q15;
	pMeas->thermal_height_q15   = thisMeas.thermal_height.value_q15;
	pMeas->thermal_recliner_q15 = thisMeas.thermal_recliner.value_q15;
}


