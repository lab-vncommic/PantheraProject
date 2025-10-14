/*
 * gpio_drv_handle.c
 *
 *  Created on: Sep 30, 2025
 *      Author: Tracy McConnell
 */
#include "gpio_drv_handle.h"
gpio_obj_t gpio_inst[NUMBER_OF_GPIO];

void gpio_config_pin(gpio_obj_t *p_gpio_obj, gpio_id_e id, uint16_t port, uint16_t pin);

void gpio_init(void)
{
	for (int i = 0; i < NUMBER_OF_GPIO; i++) {
		gpio_create_obj(&gpio_inst[i]);
	}

	/* Configure Button Pins */
	gpio_config_pin(&gpio_inst[0], GPIO_RECLINER_1, PORT_RECLIER_1, RECLIER_1);
	gpio_config_pin(&gpio_inst[1], GPIO_RECLINER_2, PORT_RECLIER_2, RECLIER_2);
	gpio_config_pin(&gpio_inst[2], GPIO_SLIDER_1, PORT_SLIDER_1, SLIDER_1);
	gpio_config_pin(&gpio_inst[3], GPIO_SLIDER_2, PORT_SLIDER_2, SLIDER_2);
	gpio_config_pin(&gpio_inst[4], GPIO_HEIGHT_1, PORT_HIEGHT_1, HIEGHT_1);
	gpio_config_pin(&gpio_inst[5], GPIO_HEIGHT_2, PORT_HIEGHT_2, HIEGHT_2);
	gpio_config_pin(&gpio_inst[6], GPIO_SAVE, PORT_SAVE, SAVE);
	gpio_config_pin(&gpio_inst[7], GPIO_RECALL_1, PORT_RECALL_1, RECALL_1);
	gpio_config_pin(&gpio_inst[8], GPIO_RECALL_2, PORT_RECALL_2, RECALL_2);
	gpio_config_pin(&gpio_inst[9], GPIO_RECALL_3, PORT_RECALL_3, RECALL_3);

	/* Configure Motor 1 Pins */
	gpio_config_pin(&gpio_inst[10], GPIO_INA_1, PORT_INA_1, INA_1);
	gpio_config_pin(&gpio_inst[11], GPIO_INB_1, PORT_INB_1, INB_1);
	gpio_config_pin(&gpio_inst[12], GPIO_SEL0_1, PORT_SEL0_1, SEL0_1);
	gpio_config_pin(&gpio_inst[13], GPIO_SEL1_1, PORT_SEL1_1, SEL1_1);
	gpio_config_pin(&gpio_inst[14], GPIO_MulEN_1, PORT_MulEN_1, MulEN_1);
	gpio_config_pin(&gpio_inst[15], GPIO_PWM_1, PORT_PWM_1, PWM_1);
	gpio_config_pin(&gpio_inst[16], GPIO_HALL_1, PORT_HALL_1, HALL_1);
	
	/* Configure Motor 2 Pins */
	gpio_config_pin(&gpio_inst[17], GPIO_INA_2, PORT_INA_2, INA_2);
	gpio_config_pin(&gpio_inst[18], GPIO_INB_2, PORT_INB_2, INB_2);
	gpio_config_pin(&gpio_inst[19], GPIO_SEL0_2, PORT_SEL0_2, SEL0_2);
	gpio_config_pin(&gpio_inst[20], GPIO_SEL1_2, PORT_SEL1_2, SEL1_2);
	gpio_config_pin(&gpio_inst[21], GPIO_MulEN_2, PORT_MulEN_2, MulEN_2);
	gpio_config_pin(&gpio_inst[22], GPIO_PWM_2, PORT_PWM_2, PWM_2);
	gpio_config_pin(&gpio_inst[23], GPIO_HALL_2, PORT_HALL_2, HALL_2);
	
	/* Configure Motor 3 Pins */
	gpio_config_pin(&gpio_inst[24], GPIO_INA_3, PORT_INA_3, INA_3);
	gpio_config_pin(&gpio_inst[25], GPIO_INB_3, PORT_INB_3, INB_3);
	gpio_config_pin(&gpio_inst[26], GPIO_SEL0_3, PORT_SEL0_3, SEL0_3);
	gpio_config_pin(&gpio_inst[27], GPIO_SEL1_3, PORT_SEL1_3, SEL1_3);
	gpio_config_pin(&gpio_inst[28], GPIO_MulEN_3, PORT_MulEN_3, MulEN_3);
	gpio_config_pin(&gpio_inst[29], GPIO_PWM_3, PORT_PWM_3, PWM_3);
	gpio_config_pin(&gpio_inst[30], GPIO_HALL_3, PORT_HALL_3, HALL_3);
}

void gpio_update_status_button_pin(gpio_btn_value_t *p_button)
{
	p_button->recliner_1 = gpio_read_pin(GPIO_RECLINER_1);
	p_button->recliner_2 = gpio_read_pin(GPIO_RECLINER_2);
	p_button->slider_1   = gpio_read_pin(GPIO_SLIDER_1);
	p_button->slider_2   = gpio_read_pin(GPIO_SLIDER_2);
	p_button->height_1   = gpio_read_pin(GPIO_HEIGHT_1);
	p_button->height_2   = gpio_read_pin(GPIO_HEIGHT_2);
	p_button->save       = gpio_read_pin(GPIO_SAVE);
	p_button->recall_1   = gpio_read_pin(GPIO_RECALL_1);
	p_button->recall_2   = gpio_read_pin(GPIO_RECALL_2);
	p_button->recall_3   = gpio_read_pin(GPIO_RECALL_3);
}

void gpio_config_pin(gpio_obj_t *p_gpio_obj, gpio_id_e id, uint16_t port, uint16_t pin)
{
	p_gpio_obj->ctx.id			 = id;
	p_gpio_obj->ctx.hw_cfg.port  = port;
	p_gpio_obj->ctx.hw_cfg.pin   = pin;
}

void gpio_set_pin(gpio_id_e id)
{
	gpio_inst[id].method->set(&gpio_inst[id].ctx);
}

void gpio_clear_pin(gpio_id_e id)
{
	gpio_inst[id].method->clear(&gpio_inst[id].ctx);
}

uint8_t gpio_read_pin(gpio_id_e id)
{
	return gpio_inst[id].method->read(&gpio_inst[id].ctx);
}

void gpio_toggle_pin(gpio_id_e id)
{
	gpio_inst[id].method->toggle(&gpio_inst[id].ctx);
}

