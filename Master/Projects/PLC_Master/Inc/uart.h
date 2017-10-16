#ifndef __UART_H_
#define __UART_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdlib.h>

#define TXBUFFERSIZE        32			// transmit
#define RXBUFFERSIZE        32			// receive

#define USARTx                           USART6
#define USARTx_CLK_ENABLE()              __USART6_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __USART6_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __USART6_RELEASE_RESET()

#define BAUDRATE						1000000

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_6 //PIN: D1
#define USARTx_TX_GPIO_PORT              GPIOC
#define USARTx_TX_AF                     GPIO_AF8_USART6
#define USARTx_RX_PIN                    GPIO_PIN_7 //PIN: D0
#define USARTx_RX_GPIO_PORT              GPIOC
#define USARTx_RX_AF                     GPIO_AF8_USART6

UART_HandleTypeDef uart_handle;

uint8_t TX_buffer[TXBUFFERSIZE];
uint8_t RX_buffer[RXBUFFERSIZE];

uint8_t address;
volatile uint8_t interrupt_flag;

void modbus_init();
void UART_send(uint8_t *buffer);


#endif /* __UART_H_ */
