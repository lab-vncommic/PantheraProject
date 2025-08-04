/*
 * MotorDriver.c
 *
 *  Created on: Jul 22, 2025
 *      Author: ADMIN
 */
#include "MotorDriver.h"
Motor_TypeDef motor_height, motor_recliner, motor_slider;

void MD_ConfigDriver(Motor_TypeDef *pMotor, GPIO_TypeDef pin_ina, GPIO_TypeDef pin_inb, GPIO_TypeDef pin_sel0,\
					 GPIO_TypeDef pin_sel1, GPIO_TypeDef pin_mul_en, GPIO_TypeDef pin_pwm);
void MD_UpdateDriver(Motor_TypeDef *pMotor);
void MD_UpdateStatusMotor(Motor_TypeDef *pMotor, MotorStatus_TypeDef status);
void MD_UpdateMeasType(Motor_TypeDef *pMotor, DriverMeasType_TypeDef meas_type);
_Bool MD_CheckRisingEdge(uint8_t current_value);
void MD_UpdateAxis(Motor_TypeDef *pMotor);

/**
 * @brief Configures the motor driver GPIO pins
 *
 * @param pMotor Pointer to the motor structure to be configured
 * @param pin_ina GPIO pin for the INA signal of the motor driver
 * @param pin_inb GPIO pin for the INB signal of the motor driver
 * @param pin_sel0 GPIO pin for the SEL0 signal of the motor driver
 * @param pin_sel1 GPIO pin for the SEL1 signal of the motor driver
 * @param pin_mul_en GPIO pin for the MUL_EN signal of the motor driver
 * @param pin_pwm GPIO pin for the PWM signal of the motor driver
 *
 * @note This function assigns the specified GPIO pins to the driver configuration
 *       in the provided motor structure.
 */
void MD_ConfigDriver(Motor_TypeDef *pMotor, GPIO_TypeDef pin_ina, GPIO_TypeDef pin_inb, GPIO_TypeDef pin_sel0,\
					 GPIO_TypeDef pin_sel1, GPIO_TypeDef pin_mul_en, GPIO_TypeDef pin_pwm)
{
    pMotor->driver.ina       = pin_ina;
    pMotor->driver.inb       = pin_inb;
    pMotor->driver.sel0      = pin_sel0;
    pMotor->driver.sel1      = pin_sel1;
    pMotor->driver.mul_en    = pin_mul_en;
    pMotor->driver.pwm       = pin_pwm;
}

/**
 * @brief Updates the motor driver GPIO pins based on current configuration
 *
 * This function configures the motor driver GPIO pins according to the values
 * stored in the Motor_TypeDef structure. It sets or clears pins for:
 * - Input A (INA)
 * - Input B (INB)
 * - Select 0 (SEL0)
 * - Select 1 (SEL1)
 * - Multiple Enable (MUL_EN)
 * - PWM control
 *
 * @param[in] pMotor Pointer to the motor configuration structure
 *
 * @note This function should be called after any changes to the motor driver 
 *       configuration to update the physical hardware state
 */
void MD_UpdateDriver(Motor_TypeDef *pMotor)
{
    if (pMotor->driver.ina.value == 1) {
        GPIO_SetPin(&pMotor->driver.ina);
    } else GPIO_ClearPin(&pMotor->driver.ina);

    if (pMotor->driver.inb.value == 1) {
        GPIO_SetPin(&pMotor->driver.inb);
    } else GPIO_ClearPin(&pMotor->driver.inb);

    if (pMotor->driver.sel0.value == 1) {
        GPIO_SetPin(&pMotor->driver.sel0);
    } else GPIO_ClearPin(&pMotor->driver.sel0);

    if (pMotor->driver.sel1.value == 1) {
        GPIO_SetPin(&pMotor->driver.sel1);
    } else GPIO_ClearPin(&pMotor->driver.sel1);

    if (pMotor->driver.mul_en.value == 1) {
        GPIO_SetPin(&pMotor->driver.mul_en);
    } else GPIO_ClearPin(&pMotor->driver.mul_en);

    if (pMotor->driver.pwm.value == 1) {
        GPIO_SetPin(&pMotor->driver.pwm);
    } else GPIO_ClearPin(&pMotor->driver.pwm);
}

/**
 * @brief Updates the status of a motor and configures its driver pins accordingly
 * 
 * @param pMotor Pointer to the motor structure to be updated
 * @param status New status to be applied to the motor (FORWARD, BACKWARD, or STOP)
 * 
 * @details This function updates the status field of the motor structure and configures
 * the motor's driver pins (INA, INB, PWM) based on the new status:
 *   - FORWARD: INA=1, INB=0, PWM=1
 *   - BACKWARD: INA=0, INB=1, PWM=1
 *   - STOP (and default): INA=0, INB=0, PWM=0
 * 
 * @return None
 */
void MD_UpdateStatusMotor(Motor_TypeDef *pMotor, MotorStatus_TypeDef status)
{
	pMotor->status = status;
	switch (pMotor->status) {
		case FORWARD:
            pMotor->driver.ina.value       = 1;
            pMotor->driver.inb.value       = 0;
            pMotor->driver.pwm.value       = 1;
			break;

		case BACKWARD:
            pMotor->driver.ina.value       = 0;
            pMotor->driver.inb.value       = 1;
            pMotor->driver.pwm.value       = 1;
			break;

		case STOP:
            pMotor->driver.ina.value       = 0;
            pMotor->driver.inb.value       = 0;
            pMotor->driver.pwm.value       = 0;
			break;

		// Stop motor
		default:
            pMotor->driver.ina.value       = 0;
            pMotor->driver.inb.value       = 0;
            pMotor->driver.pwm.value       = 0;
			break;
	}
}

/**
 * @brief Updates the motor driver status for multiple sensing options
 *
 * This function configures the motor driver's multi-sensor pins based on the requested
 * measurement type. It enables the multi-sensor functionality and sets the appropriate
 * selection pins to measure current, temperature, or supply voltage.
 *
 * @param[in,out] pMotor    Pointer to the motor structure to be configured
 * @param[in]     meas_type Measurement type to be selected (CURRENT, THERMAL, VCC)
 *
 * @note The default measurement type is CURRENT if an invalid type is provided
 */
void MD_UpdateMeasType(Motor_TypeDef *pMotor, DriverMeasType_TypeDef meas_type)
{
	// Enable multi sens pin
	pMotor->driver.mul_en.value = 1;

	pMotor->meas_type = meas_type;
	switch (pMotor->meas_type) {
		case CURRENT:
			pMotor->driver.sel0.value  = pMotor->driver.ina.value ;
			pMotor->driver.sel1.value  = 0;
			break;

		case THERMAL:
			pMotor->driver.sel0.value  = 0;
			pMotor->driver.sel1.value  = 1;
			break;

		case VCC:
			pMotor->driver.sel0.value  = 1;
			pMotor->driver.sel1.value  = 1;
			break;

		// motor sensor type = current
		default:
			pMotor->driver.sel0.value  = pMotor->driver.ina.value ;
			pMotor->driver.sel1.value  = 0;
			break;
	}
}

/**
 * @brief Updates the axis position counter based on hall sensor edge detection
 *
 * This function monitors the hall sensor for rising edges and updates the motor's
 * axis position counter accordingly. When a rising edge is detected, the function
 * increments the axis counter if the motor is moving forward, or decrements it
 * if the motor is moving backward.
 *
 * @param[in,out] pMotor Pointer to the motor structure containing status and sensor data
 *
 * @note This function should be called periodically to track motor position
 */
void MD_UpdateAxis(Motor_TypeDef *pMotor)
{
    if (MD_CheckRisingEdge(pMotor->driver.hall.value)) {
        if (pMotor->status == FORWARD) {
            pMotor->axis += 1;
        } else if (pMotor->status == BACKWARD) {
            pMotor->axis -= 1;
        }
    }
}

/**
 * @brief Detects a rising edge on a signal with debounce protection
 *
 * @details This function implements a debounced rising edge detector. It requires the
 *          high state (1) to persist for at least 100 consecutive calls before
 *          triggering. Once triggered, it will not detect another rising edge until
 *          the signal returns to low state (0).
 *
 * @param[in] current_value Current value of the signal (0 for low, 1 for high)
 *
 * @return true  Rising edge detected (after debouncing)
 * @return false No rising edge detected or debouncing in progress
 *
 * @note This function maintains state between calls using static variables
 */
_Bool MD_CheckRisingEdge(uint8_t current_value)
{
	static _Bool is_edge_detection_enabled = true;
	static uint8_t counter = 0;

	if (is_edge_detection_enabled == true) {
		if (current_value == 1)
		{
			if (counter++ >= TIME_DEBOUNCE_RISING_EDGE)
			{
				counter = 0;
				is_edge_detection_enabled = false;
				return true;
			}

		} else counter = 0;
	}

	if (current_value == 0)
	{
		is_edge_detection_enabled = true;
	}
	return false;
}

/**
 * @brief Initializes all motor drivers in the system
 *
 * @details This function performs the initialization of three motors:
 *          - Height motor
 *          - Recliner motor
 *          - Slider motor
 *          For each motor, it configures the driver pins, updates the MULSEN 
 *          (Multi-Sense) status with current values, and finally stops all motors
 *          to ensure a safe initial state.
 *
 * @pre The PIN constants for all motors must be defined before calling this function
 * @post All motors will be configured and stopped after initialization
 *
 * @return None
 */
void MD_Init(void)
{
	MD_ConfigDriver(&motor_slider, PIN_INA_1, PIN_INB_1, PIN_SEL0_1, PIN_SEL1_1, PIN_MulEN_1, PIN_PWM_1);
	MD_ConfigDriver(&motor_height, PIN_INA_2, PIN_INB_2, PIN_SEL0_2, PIN_SEL1_2, PIN_MulEN_2, PIN_PWM_2);
	MD_ConfigDriver(&motor_recliner, PIN_INA_3, PIN_INB_3, PIN_SEL0_3, PIN_SEL1_3, PIN_MulEN_3, PIN_PWM_3);

	MD_UpdateMeasType(&motor_height, CURRENT);
	MD_UpdateMeasType(&motor_recliner, CURRENT);
	MD_UpdateMeasType(&motor_slider, CURRENT);
	MD_StopAll();
}

/**
 * @brief Stops all motors in the system
 *
 * This function stops all motors (height, recliner, and slider) by:
 * 1. Setting the status of each motor to STOP
 * 2. Updating the driver for each motor to apply the new status
 *
 * @return None
 *
 * @note This is a safety function that can be called to immediately halt all motor operations
 * @see MD_UpdateStatusMotor(), MD_UpdateDriver()
 */
void MD_StopAll(void)
{
	// Update status to STOP for all motors
	MD_UpdateStatusMotor(&motor_height, STOP);
	MD_UpdateStatusMotor(&motor_recliner, STOP);
	MD_UpdateStatusMotor(&motor_slider, STOP);

	// Update driver for all motors
	MD_UpdateDriver(&motor_height);
	MD_UpdateDriver(&motor_recliner);
	MD_UpdateDriver(&motor_slider);
}

/**
 * @brief Runs a specified motor with a given status
 * @details Activates the selected motor (height, recliner, or slider) with
 *          the specified status (FORWARD, BACKWARD, or STOP). Updates the motor
 *          driver configuration and tracks axis position changes.
 * @param[in] motor_name The motor to control (MOTOR_HEIGHT, MOTOR_RECLINER, MOTOR_SLIDER)
 * @param[in] status The operation to perform (FORWARD, BACKWARD, or STOP)
 * @return None
 */
void MD_Run(MotorName_TypeDef motor_name, MotorStatus_TypeDef status)
{
	switch (motor_name) {
		case MOTOR_HEIGHT:
			MD_UpdateStatusMotor(&motor_height, status);
			MD_UpdateDriver(&motor_height);
			MD_UpdateAxis(&motor_height);
			break;

		case MOTOR_RECLINER:
			MD_UpdateStatusMotor(&motor_recliner, status);
			MD_UpdateDriver(&motor_recliner);
			MD_UpdateAxis(&motor_recliner);
			break;

		case MOTOR_SLIDER:
			MD_UpdateStatusMotor(&motor_slider, status);
			MD_UpdateDriver(&motor_slider);
			MD_UpdateAxis(&motor_slider);
			break;

		default:
			MD_StopAll();
			break;
	}
}

/**
 * @brief   Updates measurement types and drivers for all motors
 * @details This function updates the measurement type settings for all motors
 *          (height, recliner, slider) and then updates their respective drivers
 *          with the new configuration.
 *
 * @param[in] meas_type  The measurement type to be applied to all motors
 *
 * @return    None
 *
 * @note      This function should be called whenever the measurement type
 *            needs to be changed for all motors simultaneously.
 *
 * @see       MD_UpdateMeasType
 * @see       MD_UpdateDriver
 */
void MD_UpdateMeas(DriverMeasType_TypeDef meas_type)
{
	MD_UpdateMeasType(&motor_height, meas_type);
	MD_UpdateMeasType(&motor_recliner, meas_type);
	MD_UpdateMeasType(&motor_slider, meas_type);
	MD_UpdateDriver(&motor_height);
	MD_UpdateDriver(&motor_recliner);
	MD_UpdateDriver(&motor_slider);
}

/**
 * @brief Retrieves the current position of all seat axes
 * @details Reads the current axis position values from all motors and
 *          returns them in a unified SeatAxis structure
 * @return SeatAxis_TypeDef Structure containing the current position values
 *         for height, recliner and slider axes
 * @note Currently returns the same value (height axis) for all axes
 */
SeatAxis_TypeDef MD_GetSeatAxis(void)
{
	SeatAxis_TypeDef temp_axis;
	temp_axis.height 	= motor_height.axis;
	temp_axis.recliner  = motor_height.axis;
	temp_axis.slider 	= motor_height.axis;
	return temp_axis;
}





