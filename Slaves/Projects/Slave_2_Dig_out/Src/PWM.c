#include "PWM.h"

void pwm_init()
{
	// TIM3_CH2 init as PWM
	// D9  -> PC7
	__HAL_RCC_TIM3_CLK_ENABLE();		//clock enable

	pwm_handle.Instance = TIM3;
	pwm_handle.State = HAL_TIM_STATE_RESET;
	pwm_handle.Channel = HAL_TIM_ACTIVE_CHANNEL_2;
	pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	pwm_handle.Init.Period = 0xFFFF;
	pwm_handle.Init.Prescaler = 0;
	HAL_TIM_PWM_Init(&pwm_handle);

	pwm_oc_init.OCFastMode = TIM_OCFAST_DISABLE;
	pwm_oc_init.OCIdleState = TIM_OCIDLESTATE_RESET;
	pwm_oc_init.OCMode = TIM_OCMODE_PWM1;
	pwm_oc_init.OCPolarity = TIM_OCPOLARITY_HIGH;
	pwm_oc_init.Pulse = 0xFFFF;
	HAL_TIM_PWM_ConfigChannel(&pwm_handle, &pwm_oc_init, TIM_CHANNEL_2);
}

void pwm_set_duty(float duty)
{
	uint32_t pulse = pwm_handle.Init.Period * (duty / 100.0);
	pwm_oc_init.Pulse = pulse;
	HAL_TIM_PWM_ConfigChannel(&pwm_handle, &pwm_oc_init, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&pwm_handle, TIM_CHANNEL_2);
}
