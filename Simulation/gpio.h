/*
 * gpio.h
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */

#ifndef DRIVERLAYER_GPIO_GPIO_H_
#define DRIVERLAYER_GPIO_GPIO_H_
// #include "components.h"

/*===========================================================================
 * REUSABLE CODE SECTION
 *===========================================================================*/
typedef struct {
	uint16_t port;
    uint16_t pin;
    uint8_t  value;
} GPIO_TypeDef;

/*===========================================================================
 * PTR CODE SECTION
 *===========================================================================*/
// #define PIN_INA_1		((GPIO_TypeDef){PORT_INA_1, INA_1, 0})
// #define PIN_INB_1		((GPIO_TypeDef){PORT_INB_1, INB_1, 0})
// #define PIN_SEL0_1		((GPIO_TypeDef){PORT_SEL0_1, SEL0_1, 0})
// #define PIN_SEL1_1		((GPIO_TypeDef){PORT_SEL1_1, SEL1_1, 0})
// #define PIN_MulEN_1		((GPIO_TypeDef){PORT_MulEN_1, MulEN_1, 0})

// #define PIN_INA_2		((GPIO_TypeDef){PORT_INA_2, INA_2, 0})
// #define PIN_INB_2		((GPIO_TypeDef){PORT_INB_2, INB_2, 0})
// #define PIN_SEL0_2		((GPIO_TypeDef){PORT_SEL0_2, SEL0_2, 0})
// #define PIN_SEL1_2		((GPIO_TypeDef){PORT_SEL1_2, SEL1_2, 0})
// #define PIN_MulEN_2		((GPIO_TypeDef){PORT_MulEN_2, MulEN_2, 0})

// #define PIN_INA_3		((GPIO_TypeDef){PORT_INA_3, INA_3, 0})
// #define PIN_INB_3		((GPIO_TypeDef){PORT_INB_3, INB_3, 0})
// #define PIN_SEL0_3		((GPIO_TypeDef){PORT_SEL0_3, SEL0_3, 0})
// #define PIN_SEL1_3		((GPIO_TypeDef){PORT_SEL1_3, SEL1_3, 0})
// #define PIN_MulEN_3		((GPIO_TypeDef){PORT_MulEN_3, MulEN_3, 0})

typedef struct {
    GPIO_TypeDef INA;
    GPIO_TypeDef INB;
    GPIO_TypeDef PWM;
    GPIO_TypeDef SEL0;
    GPIO_TypeDef SEL1;
    GPIO_TypeDef MulEN;
} Pin_Output_TypeDef;

/*===========================================================================
 * REUSABLE CODE SECTION
 *===========================================================================*/
void GPIO_SetPin(GPIO_TypeDef *ppin);
void GPIO_ClearPin(GPIO_TypeDef *ppin);
void GPIO_TogglePin(GPIO_TypeDef *ppin);
uint8_t GPIO_ReadPin(GPIO_TypeDef *ppin);

extern Pin_Output_TypeDef thisGPIO;
#endif /* DRIVERLAYER_GPIO_GPIO_H_ */
