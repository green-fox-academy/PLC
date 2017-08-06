/* Includes ------------------------------------------------------------------*/
#include "modbus.h"
#include "stm32l4xx_hal_uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* UART handler declaration */
UART_HandleTypeDef UartHandle;

/* Buffers for Send, and Receive messages */
 uint8_t num_to_send = 1;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/



void modbus_sender_measure_test()
{
	uint8_t transmit = 0;
	uint16_t counter_ok = 0;
	uint16_t counter_nope = 0;

	for (uint16_t i = 0; i < 1000; i++) {

		transmit = HAL_UART_Transmit (&UartHandle, (uint8_t*)aTxBuffer, 11, 2);

		if (transmit != HAL_OK) {
			counter_nope++;
		} else {
			counter_ok++;
		}
	}
}

void modbus_receive_measure_test()
{
	uint16_t counter_ok = 0;
	uint16_t counter_nope = 0;
	uint8_t receive = 0;

	while (1) {

		receive = HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 2);

		if (receive == HAL_OK) {
			counter_ok++;
		} else if (counter_ok != 0) {
			counter_nope++;
		}

		if (counter_ok + counter_nope == 1000) {
			counter_nope = 0;
			counter_ok = 0;
		}

	}

}


int modbus_send_command(uint8_t slave_address)
{
	uint8_t receive;
	uint8_t transmit;

	transmit = HAL_UART_Transmit (&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE, 2);

	if (transmit != HAL_OK) {
		modbus_error_handler(transmit);
		return -1;

	} else {

		receive = HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 2);

		if (receive != HAL_OK) {
			modbus_error_handler(receive);
			return -1;
		} else {
		}
	}

	return 0;
}

uint8_t modbus_send_message(uint8_t *msg, uint8_t msg_len)
{
	uint8_t msg_size = sizeof(msg[0]) * msg_len;

	uint8_t transmit = HAL_UART_Transmit (&UartHandle, msg, msg_size, 2);

	if (transmit == HAL_OK) {
		return 1;
	}

	return 0;
}

void modbus_listen()
{
	uint8_t receive;
	uint8_t transmit;
	aTxBuffer[0] = 0b10100110;

	while (1) {

		receive = HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 2);
		if (receive != HAL_OK) {
			;
		} else {
			transmit = HAL_UART_Transmit (&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE, 2);

			if (transmit != HAL_OK) {
				modbus_error_handler(transmit);
			}
		}
	}
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


void modbus_init(void)
{
	rx_tx_GPIO_init();
	uart_init();
}

void uart_init(void)
{
	UartHandle.Instance 	   	= USARTx;
	UartHandle.Init.BaudRate   	= 115200;
	UartHandle.Init.WordLength	= UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits  	= UART_STOPBITS_1;
	UartHandle.Init.Parity     	= UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl  	= UART_HWCONTROL_NONE;
	UartHandle.Init.Mode       	= UART_MODE_TX_RX;
	UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
		;
	if(HAL_UART_Init(&UartHandle) != HAL_OK)
		;
		// Error_Handler();
}

void rx_tx_GPIO_init(void)
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


