/** TOTORO PLC Project - Slave 3 - Analog In
  ******************************************************************************
  * @file    PLC\Slaves\Projects\Slave_3_An_in\Src\main.c
  * @author  Gyula Rasztovich
  * @version V1.0
  * @date    08-08-2017
  * @brief   Main program body
  ******************************************************************************
  * @attention
  * Slave Analog In
  * base: L4 cube template file
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

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
ADC_HandleTypeDef adc_handle;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */

void led_on()
{
	// LED on Digital 2 output!
	// PIN: D2 - PA10 --> GPIOA, PIN10
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef led_init;
	led_init.Mode 	= GPIO_MODE_OUTPUT_PP;
	led_init.Pin 	= GPIO_PIN_10;
	led_init.Pull 	= GPIO_NOPULL;
	led_init.Speed 	= GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOA, &led_init);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
}

void adc_init()
{
	// ADC init test, only for A1 analog pin
	// PIN: A1 - ADC12_IN6 --> GPIOA, PIN1, Channel 6, ADC1, 12 bit
	__HAL_RCC_ADC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef adc_init_struct;
	adc_init_struct.Mode 	= GPIO_MODE_ANALOG;
	adc_init_struct.Pin 	= GPIO_PIN_1;
	adc_init_struct.Pull 	= GPIO_NOPULL;
	adc_init_struct.Speed 	= GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOA, &adc_init_struct);


	adc_handle.Instance 			= ADC1;
	adc_handle.Init.ClockPrescaler 	= ADC_CLOCK_SYNC_PCLK_DIV2;
	adc_handle.Init.Resolution 		= ADC_RESOLUTION_12B;
	adc_handle.Init.DataAlign 		= ADC_DATAALIGN_RIGHT;
	HAL_ADC_Init(&adc_handle);

	ADC_ChannelConfTypeDef adc_channel;
	adc_channel.Channel 		= ADC_CHANNEL_6;
	adc_channel.Offset 			= 0;
	adc_channel.Rank 			= 1;
	adc_channel.SamplingTime 	= ADC_SAMPLETIME_640CYCLES_5;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_channel);

/*	if (HAL_ADC_Init(&adc_handle) != HAL_OK)
	{
	// ADC initialization error
	Error_Handler();
	}
	*/
}

uint32_t adc_measure()
{
	uint32_t measurement = 0;
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	measurement = HAL_ADC_GetValue(&adc_handle);
	return measurement;
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

  /* Initialize LED on board */
  BSP_LED_Init(LED2);

  /* Add your application code here */
  adc_init();
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  while (1) {
  	BSP_LED_On(LED2);
  	HAL_Delay(adc_measure());
  	//led_on();
  	BSP_LED_Off(LED2);
  	HAL_Delay(adc_measure());
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

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */


 /* static void Error_Handler(void)
{
  // User may add here some code to deal with a potential error

  // In case of error, LED2 is toggling at a frequency of 1Hz
  while(1)
  {
    // Toggle LED2
    BSP_LED_Toggle(LED2);
    HAL_Delay(500);
  }
}
*/

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
