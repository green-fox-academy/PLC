/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MODBUS_H
#define __MODBUS_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated
   resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()  // __HAL_RCC_GPIOC_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()  // __HAL_RCC_GPIOC_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_3  // GPIO_PIN_0 for analoge
#define USARTx_TX_GPIO_PORT              GPIOA  //
#define USARTx_TX_AF                     GPIO_AF7_USART2 //GPIO_AF8_UART4
#define USARTx_RX_PIN                    GPIO_PIN_2  // GPIO_PIN_1
#define USARTx_RX_GPIO_PORT              GPIOA  //
#define USARTx_RX_AF                     GPIO_AF7_USART2 //GPIO_AF8_UART4

/* Size of Trasmission buffer */
#define TXBUFFERSIZE                      2
/* Size of Reception buffer */
#define RXBUFFERSIZE                      1

 uint8_t aTxBuffer[TXBUFFERSIZE];
 uint8_t aRxBuffer[RXBUFFERSIZE];

/* Exported macro ------------------------------------------------------------*/

void modbus_init(void);
int modbus_send_command(uint8_t slave_address);
void modbus_listen();
void modbus_error_handler(uint8_t error);
void modbus_sender_measure_test();
void modbus_receive_measure_test();

#endif /* __SOCKET__MODBUS_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
