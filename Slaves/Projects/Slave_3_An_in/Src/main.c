/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ain_slave_loop.h"

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup UART_TwoBoards_ComPolling
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* UART handler declaration */
UART_HandleTypeDef UartHandle;
ADC_HandleTypeDef adc_handle;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);
void system_init();
uint8_t slave_address_set();

void adc_init(void);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	system_init();

	slave_address = 11;

	ain_slave_loop_thread();

}

void system_init()
{
	/* STM32L4xx HAL library initialization:
	   - Configure the Flash prefetch
	   - Systick timer is configured by default as source of time base, but user
		 can eventually implement his proper time base source (a general purpose
		 timer for example or other time source), keeping in mind that Time base
		 duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
		 handled in milliseconds basis.
	   - Set NVIC Group Priority to 4
	   - Low Level Initialization
	 */
	HAL_Init();

	/* Configure the system clock to 80 MHz */
	SystemClock_Config();

	/* Configure LED2 */
	BSP_LED_Init(LED2);
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

	/* Init Uart and modbus protocol DPIN0 : RX and DPIN1 : TX */
	modbus_init();


	/* Init Analog pins from A0 to A6 */
	for (int i = 0; i < 6; i++) {
		gpio_init_analoge_pin(i);
	}

	/* Init Pins from DPIN2 to DPIN6 as an Digital inputs for Slave_address */
	for (int i = 2; i < 7; i++) {
		gpio_init_digital_pin(i, GPIO_MODE_INPUT, GPIO_PULLDOWN);
	}

	//slave_address = slave_address_set();

	adc_init();

}


/* Function name: 		slave_address_set
 * Function purpose:
 *
 */
uint8_t slave_address_set()
{
	uint8_t slave_adr = 0;

	for (uint8_t i = 0; i < 5; i++) {
		slave_adr += (gpio_read_digital_pin(i+2) << i);
	}

	return slave_adr;
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

	// Init ADC
	HAL_ADC_Init(&adc_handle);

	// Set up ADC channel
	ADC_ChannelConfTypeDef adc_channel;
	adc_channel.Channel 		= ADC_CHANNEL_9;				// Specify the channel to configure into ADC regular group, according to User manual's Table 23. Arduino connectors on NUCLEO-L476RG
	adc_channel.Rank 			= ADC_REGULAR_RANK_1;			// Specify the rank in the regular group sequencer.
	adc_channel.SamplingTime 	= ADC_SAMPLETIME_24CYCLES_5;	// Sampling time value to be set for the selected channel.
	adc_channel.Offset 			= 0;							// Define the offset to be subtracted from the raw converted data.

	HAL_ADC_ConfigChannel(&adc_handle, &adc_channel);
}

/**
  * @brief  ADC measurement program.
  * @param  None
  * @retval measurement value between 0...4095 (note: adc is initialized for 12 bit resolution)
  */
uint16_t adc_measure()
{
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return (uint16_t)HAL_ADC_GetValue(&adc_handle);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
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
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

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
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
    Error_Handler();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED2 on */
  BSP_LED_On(LED2);
  while(1)
  {
    /* Error if LED2 is slowly blinking (1 sec. period) */
    BSP_LED_Toggle(LED2);
    HAL_Delay(1000);
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
