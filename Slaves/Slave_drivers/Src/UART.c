#include "UART.h"

uint8_t TxMessage[] = "UART Ready\n\r";

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

void MX_GPIO_init()
{
	__GPIOA_CLK_ENABLE();
	__USART2_CLK_ENABLE();
}

void pin_init()
{
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void UART2_init()
{
	UartHandle.Instance        = USART2;
	UartHandle.Init.BaudRate   = 9600;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits   = UART_STOPBITS_1;
	UartHandle.Init.Parity     = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode       = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	UartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	HAL_UART_Init(&UartHandle);
}


PUTCHAR_PROTOTYPE	// Retargets the C library printf function to the USART.
{
  HAL_UART_Transmit(&uart_handle, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

HAL_UART_Transmit(&UartHandle, TxMessage, 255, 10);
