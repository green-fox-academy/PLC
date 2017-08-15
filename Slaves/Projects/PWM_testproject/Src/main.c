/**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    21-April-2017
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef pwm_handle;
TIM_OC_InitTypeDef pwm_oc_init;
TIM_MasterConfigTypeDef sMasterConfig;
TIM_Base_InitTypeDef teszt;



/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/

/* Private inits ---------------------------------------------------------*/


void PWM_pin_init()
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__TIM3_CLK_ENABLE();

	GPIO_InitTypeDef gpio_c;
	gpio_c.Pin = GPIO_PIN_7;
	gpio_c.Mode = GPIO_MODE_AF_PP;
	//gpio_c.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_c.Pull = GPIO_NOPULL;
	gpio_c.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_c.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOC, &gpio_c);
}

/*void teszt_init()
{
	teszt.Prescaler = 0;
	teszt.CounterMode = TIM_COUNTERMODE_UP;
	teszt.Period = 1300;
}
*/
void pwm_init()
{
	// TIM3_CH2 init as PWM
	// D9 - PC7
	pwm_handle.Instance = TIM3;
	//pwm_handle.State = HAL_TIM_STATE_RESET;
	//pwm_handle.Channel = HAL_TIM_ACTIVE_CHANNEL_2;
	pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	pwm_handle.Init.Period = 0xFFFF;
	pwm_handle.Init.Prescaler = 0;
	HAL_TIM_PWM_Init(&pwm_handle);

	//sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	//sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	//HAL_TIMEx_MasterConfigSynchronization(&pwm_handle, &sMasterConfig);

	pwm_oc_init.OCFastMode = TIM_OCFAST_ENABLE;
	pwm_oc_init.OCIdleState = TIM_OCIDLESTATE_SET;
	pwm_oc_init.OCMode = TIM_OCMODE_PWM1;
	pwm_oc_init.OCPolarity = TIM_OCPOLARITY_HIGH;
	pwm_oc_init.Pulse = 0xFFFF;
	HAL_TIM_PWM_ConfigChannel(&pwm_handle, &pwm_oc_init, TIM_CHANNEL_2);

	//HAL_TIM_PWM_Start(&pwm_handle, TIM_CHANNEL_2);
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */

void pwm_set_duty(float duty)
{
	uint32_t pulse = pwm_handle.Init.Period * (duty / 100.0);
	pwm_oc_init.Pulse = pulse;
	HAL_TIM_PWM_ConfigChannel(&pwm_handle, &pwm_oc_init, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&pwm_handle, TIM_CHANNEL_2);
}

int main(void)
{

  /* STM32L4xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user 
             can eventually implement his proper time base source (a general purpose 
             timer for example or other time source), keeping in mind that Time base 
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
             handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the System clock to have a frequency of 80 MHz */
  SystemClock_Config();



  /* Add your application code here
     */
  //teszt_init();
  pwm_init();
  PWM_pin_init();

  /* Infinite loop */
  while (1)
  {
	  for (int i = 0; i <=100; i++) {
		  pwm_set_duty(i);
		  HAL_Delay(30);
	  }
	  for (int i = 100; i >= 0; i--) {
		  pwm_set_duty(i);
		  HAL_Delay(30);
	  	  }

	 //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
