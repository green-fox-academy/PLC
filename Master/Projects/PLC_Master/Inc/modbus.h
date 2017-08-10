/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MODBUS_H
#define __MODBUS_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

// Defines for logic process

/* Logical operators*/
#define AND 	&&
#define OR		||
#define NOT		!

/* Digital input pins value */
#define DIN1 	(din_state & 0b00000001)  //D8 on input board
#define DIN2 	(din_state & 0b00000010)  //D9 on input board
#define DIN3 	(din_state & 0b00000100)  //D10 on input board
#define DIN4 	(din_state & 0b00001000)  //D11 on input board
#define DIN5 	(din_state & 0b00010000)  //D12 on input board
#define DIN6 	(din_state & 0b00100000)  //D13 on input board
#define DIN7 	(din_state & 0b01000000)  //D14 on input board
#define DIN8	(din_state & 0b10000000)  //D15 on input board

/* Digital input pins value */
#define DOU1 	(dout_state & 0b00000001)  //D8 status on output board
#define DOU2 	(dout_state & 0b00000010)  //D9 status on output board
#define DOU3 	(dout_state & 0b00000100)  //D10 status on output board
#define DOU4 	(dout_state & 0b00001000)  //D11 status on output board
#define DOU5 	(dout_state & 0b00010000)  //D12 status on output board
#define DOU6 	(dout_state & 0b00100000)  //D13 status on output board
#define DOU7 	(dout_state & 0b01000000)  //D14 status on output board
#define DOU8	(dout_state & 0b10000000)  //D15 status on output board

/* Write digital output pins */
#define DOU1_ON		(dout_state |= 0b00000001)  //D8 on output board
#define DOU1_OFF	(dout_state &= 0b11111110)	//D8 on output board
#define DOU2_ON		(dout_state |= 0b00000010)	//D9 on output board
#define DOU2_OFF	(dout_state &= 0b11111101)	//D9 on output board
#define DOU3_ON		(dout_state |= 0b00000100)	//D10 on output board
#define DOU3_OFF	(dout_state &= 0b11111011)	//D10 on output board
#define DOU4_ON		(dout_state |= 0b00001000)	//D11 on output board
#define DOU4_OFF	(dout_state &= 0b11110111)	//D11 on output board
#define DOU5_ON		(dout_state |= 0b00010000)	//D12 on output board
#define DOU5_OFF	(dout_state &= 0b11101111)	//D12 on output board
#define DOU6_ON		(dout_state |= 0b00100000)	//D13 on output board
#define DOU6_OFF	(dout_state &= 0b11011111)	//D13 on output board
#define DOU7_ON		(dout_state |= 0b01000000)	//D14 on output board
#define DOU7_OFF	(dout_state &= 0b10111111)	//D14 on output board
#define DOU8_ON		(dout_state |= 0b10000000)	//D15 on output board
#define DOU8_OFF	(dout_state &= 0b01111111)	//D15 on output board

/* Analog input value of pins */
#define AIN1		ain_state[0]
#define AIN2		ain_state[1]
#define AIN3		ain_state[2]
#define AIN4		ain_state[3]
#define AIN5		ain_state[4]
#define AIN6		ain_state[5]

/* Write analoge output pins */
#define AOU1=		aout_state[0]=
#define AOU2=		aout_state[1]=
#define AOU3=		aout_state[2]=
#define AOU4=		aout_state[3]=
#define AOU5=		aout_state[4]=
#define AOU6=		aout_state[5]=

// Defines for UART Init

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
uint16_t* modbus_receive_u16_data(uint8_t data_len);
uint8_t modbus_send_command(uint8_t *command, uint8_t command_len);
#endif /* __SOCKET__MODBUS_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
