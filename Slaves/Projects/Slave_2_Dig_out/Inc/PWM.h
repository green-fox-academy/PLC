#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDED

#include "stm32l4xx_hal.h"

TIM_HandleTypeDef pwm_handle;
TIM_OC_InitTypeDef pwm_oc_init;

void pwm_init();
void pwm_set_duty(float duty);


#endif // PWM_H_INCLUDED
