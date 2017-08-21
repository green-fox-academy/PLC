#ifndef __TIMER_H_
#define __TIMER_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "GPIO.h"

TIM_HandleTypeDef tim4_handle;

void timer4_init(void);


#endif /* __TIMER_H_ */
