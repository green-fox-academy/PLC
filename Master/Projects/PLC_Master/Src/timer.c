/* Includes ------------------------------------------------------------------*/
#include "timer.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void timer3_init(void)
{

	time_out_flag = 0;

	  /* Set TIMx instance */
	tim3_handle.Instance = TIM3;

	tim3_handle.Init.Period            = PERIOD;
	tim3_handle.Init.Prescaler         = PRESCALE;
	tim3_handle.Init.ClockDivision     = 0;
	tim3_handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	tim3_handle.Init.RepetitionCounter = 0;
	tim3_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	  HAL_TIM_Base_Init(&tim3_handle);

	  /*##-2- Start the TIM Base generation in interrupt mode ####################*/
	  /* Start Channel1 */
	  //HAL_TIM_Base_Start_IT(&tim3_handle);

}

// THe Interrupt handler is in stm32f7xx_hal_timiebase_tim.c

