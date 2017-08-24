#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDED

#include "stm32l4xx_hal.h"


void pwm_init(uint8_t pin_index);
void pwm_set_duty(float duty, uint8_t pin_index);


/* ########## Functions for Digital pins ########## */

/* Function name: GPIO_Set_PIN
 * Function purpose: Set PIN to output or input
 * Function input - GPIO_Pin_t *GPIO_Pin: This is the pin to be set
 * Function input - uint32_t pode: This is the mode it can be these:
 * 		GPIO_MODE_INPUT:		Input Floating Mode
 * 		GPIO_MODE_OUTPUT_PP: 	Output Push Pull Mode
 * 		GPIO_MODE_OUTPUT_OD: 	Output Open Drain Mode
 * 		GPIO_MODE_AF_PP:		Alternate Function Push Pull Mode
 *		GPIO_MODE_AF_OD:		Alternate Function Open Drain Mode
 * Function input - uint32_t pull can be:
 * 		GPIO_PULLDOWN
 * 		GPIO_PULLUP
 * 		GPIO_NOPULL
 *
 */
void pwm_pin_init(uint8_t pin_index, uint32_t mode, uint32_t pull);

void init_pwms();



#endif // PWM_H_INCLUDED
