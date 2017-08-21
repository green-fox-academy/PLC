/* Includes ------------------------------------------------------------------*/
#include "timer.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void timer4_init(void)
{
	tim4_handle.Instance = TIM4;
	tim4_handle.Init.AutoReloadPreload = DISABLE;
	tim4_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim4_handle.Init.Period =  0xFFFF;
	tim4_handle.Init.Prescaler = 100;
	tim4_handle.Init.RepetitionCounter = 0;
	tim4_handle.State = HAL_TIM_STATE_RESET;
	__HAL_RCC_TIM4_CLK_ENABLE();
	HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
	HAL_TIM_Base_Init(&tim4_handle);
	HAL_TIM_Base_Start_IT(&tim4_handle);
}

/*
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{

}
*/

/*
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	gpio_toggle_digital_pin(2);

}
*/
