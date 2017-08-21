/** UART in polling mode between L4-RealTerm terminal
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  Gyula
  * @version V1.8.0
  * @date    17-08-2017
  * @brief   UART in polling mode between L4 - RealTerm terminal
  * 		 Base: L4 Cube template file
  *******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// UART init structure
UART_HandleTypeDef uart_handle;
// Buffer used for reception
// uint8_t buff_for_receive[100];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Configure and init UART
  * @param  None
  * @retval None
  */
void uart_init(void)
{
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
	__USART2_CLK_ENABLE();								// Enable UART clock
	uart_handle.Instance        = USART2;				// UART -> D0 PA3 USART2_RX, D1 PA2 USART2_TX
	uart_handle.Init.BaudRate   = 9600;					// configures the UART communication baud rate
	uart_handle.Init.WordLength = UART_WORDLENGTH_8B;	// Specifies the number of data bits transmitted or received in a frame.
	uart_handle.Init.StopBits   = UART_STOPBITS_1;		// Specifies the number of stop bits transmitted.
	uart_handle.Init.Parity     = UART_PARITY_NONE;		// Specifies the parity mode
//	uart_handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;	// Specifies whether the hardware flow control mode is enabled or disabled.
	uart_handle.Init.Mode       = UART_MODE_TX_RX;		// Specifies whether the Receive or Transmit mode is enabled or disabled.
//	uart_handle.Init.OverSampling = UART_OVERSAMPLING_16; // Specifies whether the Over sampling 8 is enabled or disabled.
	HAL_UART_Init(&uart_handle);						// init UART
}

/**
  * @brief  Send message function
  * @param
  * @retval None
  */
void uart_send(char* buffer)
{
/*	char msg[] = "UART test - char text send to terminal with HAL_UART_Transmit.\r\n";
	HAL_UART_Transmit(&uart_handle, msg, strlen(msg), 100);
	BSP_LED_On(LED2);
*/
	uint16_t bufflength = strlen(buffer);
	uint32_t timeout = 500;

	// Send buffer content
	HAL_UART_Transmit(&uart_handle, (uint8_t*)buffer, bufflength, timeout);
	HAL_Delay(10);

	// Send new line
	//HAL_UART_Transmit(&uart_handle, (uint8_t*) "\r\n", 2, timeout);
}

void uart_receive(void)
{
	char* buffer;
	uint16_t bufflength = strlen(buffer);
	uint32_t timeout = 500;
	HAL_UART_Receive(&uart_handle, (uint8_t *)buffer, bufflength, timeout);
	//BSP_LED_Toggle(LED2);
	HAL_Delay(10);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
