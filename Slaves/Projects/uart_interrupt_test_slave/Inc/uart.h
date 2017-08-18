#ifndef __UART_H_
#define __UART_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"
#include <string.h>
#include <stdlib.h>

#define TXBUFFERSIZE        16			// transmit
#define RXBUFFERSIZE        16			// receive

#define USARTx                           USART3
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()    	 __HAL_RCC_GPIOC_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

#define BAUDRATE						115200

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_10
#define USARTx_TX_GPIO_PORT              GPIOC
#define USARTx_TX_AF                     GPIO_AF7_USART3
#define USARTx_RX_PIN                    GPIO_PIN_11
#define USARTx_RX_GPIO_PORT              GPIOC
#define USARTx_RX_AF                     GPIO_AF7_USART3

UART_HandleTypeDef uart_handle;

uint8_t TX_buffer[TXBUFFERSIZE];
uint8_t RX_buffer[RXBUFFERSIZE];
uint8_t address;
volatile uint8_t interrupt_flag;

void modbus_init();
void UART_send(uint8_t *buffer);


#endif /* __UART_H_ */
