/* Includes ------------------------------------------------------------------*/
#include "uart.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

void rx_tx_GPIO_init();
void uart_init();
void buffer_init();;

/* Private functions ---------------------------------------------------------*/
void modbus_error_handler(uint8_t error);

void UART_send(uint8_t *buffer)
{
	// Send buffer content
	HAL_UART_Transmit(&uart_handle, (uint8_t*) buffer, 32, 4);

}


void modbus_init()
{
	buffer_init();
	rx_tx_GPIO_init();
	uart_init();
}

void buffer_init()
{
	for (int i = 0; i < RXBUFFERSIZE; i++) {
		RX_buffer[i] = 0;
	}
}

void uart_init()
{
	uart_handle.Instance 	   	= USARTx;
	uart_handle.Init.BaudRate   = BAUDRATE;
	uart_handle.Init.WordLength	= UART_WORDLENGTH_8B;
	uart_handle.Init.StopBits  	= UART_STOPBITS_1;
	uart_handle.Init.Parity     	= UART_PARITY_NONE;
	uart_handle.Init.HwFlowCtl  	= UART_HWCONTROL_NONE;
	uart_handle.Init.Mode       	= UART_MODE_TX_RX;
	uart_handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	if(HAL_UART_DeInit(&uart_handle) != HAL_OK)
		;
	if(HAL_UART_Init(&uart_handle) != HAL_OK)
		;
		// Error_


	//Setup interrupts for UART
	HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);

	// Start UART receiver in interrupt mode
	HAL_UART_Receive_IT(&uart_handle, RX_buffer, 32);

	interrupt_flag = 0;
}

void rx_tx_GPIO_init()
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks ##*/
	/* Enable GPIO TX/RX clock */
	USARTx_TX_GPIO_CLK_ENABLE();
	USARTx_RX_GPIO_CLK_ENABLE();


	/* Enable USARTx clock */
	USARTx_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##*/
	/* UART TX GPIO pin configuration  */
	GPIO_InitStruct.Pin       = USARTx_TX_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = USARTx_TX_AF;

	HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

	/* UART RX GPIO pin configuration  */
	GPIO_InitStruct.Pin = USARTx_RX_PIN;
	GPIO_InitStruct.Alternate = USARTx_RX_AF;

	HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// This interrupt is fired when we received 20byte data via USART6

	interrupt_flag = 1;
	// Re-enable the interrupt
	HAL_UART_Receive_IT(huart, RX_buffer, 32);
}

void modbus_error_handler(uint8_t error)
{
	switch (error) {

	case 1 :
		;
		break;
	case 2 :
		;
		break;
	case 3 :
		;
		break;
	default :
		;
	}
}
