/* Includes ------------------------------------------------------------------*/
#include "GPIO.h"

/* Private function prototypes -----------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} gpio_pins_t;

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
	uint32_t ADC_channel;
}gpio_analog_pins_t;

/* Private variables ---------------------------------------------------------*/
const gpio_pins_t stm32l476rg_digital_pins[] = {
	{GPIOA, GPIO_PIN_3},     //PIN: D0
	{GPIOA, GPIO_PIN_2},     //PIN: D1
	{GPIOA, GPIO_PIN_10},    //PIN: D2
	{GPIOB, GPIO_PIN_3},     //PIN: D3
	{GPIOB, GPIO_PIN_5},     //PIN: D4
	{GPIOB, GPIO_PIN_4},     //PIN: D5
	{GPIOB, GPIO_PIN_10},    //PIN: D6
	{GPIOA, GPIO_PIN_8},     //PIN: D7
	{GPIOA, GPIO_PIN_9},     //PIN: D8
	{GPIOC, GPIO_PIN_7},     //PIN: D9
	{GPIOB, GPIO_PIN_6},     //PIN: D10
	{GPIOA, GPIO_PIN_7},     //PIN: D11
	{GPIOA, GPIO_PIN_6},     //PIN: D12
	{GPIOA, GPIO_PIN_5},     //PIN: D13
	{GPIOB, GPIO_PIN_9},     //PIN: D14
	{GPIOB, GPIO_PIN_8},     //PIN: D15
	{GPIOA, GPIO_PIN_0},     //PIN: A0 - As a Digital PIN
	{GPIOA, GPIO_PIN_1},     //PIN: A1 - As a Digital PIN
	{GPIOA, GPIO_PIN_4},     //PIN: A2 - As a Digital PIN
	{GPIOB, GPIO_PIN_0},     //PIN: A3 - As a Digital PIN
	{GPIOC, GPIO_PIN_1},     //PIN: A4 - As a Digital PIN
	{GPIOC, GPIO_PIN_0},     //PIN: A5 - As a Digital PIN
};

const gpio_analog_pins_t stm32l476rg_analog_pins[] = {
	{GPIOA, GPIO_PIN_0, ADC_CHANNEL_5},    //PIN: A0 - ADC12_IN5
	{GPIOA, GPIO_PIN_1, ADC_CHANNEL_6},    //PIN: A1 - ADC12_IN6
	{GPIOA, GPIO_PIN_4, ADC_CHANNEL_9},    //PIN: A2 - ADC12_IN9
	{GPIOB, GPIO_PIN_0, ADC_CHANNEL_15},   //PIN: A3 - ADC12_IN15
	{GPIOC, GPIO_PIN_1, ADC_CHANNEL_2},    //PIN: A4 - ADC123_IN2 // GPIOB GPIO_PIN_9 - I2C1_SDA
	{GPIOC, GPIO_PIN_0, ADC_CHANNEL_1},    //PIN: A5 - ADC123_IN1 // GPIOB GPIO_PIN_8 - I2C1_SCL
};

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
}


/* ########## Functions for Digital pins ########## */

void gpio_init_digital_pin(uint8_t pin_index, uint32_t mode, uint32_t pull)
{
	GPIO_InitTypeDef gpio_init_structure;

	// Set the clock
	gpio_clk_enable(stm32l476rg_digital_pins[pin_index].port);

	// Set the init structure
	gpio_init_structure.Pin = stm32l476rg_digital_pins[pin_index].pin;
	gpio_init_structure.Mode = mode;
	gpio_init_structure.Pull = pull;
	gpio_init_structure.Speed = GPIO_SPEED_FAST;

	// Init the pin
	HAL_GPIO_Init(stm32l476rg_digital_pins[pin_index].port, &gpio_init_structure);
}

/* Init PINs from DPIN8 to DPIN15 as a digital outputs */
void init_gpio_digital_pins()
{
	for (int i = 8; i < 16; i++) {
		gpio_init_digital_pin(i, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);
	}
}

void gpio_write_digital_pin(uint8_t pin_index, GPIO_PinState State)
{
	HAL_GPIO_WritePin(stm32l476rg_digital_pins[pin_index].port, stm32l476rg_digital_pins[pin_index].pin, State);
}

void gpio_set_digital_pin(uint8_t pin_index)
{
	HAL_GPIO_WritePin(stm32l476rg_digital_pins[pin_index].port, stm32l476rg_digital_pins[pin_index].pin, GPIO_PIN_SET);
}

void gpio_reset_digital_pin(uint8_t pin_index)
{
	HAL_GPIO_WritePin(stm32l476rg_digital_pins[pin_index].port, stm32l476rg_digital_pins[pin_index].pin, GPIO_PIN_RESET);
}

void gpio_toggle_digital_pin(uint8_t pin_index)
{
	HAL_GPIO_TogglePin(stm32l476rg_digital_pins[pin_index].port, stm32l476rg_digital_pins[pin_index].pin);
}

GPIO_PinState gpio_read_digital_pin(uint8_t pin_index)
{
	return HAL_GPIO_ReadPin(stm32l476rg_digital_pins[pin_index].port, stm32l476rg_digital_pins[pin_index].pin);
}

/*	Function name:							gpio_read_8_pin
 * 	Function purpose:						Read pin states and make an uint8_t from them.
 * 	Function input - uint8_t from:			First Pin (D8)
 * 	Function input - uint8_t to:			Last Pin  (D15)
 * 	Function Output - uint8_t pins_states:	This is the pins states example: 0b10110101 (The last bit is the D8 and the first bit is the D15)
 */
uint8_t gpio_read_8_pin(uint8_t from_pin_index, uint8_t to_pin_index)
{
	uint8_t pins_states = 0;

	for (uint8_t i = from_pin_index; i  <= to_pin_index; i++) {
		pins_states += (gpio_read_digital_pin(i) << (i - from_pin_index));
	}

	return pins_states;
}

/*	Function name:							gpio_set_8_pin
 * 	Function purpose:						Sets the Digital outputs
 * 	Function input - uint8_t from:			First Pin (D8)
 * 	Function input - uint8_t to:			Last Pin  (D15)
 * 	Function input - uint8_t data:			Data came from Master, last bit is the from(index), first bit is the to(index).
 */
void gpio_set_8_pin(uint8_t from, uint8_t to, uint8_t data)
{
	uint8_t pin_index;

	for (uint8_t i = from; i <= to; i++) {
		/* this will move the i bit from behind to the first place, then it will move it back to last bits place
		   example: we want this numbers 3th bit 01001101 << (7-2) -> 10100000, and now 10100000 >> 7 -> 00000001 = 1 */

		pin_index = data & (1 << (i - from));
		gpio_write_digital_pin(i, (pin_index >> (i - from)));
	}
}

/* Init Pins from DPIN0 to DPIN4 as an Digital inputs for Slave_address */
void gpio_set_address_pins()
{
	for (int i = 2; i < 7; i++) {
		gpio_init_digital_pin(i, GPIO_MODE_INPUT, GPIO_PULLDOWN);
	}
}

/* Init PINs from DPIN8 to DPIN15 as a digital outputs */
void gpio_set_dig_out_pins()
{
	for (int i = 8; i < 16; i++) {
		gpio_init_digital_pin(i, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);
	}
}

/* Init PINs from DPIN8 to DPIN15 as a digital inputs */
void gpio_set_dig_in_pins()
{
	for (int i = 5; i < 16; i++) {
		gpio_init_digital_pin(i, GPIO_MODE_INPUT, GPIO_PULLDOWN);
	}
}
/* Function name: 		slave_address_set
 * Function purpose:
 *
 */
uint8_t slave_address_set()
{
	uint8_t slave_adr = 0;

	for (uint8_t i = 2; i < 7; i++) {
		slave_adr += (gpio_read_digital_pin(i) << (i - 2));
	}

	return slave_adr;
}

/* ########## Functions for Analog pins ########## */

void gpio_init_analog_pin(uint8_t pin_index)
{
	GPIO_InitTypeDef gpio_init_structure;

	// Set the clock
	gpio_clk_enable(stm32l476rg_analog_pins[pin_index].port);

	gpio_init_structure.Pin = stm32l476rg_analog_pins[pin_index].pin;
	gpio_init_structure.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	gpio_init_structure.Pull = GPIO_NOPULL;
	gpio_init_structure.Speed = GPIO_SPEED_FREQ_MEDIUM;

	HAL_GPIO_Init(stm32l476rg_analog_pins[pin_index].port, &gpio_init_structure);

	return;
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
