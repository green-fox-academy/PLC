/** TOTORO PLC Project - Slave 3 - Analog In
  ******************************************************************************
  * @file    PLC\Slaves\Projects\Slave_3_An_in\Src\main.c
  * @author  Gyula Rasztovich
  * @version V1.0
  * @date    08-08-2017
  * @brief   Main program body for Slave Analog In board.
  * 		 Base: L4 cube empty template file
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
void led_init(void);
void led_on(void);
void led_off(void);
void adc_init(void);
uint32_t adc_measure();


static void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
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
	// Init ADC
	adc_init();

	// Init led
	led_init();

	// Init user button - mid-term plan
	// BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

	/* Forever loop: changing the potentiometer's if adc measurement is less than the half of the possible
	 * maximal value, the board's LED2 and the attached red led turns on.
	 * For higher value leds are turning off.
	 */
	while (1) {
		if (adc_measure() < 4095/2 ) {
			BSP_LED_On(LED2);
			led_on();
		} else {
			BSP_LED_Off(LED2);
			led_off();
		}
	}
}

void led_init(void)
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
}

void led_on(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
}

void led_off(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
}

/**
  * @brief  ADC1 initialization and configuration for A2 pin, 12 bit resolution
  * 		PIN A2 -> PA4 -> ADC12_IN9, GPIO_PIN: 4, GPIOA, Channel 9
  * @param  None
  * @retval None
  */
void adc_init(void)
{
	// Enable ADC and GPIOA clock
	__HAL_RCC_ADC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// Enable A2 analog pin
	GPIO_InitTypeDef adc_init_struct;
	adc_init_struct.Mode 	= GPIO_MODE_ANALOG_ADC_CONTROL;
	adc_init_struct.Pin 	= GPIO_PIN_4;
	adc_init_struct.Pull 	= GPIO_NOPULL;
	adc_init_struct.Speed 	= GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOA, &adc_init_struct);

	// Set up ADC1
	adc_handle.Instance 			= ADC1;							// Register base address
	adc_handle.Init.ClockPrescaler	= ADC_CLOCK_SYNC_PCLK_DIV2;		// Select ADC clock source
	adc_handle.Init.Resolution 		= ADC_RESOLUTION_12B;			// Resolution: measurement values will between 0...4095 ((2^12)-1)
	adc_handle.Init.DataAlign 		= ADC_DATAALIGN_RIGHT;			// Specify ADC data alignment in conversion data register (right or left).
	/* Other optional settings
		adc_handle.State= HAL_ADC_STATE_RESET;					// ADC communication state (bit-map of ADC states)
		adc_handle.Init.ScanConvMode= ADC_SCAN_DISABLE;			// Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1)
		adc_handle.Init.EOCSelection= ADC_EOC_SEQ_CONV;			// Specify which EOC (End Of Conversion) flag is used for conversion by polling and interruption
		adc_handle.Init.LowPowerAutoWait= DISABLE;				// Select the dynamic low power Auto Delay
		adc_handle.Init.ContinuousConvMode= DISABLE;			// Continuous mode disabled to have only 1 conversion at each conversion trig
		adc_handle.Init.NbrOfConversion= 1;						// Parameter discarded because sequencer is disabled
		adc_handle.Init.DiscontinuousConvMode= DISABLE;			// Parameter discarded because sequencer is disabled
		adc_handle.Init.NbrOfDiscConversion= DISABLE;			// Parameter discarded because sequencer is disabled
		adc_handle.Init.ExternalTrigConv= ADC_SOFTWARE_START;	// Software start to trig the 1st conversion manually, without external event
		adc_handle.Init.DMAContinuousRequests= DISABLE;			// ADC with DMA transfer: continuous requests to DMA disabled (default state) since DMA is not used in this example.
		adc_handle.Init.Overrun= ADC_OVR_DATA_OVERWRITTEN;		// Select the behavior in case of overrun: data overwritten or preserved
	*/

	// Init ADC
	HAL_ADC_Init(&adc_handle);

	// Set up ADC channel
	ADC_ChannelConfTypeDef adc_channel;
	adc_channel.Channel 		= ADC_CHANNEL_9;				// Specify the channel to configure into ADC regular group, according to User manual's Table 23. Arduino connectors on NUCLEO-L476RG
	adc_channel.Rank 			= ADC_REGULAR_RANK_1;			// Specify the rank in the regular group sequencer.
	adc_channel.SamplingTime 	= ADC_SAMPLETIME_24CYCLES_5;	// Sampling time value to be set for the selected channel.
	adc_channel.Offset 			= 0;							// Define the offset to be subtracted from the raw converted data.
	/* Other optional channel settings
		adc_channel.SingleDiff   	= ADC_SINGLE_ENDED;			// Select single-ended or differential input.
		adc_channel.OffsetNumber 	= ADC_OFFSET_1;				// Select the offset number
	 */
	HAL_ADC_ConfigChannel(&adc_handle, &adc_channel);

	/* Error handling - middle-range plan, does not work at the moment
	if (HAL_ADC_Init(&adc_handle) != HAL_OK)
	{
		ADC initialization error
		Error_Handler();
	}
	*/
}

/**
  * @brief  ADC measurement program.
  * @param  None
  * @retval measurement value between 0...4095 (note: adc is initialized for 12 bit resolution)
  */
uint32_t adc_measure()
{
	//uint32_t measurement = 0;
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	//measurement = HAL_ADC_GetValue(&adc_handle);
	//return measurement;
	//HAL_ADC_Stop(&adc_handle);
	return HAL_ADC_GetValue(&adc_handle);
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

/** personal notes - for mid-term developing
csak set_duty-t meghív.
pwm-hez: uint32_t adc_measurement_value = adc_measure();


//void pwm_init();


void pwm_set_duty(adc_measurement_value)
{
	uint32_t pulse = pwm_handle.Init.Period * (adc_measurement_value / 4095);
	pwm_oc_init.Pulse = pulse;
	HAL_TIM_PWM_ConfigChannel(&pwm_handle, &pwm_oc_init, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&pwm_handle, TIM_CHANNEL_1);
}

 pwm outoutra kell kötni a ledet. note: pin beállítás: hal_msp.c-ben!
*/


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
