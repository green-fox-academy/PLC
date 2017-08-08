/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MODBUS_H
#define __MODBUS_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define USARTx                           USART6
#define USARTx_CLK_ENABLE()              __USART6_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __USART6_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __USART6_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_6 //PIN: D1
#define USARTx_TX_GPIO_PORT              GPIOC
#define USARTx_TX_AF                     GPIO_AF8_USART6
#define USARTx_RX_PIN                    GPIO_PIN_7 //PIN: D0
#define USARTx_RX_GPIO_PORT              GPIOC
#define USARTx_RX_AF                     GPIO_AF8_USART6

#define TXBUFFERSIZE                     2
#define RXBUFFERSIZE                  	 2

 uint8_t aTxBuffer[TXBUFFERSIZE];
 uint8_t aRxBuffer[RXBUFFERSIZE];

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void modbus_init();
void modbus_listen();
void modbus_error_handler(uint8_t error);
void modbus_sender_measure_test();
void modbus_receive_measure_test();

uint8_t* modbus_receive_data(uint8_t data_len);
uint8_t modbus_send_command(uint8_t *command, uint8_t command_len);
#endif /* __SOCKET__MODBUS_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
