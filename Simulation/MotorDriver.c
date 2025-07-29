/*
 * MotorDriver.c
 *
 *  Created on: Jul 22, 2025
 *      Author: ADMIN
 */
#include "MotorDriver.h"
Motor_TypeDef motor_height, motor_recliner, motor_slider;

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

void MD_UpdateStatusMulsen(Motor_TypeDef *pMotor, MotorMeasType_TypeDef meas_type)
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

void MD_Init(void)
{
	// MD_ConfigDriver(&motor_height, PIN_INA_1, PIN_INB_1, PIN_SEL0_1, PIN_SEL1_1, PIN_MulEN_1, PIN_PWM_1);
	// MD_ConfigDriver(&motor_recliner, PIN_INA_2, PIN_INB_2, PIN_SEL0_2, PIN_SEL1_2, PIN_MulEN_2, PIN_PWM_2);
	// MD_ConfigDriver(&motor_slider, PIN_INA_3, PIN_INB_3, PIN_SEL0_3, PIN_SEL1_3, PIN_MulEN_3, PIN_PWM_3);

	MD_UpdateStatusMulsen(&motor_height, CURRENT);
	MD_UpdateStatusMulsen(&motor_recliner, CURRENT);
	MD_UpdateStatusMulsen(&motor_slider, CURRENT);
	MD_StopAll();
}

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

_Bool MD_CheckRisingEdge(uint8_t current_value)
{
	static _Bool is_edge_detection_enabled = true;
	static uint8_t counter = 0;

	if (is_edge_detection_enabled == true) {
		if (current_value == 1)
		{
			if (counter++ >= 100)
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

