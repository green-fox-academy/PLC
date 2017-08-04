/* This driver was made by Marton Lorinczi
 *
 * How to use this driver?
 *
 * There are two an gpio_pins_t arrays. stm32l4_digital_pins[] is for the digital pins, stm32l4_analog_pins[] is for analoge pins.
 * stm32l4_digital_pins[]'s first element (index : 0 ) is the D0 pin and the last element (index : 15) is the D15 pin.
 * stm32l4_analoge_pins[]'s first element (index : 0 ) is the A0 pin and the last element (index : 5) is the A5 pin.
 *
 * There are two init functions for init the analog or digital pins.
 * And there are some functions to read write etc... thoose pins.
 *
 * Example for an analoge pin:
 * Init:	gpio_init_digital_pin(5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);
 * Write: 	gpio_write_digital_pin(5, GPIO_PIN_SET);
 * Reset:   gpio_reset_digital_pin(5);
 */



/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef GPIO_PIN_INIT_H_INCLUDED
#define GPIO_PIN_INIT_H_INCLUDED


/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported typedef -----------------------------------------------------------*/
typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} gpio_pins_t;

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
	uint32_t ADC_channel
}gpio_analog_pins_t;

/* Exported macro ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
const gpio_pins_t stm32l476rg_digital_pins[] = {
                             //PIN: D0      RX
                             //PIN: D1      TX
	{GPIOA, GPIO_PIN_10},    //PIN: D2
	{GPIOB, GPIO_PIN_3},     //PIN: D3      PWM
	{GPIOB, GPIO_PIN_5},     //PIN: D4
	{GPIOB, GPIO_PIN_4},     //PIN: D5      PWM
	{GPIOB, GPIO_PIN_10},    //PIN: D6      PWM
	{GPIOA, GPIO_PIN_8},     //PIN: D7
	{GPIOA, GPIO_PIN_9},     //PIN: D8
	{GPIOC, GPIO_PIN_7},     //PIN: D9      PWM
	{GPIOB, GPIO_PIN_6},     //PIN: D10     PWM
	{GPIOA, GPIO_PIN_7},     //PIN: D11     PWM
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


/* Exported functions ------------------------------------------------------- */

/* ########## Functions for Digital pins ########## */

/* Function name: GPIO_Set_PIN
 * Function purpose: Set PIN to output or input
 * Function input - GPIO_Pin_t *GPIO_Pin: This is the pin to be set
 * Function input - uint32_t pode: This is the mode it can be these:
 * 		GPIO_MODE_INPUT:		Input Floating Mode
 * 		GPIO_MODE_OUTPUT_PP: 	Output Push Pull Mode
 * 		GPIO_MODE_OUTPUT_OD: 	Output Open Drain Mode
 * 		GPIO_MODE_AF_PP:		Alternate Function Push Pull Mode
 *		GPIO_MODE_AF_OD:		Alternate Function Open Drain Mode
 * Function input - uint32_t pull can be:
 * 		GPIO_PULLDOWN
 * 		GPIO_PULLUP
 * 		GPIO_NOPULL
 */
void gpio_init_digital_pin(uint8_t pin_index, uint32_t mode, uint32_t pull);

/*	Function name: gpio_write_digital_pin
 * 	Function purpose: Turn on or off the digital pin
 *	Function input - uint8_t pin_index: index of the pin
 *  Function input - GPIO_PinState State: can be: GPIO_PIN_SET or GPIO_PIN_RESET
 */
void gpio_write_digital_pin(uint8_t pin_index, GPIO_PinState State);

void gpio_set_digital_pin(uint8_t pin_index);

void gpio_reset_digital_pin(uint8_t pin_index);

void gpio_toggle_digital_pin(uint8_t pin_index);

GPIO_PinState gpio_read_digital_pin(uint8_t pin_index);

/* ########## Functions for Analoge pins ########## */

/* Function name: gpio_init_analoge_pin
 * Function purpose: Init analoge input
 * function input: uint8_t pin_index - Index of the pin 0 is A0 ... 5 is A5
 */
void gpio_init_analoge_pin(uint8_t pin_index);


#endif // GPIO_PIN_INIT_H_INCLUDED
