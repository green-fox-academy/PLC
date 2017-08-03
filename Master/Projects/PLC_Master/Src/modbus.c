/* Includes ------------------------------------------------------------------*/
#include "modbus.h"
#include "lcd_log.h"
#include "stm32f7xx_hal_uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* UART handler declaration */
UART_HandleTypeDef UartHandle;

/* Buffers for Send, and Receive messages */
 uint8_t aTxBuffer[] = "PLC MSG TEST";
 uint8_t aRxBuffer[RXBUFFERSIZE];

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

int modbus_send_command(uint8_t slave_address)
{
	if (HAL_UART_Transmit (&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE, 100)!= HAL_OK) {
		LCD_UsrLog("Sent failed.\n");
		return -1;

	} else {
		if (HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 100) != HAL_OK) {
			LCD_UsrLog("Receive failed.\n");
			return -1;
		} else {
			LCD_UsrLog("Received msg: %s\n", aRxBuffer);
		}
	}

	return 0;
}

void modbus_listen()
{
	while (1) {
		if (HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 100) != HAL_OK) {
				;
		} else {
			LCD_UsrLog("Received msg: %s\n", aRxBuffer);

			if (HAL_UART_Transmit (&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE, 100)!= HAL_OK) {
				LCD_UsrLog("Sent failed.\n");
			}
		}
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
	UartHandle.Init.BaudRate   	= 9600;
	UartHandle.Init.WordLength	= UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits  	= UART_STOPBITS_1;
	UartHandle.Init.Parity     	= UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl  	= UART_HWCONTROL_NONE;
	UartHandle.Init.Mode       	= UART_MODE_TX_RX;
	UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
		LCD_UsrLog("Uart deinit error.\n");
	 // Error_Handler();

	if(HAL_UART_Init(&UartHandle) != HAL_OK)
		LCD_UsrLog("Uart init error.\n");
		// Error_Handler();

	LCD_UsrLog("UART - Initialized.\n");
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

	LCD_UsrLog("GPIO - Rx,Tx - Initialized.\n");
}
