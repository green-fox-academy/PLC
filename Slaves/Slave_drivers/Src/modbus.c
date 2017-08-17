/* Includes ------------------------------------------------------------------*/
#include "modbus.h"
#include "stm32l4xx_hal_uart.h"
#include "main.h"
#include "GPIO.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* UART handler declaration */
UART_HandleTypeDef UartHandle;


/* Private function prototypes -----------------------------------------------*/
 void uart_init();
 void rx_tx_GPIO_init();

/* Private functions ---------------------------------------------------------*/
 void set_analog_pin_state(uint8_t *data_8b);

uint8_t modbus_send_message(uint8_t *msg, uint8_t msg_len)
{
	uint8_t msg_size = sizeof(msg[0]) * msg_len;

	uint8_t transmit = HAL_UART_Transmit (&UartHandle, msg, msg_size, 2);

	if (transmit == HAL_OK) {
		return 1;
	}

	return 0;
}

void modbus_DOUT_listen()
{
	uint8_t receive;
	uint8_t transmit;
	aTxBuffer[0] = 0;
	aRxBuffer[1] = 0;

	while (1) {

		receive = HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 2);
		if (receive != HAL_OK) {
			;
		} else {
			if (aRxBuffer[0] == slave_address) {

				aTxBuffer[0] = aRxBuffer[1];
				transmit = HAL_UART_Transmit (&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE, 2);

				if (transmit != HAL_OK) {
					modbus_error_handler(transmit);
				}

				gpio_set_8_pin(8,15,aRxBuffer[1]);

			}
		}
	}
}

void modbus_DIN_listen()
{
	uint8_t receive;
	uint8_t transmit;
	aTxBuffer[0] = 0;
	aRxBuffer[1] = 0;

	while (1) {

		receive = HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 2);
		if (receive != HAL_OK) {
			;
		} else {
			if (aRxBuffer[0] == slave_address) {

				// Read pins from 8 to 15
				aTxBuffer[0] = gpio_read_8_pin(8, 15);

				transmit = HAL_UART_Transmit (&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE, 2);

				if (transmit != HAL_OK) {
					modbus_error_handler(transmit);
				}
			}
		}
	}
}

void modbus_ain_listen()
{
	/*
	uint8_t receive;
	uint8_t transmit;

	// This is for test till adc is finished

	for (uint8_t i = 0; i < 6; i++) {
		analoge_pins_state[i] = 10 * (i+1);
	}


	while (1) {

		receive = HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, 2, 4);
		if (receive != HAL_OK) {
			;
		} else {
			if (aRxBuffer[0] == slave_address) {

				// Read ADC data from A0 to A6
				// adc_read_pins(0,5);

				uint16_t one_adc_value = adc_measure();

				for (uint8_t i = 0; i < 6; i++) {
					analoge_pins_state[i] = one_adc_value;
				}

				//  Send back the data as a message
				transmit = HAL_UART_Transmit (&UartHandle, (uint8_t*)analoge_pins_state, 12, 4);

				if (transmit != HAL_OK) {
					modbus_error_handler(transmit);
				}
			}
		}
	}
	*/

	uint8_t receive;
	uint8_t transmit;
	uint16_t adc_value;

	while (1) {

		receive = HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 4);
		if (receive != HAL_OK) {
			;
		} else {
			if (aRxBuffer[0] == slave_address) {

				// Read ADC data from A0 to A6
	//			adc_value = adc_measure();

				if (adc_value > 2200 && adc_value < 2600) {
						adc_value = 2200;
				}

				for (uint8_t i = 0; i < 6; i++) {
					analoge_pins_state[i] = adc_value;
				}

				//  Send back the data as a message
				transmit = HAL_UART_Transmit (&UartHandle, (uint8_t*)analoge_pins_state, 12, 4);

				if (transmit != HAL_OK) {
					modbus_error_handler(transmit);
				}
			}
		}
	}
}

void modbus_aout_listen()
{
	uint8_t receive;
	uint8_t transmit;

	uint8_t msg_16b_in[13];

	while (1) {

		receive = HAL_UART_Receive(&UartHandle, (uint8_t *)msg_16b_in, 13, 4);
		if (receive != HAL_OK) {
			;
		} else {
			if (msg_16b_in[0] == slave_address) {

				set_analog_pin_state(msg_16b_in);

				// Send back the data as a message
				transmit = HAL_UART_Transmit (&UartHandle, (uint8_t*)analoge_pins_state, 12, 4);

				if (transmit != HAL_OK) {
					modbus_error_handler(transmit);
				}

				// Set analog output
				// dac_set_pins(0, 5, analoge_pins_state);
			}
		}
	}
}

void set_analog_pin_state(uint8_t *data_8b)
{
	// This loads the pinstate
	for (uint8_t i = 0; i < 6; i++) {
		analoge_pins_state[i] = data_8b[(2 * i) + 1] + (data_8b[(2 * i) + 2] << 8);
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

void uart_init()
{
	UartHandle.Instance 	   	= USARTx;
	UartHandle.Init.BaudRate   	= 460800;
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

void modbus_init()
{
	rx_tx_GPIO_init();
	uart_init();
}
