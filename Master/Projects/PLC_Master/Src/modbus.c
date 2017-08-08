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

 uint8_t data[10];

/* Private function prototypes -----------------------------------------------*/
 void rx_tx_GPIO_init();
/* Private functions ---------------------------------------------------------*/

 /*	Function name:
  * 	Function purpose:
  * 	Function input - :
  * 	Function input - :
  * 	Function Output - :
  */

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

	LCD_UsrLog("OK: %d, NOPE: %d\n", counter_ok, counter_nope);

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
			LCD_UsrLog("OK: %d, NOPE: %d\n", counter_ok, counter_nope);
			counter_nope = 0;
			counter_ok = 0;
		}

	}

}


/*	Function name: 							modbus_send_command
 * 	Function purpose:						Send command to a target slave
 * 	Function input - uint8_t *command:		Array of uint8_t, command[0] is the address of the slave,
 * 											command[1] - command[command_len -1] are the datas to be sent.
 * 	Function input - uint8_t command_len:	Length of the command.
 * 	Function Output - uint8_t:				1 if something went wrong, 0 if it was ok.
 */

uint8_t modbus_send_command(uint8_t *command, uint8_t command_len)
{
	uint8_t transmit = 0;

	transmit = HAL_UART_Transmit (&UartHandle, command, (sizeof(command[0]) * command_len), 2);

	if (transmit != HAL_OK) {
		LCD_UsrLog("Transmit, ");
		modbus_error_handler(transmit);
		return 1;
	}

	return 0;
}

/*	Function name:						modbus_receive_data
 * 	Function purpose:					Receive uint8_t data array from slave.
 * 	Function input - uint8_t data_len:	Data length to be received.
 * 	Function Output - uint8_t*:			NULL if something went wrong, data if it was ok.
 */
uint8_t* modbus_receive_data(uint8_t data_len)
{
	uint8_t receive = 0;

	receive = HAL_UART_Receive(&UartHandle, data, (sizeof(data[0]) * data_len) , 3);

	if (receive != HAL_OK) {
		LCD_UsrLog("Receive, ");
		modbus_error_handler(receive);
		return NULL;
	}

	return data;
}

void modbus_listen()
{
	uint8_t receive;
	uint8_t transmit;

	while (1) {

		receive = HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, 2, 1000);
		if (receive != HAL_OK) {
			;
			//LCD_UsrLog("Receive, ");
			modbus_error_handler(receive);
		} else {
			LCD_UsrLog("Received msg[0]:%u msg[1]: %u\n", aRxBuffer[0], aRxBuffer[1]);

			transmit = HAL_UART_Transmit (&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE, 2);

			if (transmit != HAL_OK) {
				LCD_UsrLog("Transmit, ");
				modbus_error_handler(transmit);
			}
		}
	}
}

void modbus_error_handler(uint8_t error)
{
	switch (error) {

	case 1 :
		LCD_UsrLog("Uart Error: HAL_ERROR\n");
		break;
	case 2 :
		LCD_UsrLog("Uart Error: HAL_BUSY\n");
		break;
	case 3 :
		LCD_UsrLog("Uart Error: HAL_TIMEOUT\n");
		break;
	default :
		LCD_UsrLog("Uart Error: Something else.\n");
	}
}


void modbus_init()
{
	rx_tx_GPIO_init();
	uart_init();
}

void uart_init()
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
