/*
 * gpio.h
 *
 *  Created on: Jul 21, 2025
 *      Author: ADMIN
 */

#ifndef DRIVERLAYER_GPIO_GPIO_H_
#define DRIVERLAYER_GPIO_GPIO_H_
#include "components.h"

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
/* Motor 1 Pins */
#define PIN_INA_1       ((GPIO_TypeDef){PORT_INA_1, INA_1, 0})
#define PIN_INB_1       ((GPIO_TypeDef){PORT_INB_1, INB_1, 0})
#define PIN_SEL0_1      ((GPIO_TypeDef){PORT_SEL0_1, SEL0_1, 0})
#define PIN_SEL1_1      ((GPIO_TypeDef){PORT_SEL1_1, SEL1_1, 0})
#define PIN_MulEN_1     ((GPIO_TypeDef){PORT_MulEN_1, MulEN_1, 0})
#define PIN_PWM_1       ((GPIO_TypeDef){PORT_PWM_1, PWM_1, 0})
#define PIN_HALL_1      ((GPIO_TypeDef){PORT_HALL_1, HALL_1, 0})

/* Motor 2 Pins */
#define PIN_INA_2       ((GPIO_TypeDef){PORT_INA_2, INA_2, 0})
#define PIN_INB_2       ((GPIO_TypeDef){PORT_INB_2, INB_2, 0})
#define PIN_SEL0_2      ((GPIO_TypeDef){PORT_SEL0_2, SEL0_2, 0})
#define PIN_SEL1_2      ((GPIO_TypeDef){PORT_SEL1_2, SEL1_2, 0})
#define PIN_MulEN_2     ((GPIO_TypeDef){PORT_MulEN_2, MulEN_2, 0})
#define PIN_PWM_2       ((GPIO_TypeDef){PORT_PWM_2, PWM_2, 0})
#define PIN_HALL_2      ((GPIO_TypeDef){PORT_HALL_2, HALL_2, 0})

/* Motor 3 Pins */
#define PIN_INA_3       ((GPIO_TypeDef){PORT_INA_3, INA_3, 0})
#define PIN_INB_3       ((GPIO_TypeDef){PORT_INB_3, INB_3, 0})
#define PIN_SEL0_3      ((GPIO_TypeDef){PORT_SEL0_3, SEL0_3, 0})
#define PIN_SEL1_3      ((GPIO_TypeDef){PORT_SEL1_3, SEL1_3, 0})
#define PIN_MulEN_3     ((GPIO_TypeDef){PORT_MulEN_3, MulEN_3, 0})
#define PIN_PWM_3       ((GPIO_TypeDef){PORT_PWM_3, PWM_3, 0})
#define PIN_HALL_3      ((GPIO_TypeDef){PORT_HALL_3, HALL_3, 0})

/* Function Buttons */
#define PIN_HEIGHT_1    ((GPIO_TypeDef){PORT_HIEGHT_1, HIEGHT_1, 0})
#define PIN_HEIGHT_2    ((GPIO_TypeDef){PORT_HIEGHT_2, HIEGHT_2, 0})
#define PIN_RECLINER_1  ((GPIO_TypeDef){PORT_RECLIER_1, RECLIER_1, 0})
#define PIN_RECLINER_2  ((GPIO_TypeDef){PORT_RECLIER_2, RECLIER_2, 0})
#define PIN_SLIDER_1    ((GPIO_TypeDef){PORT_SLIDER_1, SLIDER_1, 0})
#define PIN_SLIDER_2    ((GPIO_TypeDef){PORT_SLIDER_2, SLIDER_2, 0})
#define PIN_RECALL_1    ((GPIO_TypeDef){PORT_RECALL_1, RECALL_1, 0})
#define PIN_RECALL_2    ((GPIO_TypeDef){PORT_RECALL_2, RECALL_2, 0})
#define PIN_RECALL_3    ((GPIO_TypeDef){PORT_RECALL_3, RECALL_3, 0})

#define PIN_SAVE        ((GPIO_TypeDef){PORT_SAVE, SAVE, 0})

typedef struct{
    GPIO_TypeDef recliner_1;
    GPIO_TypeDef recliner_2;
    GPIO_TypeDef height_1;
    GPIO_TypeDef height_2;
    GPIO_TypeDef slider_1;
    GPIO_TypeDef slider_2;
    GPIO_TypeDef save;
    GPIO_TypeDef recall_1;
    GPIO_TypeDef recall_2;
    GPIO_TypeDef recall_3;

    GPIO_TypeDef hall_1;
    GPIO_TypeDef hall_2;
    GPIO_TypeDef hall_3;
} GPIO_Input_TypeDef;

/*===========================================================================
 * REUSABLE CODE SECTION
 *===========================================================================*/
void GPIO_SetPin(GPIO_TypeDef *pPin);
void GPIO_ClearPin(GPIO_TypeDef *pPin);
void GPIO_TogglePin(GPIO_TypeDef *pPin);
uint8_t GPIO_ReadPin(GPIO_TypeDef *pPin);

/*===========================================================================
 * PTR CODE SECTION
 *===========================================================================*/
void GPIO_Init(void);
void GPIO_Update(void);

extern GPIO_Input_TypeDef thisGPIO;
#endif /* DRIVERLAYER_GPIO_GPIO_H_ */
