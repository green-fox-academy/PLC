/** UART in polling mode between L4-RealTerm terminal
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  Gyula
  * @version V1.8.0
  * @date    15-08-2017
  * @brief   UART in polling mode between L4 - RealTerm terminal
  * 		 Base: L4 Cube template file
  *******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef uart_handle;
// Buffer used for reception
uint8_t buff_for_receive[100];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/
#ifdef __GNUC__
// With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
//  set to 'Yes') calls __io_putchar()
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif // __GNUC__


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

	// Init LED and pushbutton
	BSP_LED_Init(LED2);
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

	// Init D0/RX/PA3/USART2_RX and D1/TX/PA2/USART2_TX pins
	GPIO_InitTypeDef GPIO_InitStruct;
	__GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Set up UART
	__USART2_CLK_ENABLE();
	uart_handle.Instance        = USART2;				// UART -> D0 PA3 USART2_RX, D1 PA2 USART2_TX
	uart_handle.Init.BaudRate   = 9600;				//
	uart_handle.Init.WordLength = UART_WORDLENGTH_8B;	//
	uart_handle.Init.StopBits   = UART_STOPBITS_1;	//
	uart_handle.Init.Parity     = UART_PARITY_NONE;	//
	uart_handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;//
	uart_handle.Init.Mode       = UART_MODE_TX_RX;	//
	uart_handle.Init.OverSampling = UART_OVERSAMPLING_16; //
	HAL_UART_Init(&uart_handle);

	/* Loop */
	uint8_t i = 0;
	while (i < 10)
	{
/*	if (BSP_PB_GetState(BUTTON_USER == 1)) {
		BSP_LED_On(LED2);
		char msg[] = "UART test: char text send to terminal\r\n";
		HAL_UART_Transmit(&uart_handle, msg, strlen(msg), 100);
		//HAL_Delay(500);
	} else {
		//if (BSP_PB_GetState(BUTTON_USER == 0)) {
		BSP_LED_Off(LED2);
		HAL_UART_Transmit(&uart_handle, "No msg from board\r\n", 50, 100);
	}
*/
	// Output without printf, using HAL function
	char msg[] = "UART test - char text send to terminal with HAL_UART_Transmit.\r\n";
	HAL_UART_Transmit(&uart_handle, msg, strlen(msg), 100);
	BSP_LED_On(LED2);

	// Receive txt from Terminal
	HAL_UART_Receive(&uart_handle, (uint8_t *)buff_for_receive, 100, 100);

	// Output a message using printf function
	printf("UART Printf Example: retarget the C library printf function to the UART\r\n");
	printf("** Test finished successfully. ** \r\n");
	HAL_Delay(500);
	i++;
	}

	HAL_UART_Transmit(&uart_handle, "Test done\r\n", 12, 100);
	BSP_LED_Off(LED2);
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */

PUTCHAR_PROTOTYPE
{
  // Place your implementation of fputc here
  // e.g. write a character to the EVAL_COM1 and Loop until the end of transmission
  HAL_UART_Transmit(&uart_handle, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
