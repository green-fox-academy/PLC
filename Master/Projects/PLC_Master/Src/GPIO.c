/* Includes ------------------------------------------------------------------*/
#include "GPIO.h"
#include "stm32f7xx_hal.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

void gpio_clk_enable(GPIO_TypeDef *port)
{
	if (port == GPIOA)
		__HAL_RCC_GPIOA_CLK_ENABLE();
	else if (port == GPIOB)
		__HAL_RCC_GPIOB_CLK_ENABLE();
	else if (port == GPIOC)
		__HAL_RCC_GPIOC_CLK_ENABLE();
	else if (port == GPIOD)
		__HAL_RCC_GPIOD_CLK_ENABLE();
	else if (port == GPIOE)
		__HAL_RCC_GPIOE_CLK_ENABLE();
	else if (port == GPIOF)
		__HAL_RCC_GPIOF_CLK_ENABLE();
	else if (port == GPIOG)
		__HAL_RCC_GPIOG_CLK_ENABLE();
	else if (port == GPIOH)
		__HAL_RCC_GPIOH_CLK_ENABLE();
	else if (port == GPIOI)
		__HAL_RCC_GPIOI_CLK_ENABLE();
	else if (port == GPIOJ)
		__HAL_RCC_GPIOJ_CLK_ENABLE();
	else if (port == GPIOK)
		__HAL_RCC_GPIOK_CLK_ENABLE();
}


/* ########## Functions for Digital pins ########## */

void gpio_init_digital_pin(uint8_t pin_index, uint32_t mode, uint32_t pull)
{
	GPIO_InitTypeDef gpio_init_structure;

	// Set the clock
	GPIO_clk_enable(stm32f7_digital_pins[pin_index].port);

	// Set the init structure
	gpio_init_structure.Pin = stm32f7_digital_pins[pin_index].pin;
	gpio_init_structure.Mode = mode;
	gpio_init_structure.Pull = pull;
	gpio_init_structure.Speed = GPIO_SPEED_FAST;

	// Init the pin
	HAL_GPIO_Init(stm32f7_digital_pins[pin_index].port, &gpio_init_structure);

}


void gpio_write_digital_pin(uint8_t pin_index, GPIO_PinState State)
{
	HAL_GPIO_WritePin(stm32f7_digital_pins[pin_index].port, stm32f7_digital_pins[pin_index].pin, State);
}

void gpio_set_digital_pin(uint8_t pin_index)
{
	HAL_GPIO_WritePin(stm32f7_digital_pins[pin_index].port, stm32f7_digital_pins[pin_index].pin, GPIO_PIN_SET);
}

void gpio_reset_digital_pin(uint8_t pin_index)
{
	HAL_GPIO_WritePin(stm32f7_digital_pins[pin_index].port, stm32f7_digital_pins[pin_index].pin, GPIO_PIN_RESET);
}

void gpio_toggle_digital_pin(uint8_t pin_index)
{
	HAL_GPIO_TogglePin(stm32f7_digital_pins[pin_index].port, stm32f7_digital_pins[pin_index].pin);
}

GPIO_PinState gpio_read_digital_pin(uint8_t pin_index)
{
	return HAL_GPIO_ReadPin(stm32f7_digital_pins[pin_index].port, stm32f7_digital_pins[pin_index].pin);
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
