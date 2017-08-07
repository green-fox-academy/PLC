/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MODBUS_H
#define __MODBUS_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/*	UART4 on A0-TX, A1-RX;
 *
#define USARTx                           UART4
#define USARTx_CLK_ENABLE()              __HAL_RCC_UART4_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_UART4_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_UART4_RELEASE_RESET()

#define USARTx_TX_PIN                    GPIO_PIN_0
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF8_UART4
#define USARTx_RX_PIN                    GPIO_PIN_1
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF8_UART4
/*

/* Definition for USARTx clock resources */
#define USARTx                           USART3
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_10
#define USARTx_TX_GPIO_PORT              GPIOC
#define USARTx_TX_AF                     GPIO_AF7_USART3
#define USARTx_RX_PIN                    GPIO_PIN_11
#define USARTx_RX_GPIO_PORT              GPIOC
#define USARTx_RX_AF                     GPIO_AF7_USART3

/* Size of Trasmission buffer */
#define TXBUFFERSIZE                      1
/* Size of Reception buffer */
#define RXBUFFERSIZE                      2

 uint8_t aTxBuffer[TXBUFFERSIZE];
 uint8_t aRxBuffer[RXBUFFERSIZE];

/* Exported macro ------------------------------------------------------------*/

void modbus_init(void);
void modbus_error_handler(uint8_t error);

void modbus_DOUT_listen();
void modbus_DIN_listen();
uint8_t modbus_send_message(uint8_t *msg, uint8_t msg_len);

#endif /* __SOCKET__MODBUS_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
