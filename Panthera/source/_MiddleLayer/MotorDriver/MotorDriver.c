/*
 * MotorDriver.c
 *
 *  Created on: Jul 22, 2025
 *      Author: ADMIN
 */
#include "MotorDriver.h"
Motor_TypeDef motor_height, motor_recliner, motor_slider;
uint16_t cnt_mt_recliner_softstart;
uint16_t cnt_mt_recliner_available;
_Bool flag_mt_recliner_available;

// FIXME: Variable for testing sensorless
int32_t cnt_old_axis;
int32_t cnt_compensation;
_Bool flag_compensation;
int32_t debug_variable;

void MD_ConfigDriver(Motor_TypeDef *pMotor, GPIO_TypeDef pin_ina, GPIO_TypeDef pin_inb, GPIO_TypeDef pin_sel0,\
					 GPIO_TypeDef pin_sel1, GPIO_TypeDef pin_mul_en, GPIO_TypeDef pin_pwm, GPIO_TypeDef pin_hall);
void MD_ConfigTopology(Motor_TypeDef *pMotor, MotorTopology_TypeDef topology);
void MD_UpdateDriver(Motor_TypeDef *pMotor);
void MD_UpdateStatusMotor(Motor_TypeDef *pMotor, MotorStatus_TypeDef status);
void MD_UpdateMeasType(Motor_TypeDef *pMotor, MeasType_TypeDef meas_type);
_Bool MD_DetectHallSignalRising(Motor_TypeDef *pMotor);
_Bool MD_DetectHallSignalFalling(Motor_TypeDef *pMotor);
_Bool MD_DetectPeak(Motor_TypeDef *pMotor);
void MD_UpdateAxisSensor(Motor_TypeDef *pMotor);
void MD_UpdateAxisSensorless(Motor_TypeDef *pMotor);
void MD_HandlePeakCurrentMode(Motor_TypeDef *pMotor, MotorStatus_TypeDef status, uint16_t current_value, uint16_t current_limit_softstart, uint16_t current_limit_locked);

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
					 GPIO_TypeDef pin_sel1, GPIO_TypeDef pin_mul_en, GPIO_TypeDef pin_pwm, GPIO_TypeDef pin_hall)
{
    pMotor->driver.ina       = pin_ina;
    pMotor->driver.inb       = pin_inb;
    pMotor->driver.sel0      = pin_sel0;
    pMotor->driver.sel1      = pin_sel1;
    pMotor->driver.mul_en    = pin_mul_en;
    pMotor->driver.pwm       = pin_pwm;
    pMotor->driver.hall		 = pin_hall;
}

void MD_ConfigTopology(Motor_TypeDef *pMotor, MotorTopology_TypeDef topology)
{
	pMotor->topology = topology;
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
            pMotor->driver.pwm.value       = 0;
			break;

		// Stop motor
		default:
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
void MD_UpdateMeasType(Motor_TypeDef *pMotor, MeasType_TypeDef meas_type)
{
	pMotor->meas_type = meas_type;
	switch (pMotor->meas_type) {
		case MEAS_CURRENT:
			pMotor->driver.mul_en.value = 1;
			pMotor->driver.sel0.value  = pMotor->driver.ina.value ;
			pMotor->driver.sel1.value  = 0;
			break;

		case MEAS_THERMAL:
			pMotor->driver.mul_en.value = 1;
			pMotor->driver.sel0.value  = 0;
			pMotor->driver.sel1.value  = 1;
			break;

		case MEAS_VCC:
			pMotor->driver.mul_en.value = 1;
			pMotor->driver.sel0.value  = 1;
			pMotor->driver.sel1.value  = 1;
			break;

		case MEAS_NONE:
			pMotor->driver.mul_en.value = 0;
			pMotor->driver.sel0.value  = 0;
			pMotor->driver.sel1.value  = 0;
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
void MD_UpdateAxis(Motor_TypeDef *pMotor, uq15_t current_value)
{
	if (pMotor->topology == SENSOR) {
		pMotor->driver.hall.value = GPIO_ReadPin(&pMotor->driver.hall);
		MD_UpdateAxisSensor(pMotor);
	} else {
		pMotor->sensorless.previous_value = pMotor->sensorless.current_value;
		pMotor->sensorless.current_value  = current_value;
		MD_UpdateAxisSensorless(pMotor);
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
_Bool MD_DetectHallSignalRising(Motor_TypeDef *pMotor)
{
	if (pMotor->sensor.flag_checking_rising == true) {
		if (pMotor->driver.hall.value == 1)
		{
			if (pMotor->sensor.cnt_debounce_rising++ >= TIME_DEBOUNCE_RISING_EDGE)
			{
				pMotor->sensor.cnt_debounce_rising = 0;
				pMotor->sensor.flag_checking_rising = false;
				return true;
			}

		} else pMotor->sensor.cnt_debounce_rising = 0;
	}

	if (pMotor->driver.hall.value == 0)
	{
		pMotor->sensor.flag_checking_rising = true;
	}
	return false;
}

_Bool MD_DetectHallSignalFalling(Motor_TypeDef *pMotor)
{
	if (pMotor->sensor.flag_checking_falling == true) {
		if (pMotor->driver.hall.value == 0)
		{
			if (pMotor->sensor.cnt_debounce_falling++ >= TIME_DEBOUNCE_FALLING_EDGE)
			{
				pMotor->sensor.cnt_debounce_falling = 0;
				pMotor->sensor.flag_checking_falling = false;
				return true;
			}

		} else pMotor->sensor.cnt_debounce_falling = 0;
	}

	if (pMotor->driver.hall.value == 1)
	{
		pMotor->sensor.flag_checking_falling = true;
	}
	return false;
}

_Bool MD_DetectPeak(Motor_TypeDef *pMotor)
{
    int16_t diff = pMotor->sensorless.current_value - pMotor->sensorless.previous_value;

    if (diff > EPSILON) {
        if (pMotor->sensorless.flag_rising) {
        } else {
            if (pMotor->sensorless.cnt_falling > 0) {
                pMotor->sensorless.cnt_falling = 0;
            }

            pMotor->sensorless.cnt_rising++;

            if (pMotor->sensorless.cnt_rising >= NUM_DEBOUNCE_SSLESS) {
                pMotor->sensorless.flag_rising = 1;
            }
        }
    }
    else if (diff < -EPSILON) {
        if (pMotor->sensorless.flag_rising) {
            pMotor->sensorless.cnt_falling++;

            if (pMotor->sensorless.cnt_falling >= NUM_DEBOUNCE_SSLESS) {
                pMotor->sensorless.flag_falling = 1;
            }
        }
        else {
            pMotor->sensorless.cnt_rising = 0;
            pMotor->sensorless.cnt_falling = 0;
        }
    }

    if (pMotor->sensorless.flag_rising && pMotor->sensorless.flag_falling) {
        pMotor->sensorless.flag_rising = 0;
        pMotor->sensorless.flag_falling = 0;
        pMotor->sensorless.cnt_rising = 0;
        pMotor->sensorless.cnt_falling = 0;

        return true;
    }

    return false;
}

_Bool MD_DetectBottom(Motor_TypeDef *pMotor)
{
    int16_t diff = pMotor->sensorless.current_value - pMotor->sensorless.previous_value;

    if (diff < -EPSILON) {
        if (pMotor->sensorless.flag_falling) {
        } else {
            if (pMotor->sensorless.cnt_rising > 0) {
                pMotor->sensorless.cnt_rising = 0;
            }
            pMotor->sensorless.cnt_falling++;
            if (pMotor->sensorless.cnt_falling >= NUM_DEBOUNCE_SSLESS) {
                pMotor->sensorless.flag_falling = 1;
            }
        }
    }
    else if (diff > EPSILON) {
        if (pMotor->sensorless.flag_falling) {
            pMotor->sensorless.cnt_rising++;
            if (pMotor->sensorless.cnt_rising >= NUM_DEBOUNCE_SSLESS) {
                pMotor->sensorless.flag_rising = 1;
            }
        }
        else {
            pMotor->sensorless.cnt_falling = 0;
            pMotor->sensorless.cnt_rising = 0;
        }
    }

    if (pMotor->sensorless.flag_falling && pMotor->sensorless.flag_rising) {
        pMotor->sensorless.flag_falling = 0;
        pMotor->sensorless.flag_rising = 0;
        pMotor->sensorless.cnt_rising = 0;
        pMotor->sensorless.cnt_falling = 0;
        return true;
    }

    return false;
}

void MD_UpdateAxisSensor(Motor_TypeDef *pMotor)
{
	switch (pMotor->status) {
		case FORWARD:
			if (MD_DetectHallSignalRising(pMotor))	pMotor->axis++;
			break;

		case BACKWARD:
			if (MD_DetectHallSignalFalling(pMotor))	pMotor->axis--;
			break;
		default:
			break;
	}

//	if (pMotor->axis <= 0) {
//		pMotor->axis = 0;
//	}
}

void MD_UpdateAxisSensorless(Motor_TypeDef *pMotor)
{
	switch (pMotor->status) {
		case FORWARD:
			if (MD_DetectBottom(pMotor))	pMotor->axis++;
			break;

		case BACKWARD:
			if (MD_DetectBottom(pMotor))	pMotor->axis--;
			break;
		default:
			break;
	}

//	if (pMotor->axis <= 0) {
//		pMotor->axis = 0;
//	}
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
	MD_ConfigDriver(&motor_recliner, PIN_INA_1, PIN_INB_1, PIN_SEL0_1, PIN_SEL1_1, PIN_MulEN_1, PIN_PWM_1, PIN_HALL_1);
	MD_ConfigDriver(&motor_slider, PIN_INA_2, PIN_INB_2, PIN_SEL0_2, PIN_SEL1_2, PIN_MulEN_2, PIN_PWM_2, PIN_HALL_2);
	MD_ConfigDriver(&motor_height, PIN_INA_3, PIN_INB_3, PIN_SEL0_3, PIN_SEL1_3, PIN_MulEN_3, PIN_PWM_3, PIN_HALL_3);

	MD_ConfigTopology(&motor_recliner, SENSORLESS);
	MD_ConfigTopology(&motor_height, SENSORLESS);
	MD_ConfigTopology(&motor_slider, SENSORLESS);

	motor_height.name   = MOTOR_HEIGHT;
	motor_recliner.name = MOTOR_RECLINER;
	motor_slider.name   = MOTOR_SLIDER;

	MD_UpdateMeasType(&motor_height, MEAS_CURRENT);
	MD_UpdateMeasType(&motor_recliner, MEAS_CURRENT);
	MD_UpdateMeasType(&motor_slider, MEAS_CURRENT);
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
}

void MD_EnableAll(void)
{
	motor_recliner.state = SOFTSTART;
	motor_height.state = SOFTSTART;
	motor_slider.state = SOFTSTART;
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
void MD_Run(MotorName_TypeDef motor_name, MotorStatus_TypeDef status, uint16_t current_value)
{
	switch (motor_name) {
		case MOTOR_RECLINER:
			MD_HandlePeakCurrentMode(&motor_recliner, status, current_value, CURRENT_LIMIT_SOFTSTART_MT_RECLINER_Q15, CURRENT_LIMIT_LOCKED_MT_RECLINER_Q15);
			break;
		case MOTOR_SLIDER:
			MD_HandlePeakCurrentMode(&motor_slider, status, current_value, CURRENT_LIMIT_SOFTSTART_MT_SLIDER_Q15, CURRENT_LIMIT_LOCKED_MT_SLIDER_Q15);
			break;
		case MOTOR_HEIGHT:
			MD_HandlePeakCurrentMode(&motor_height, status, current_value, CURRENT_LIMIT_SOFTSTART_MT_HEIGHT_Q15, CURRENT_LIMIT_LOCKED_MT_HEIGHT_Q15);
			break;

		default:
			MD_StopAll();
			break;
	}
}

void MD_HandlePeakCurrentMode(Motor_TypeDef *pMotor, MotorStatus_TypeDef status, uint16_t current_value, uint16_t current_limit_softstart, uint16_t current_limit_locked)
{
	if (status != STOP) {
	    switch (pMotor->state) {
	        case SOFTSTART:
	        	if ((status == FORWARD) && (pMotor->block_status == BLOCK_FORWARD)) {
	                pMotor->state = BLOCK;
	                break;
				} else if ((status == BACKWARD) && (pMotor->block_status == BLOCK_BACKWARD)) {
	                pMotor->state = BLOCK;
	                break;
				}

	        	pMotor->block_status = BLOCK_NOT;
	        	pMotor->cnt_soft_start++;
	            if (current_value <= current_limit_softstart) {
	                MD_UpdateStatusMotor(pMotor, status);
	            } else {
	                MD_UpdateStatusMotor(pMotor, STOP);
	            }
	            if ((pMotor->cnt_soft_start) >= 2000) {
	            	pMotor->cnt_soft_start = 0;
	            	pMotor->flag_available = true;
	                pMotor->state = STEADY;

		        	if (pMotor->topology == SENSOR) {
		        		if (pMotor->status == FORWARD) {
			        		pMotor->axis++;
						} else pMotor->axis--;
					}
	            }
	            break;

	        case STEADY:
	            if (current_value <= current_limit_locked && pMotor->flag_available) {
	                MD_UpdateStatusMotor(pMotor, status);
	            } else {
	                MD_UpdateStatusMotor(pMotor, STOP);
	                pMotor->cnt_available++;
	                if (pMotor->cnt_available >= 10) {
    	                pMotor->state = BLOCK;
	                	pMotor->flag_available = false;
	                	pMotor->cnt_available  = 0;
	                	if (status == FORWARD) {
	                		pMotor->block_status = BLOCK_FORWARD;

						} else if (status == BACKWARD) {
							pMotor->block_status = BLOCK_BACKWARD;
							pMotor->axis = 0;
						}
	                }

	            }

	            break;

	        case BLOCK:
	            MD_UpdateStatusMotor(pMotor, STOP);
	            break;

	        default:
	            MD_UpdateStatusMotor(pMotor, STOP);
	            break;
	    }
	} else {
        MD_UpdateStatusMotor(pMotor, STOP);
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
void MD_UpdateMeas(MeasType_TypeDef meas_type)
{
	MD_UpdateMeasType(&motor_height, meas_type);
	MD_UpdateMeasType(&motor_recliner, meas_type);
	MD_UpdateMeasType(&motor_slider, meas_type);
}

void MD_UpdateToComm(MotorComm_TypeDef *pMotor)
{
	pMotor->motor_slider.topology   = motor_slider.topology;
	pMotor->motor_slider.ina_value  = motor_slider.driver.ina.value;
	pMotor->motor_slider.inb_value  = motor_slider.driver.inb.value;
	
	pMotor->motor_height.topology   = motor_height.topology;
	pMotor->motor_height.ina_value  = motor_height.driver.ina.value;
	pMotor->motor_height.inb_value  = motor_height.driver.inb.value;
	
	pMotor->motor_recliner.topology  = motor_recliner.topology;
	pMotor->motor_recliner.ina_value = motor_recliner.driver.ina.value;
	pMotor->motor_recliner.inb_value = motor_recliner.driver.inb.value;
}

/**
 * @brief Retrieves the current position of all seat axes
 * @details Reads the current axis position values from all motors and
 *          returns them in a unified SeatAxis structure
 * @return SeatAxis_TypeDef Structure containing the current position values
 *         for height, recliner and slider axes
 * @note Currently returns the same value (height axis) for all axes
 */
void MD_GetSeatAxis(SeatAxis_TypeDef *pSeat)
{
	pSeat->slider 	= motor_slider.axis;
	pSeat->height 	= motor_height.axis;
	pSeat->recliner = motor_recliner.axis;
}

void MD_ResetAxis(void)
{
	motor_slider.axis   = 0;
	motor_height.axis	= 0;
	motor_recliner.axis = 0;
}









