#ifndef __TIMER_H_
#define __TIMER_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "GPIO.h"

#define jj           		TIM3
#define TIMx_CLK_ENABLE()	__HAL_RCC_TIM3_CLK_ENABLE()
#define TIMx_IRQn           TIM3_IRQn
#define TIMx_IRQHandler     TIM3_IRQHandler
#define PRESCALE 			500
#define PERIOD				10000

TIM_HandleTypeDef tim3_handle;

volatile uint8_t time_out_flag;

void timer3_init(void);


#endif /* __TIMER_H_ */
