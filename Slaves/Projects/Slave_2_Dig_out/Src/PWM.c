#include "PWM.h"

void pwm_test_init()
{
	// TIM3_CH2 init as PWM
	// D9  -> PC7
	__HAL_RCC_TIM3_CLK_ENABLE();		//clock enable

	pwm_handle.Instance = TIM3;
	pwm_handle.State = HAL_TIM_STATE_RESET;
	pwm_handle.Channel = HAL_TIM_ACTIVE_CHANNEL_2;
	pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	pwm_handle.Init.Period = 0xFFFF;
	pwm_handle.Init.Prescaler = 0;
	HAL_TIM_PWM_Init(&pwm_handle);

	pwm_oc_init.OCFastMode = TIM_OCFAST_DISABLE;
	pwm_oc_init.OCIdleState = TIM_OCIDLESTATE_RESET;
	pwm_oc_init.OCMode = TIM_OCMODE_PWM1;
	pwm_oc_init.OCPolarity = TIM_OCPOLARITY_HIGH;
	pwm_oc_init.Pulse = 0xFFFF;
	HAL_TIM_PWM_ConfigChannel(&pwm_handle, &pwm_oc_init, TIM_CHANNEL_2);
}

void pwm_set_duty(float duty)
{
	uint32_t pulse = pwm_handle.Init.Period * (duty / 100.0);
	pwm_oc_init.Pulse = pulse;
	HAL_TIM_PWM_ConfigChannel(&pwm_handle, &pwm_oc_init, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&pwm_handle, TIM_CHANNEL_2);
}

void pwm_inti(uint8_t pin_index)
{
	TIM_HandleTypeDef pwm_handle;
	TIM_OC_InitTypeDef pwm_oc_init;

	pwm_clk_enable(stm32l476rg_digital_pins_pwm[pin_index].Alternate);


}


typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
	GPIO_InitTypeDef *Alternate;
} gpio_pins_t_pwm;

typedef struct {

};


/* Private variables ---------------------------------------------------------*/
const gpio_pins_t_pwm stm32l476rg_digital_pins_pwm[] = {
	{GPIOA, GPIO_PIN_3, 0},  				   	//PIN: D0
	{GPIOA, GPIO_PIN_2, 0},						//PIN: D1
	{GPIOA, GPIO_PIN_10, 0},					//PIN: D2
	{GPIOB, GPIO_PIN_3, 0},   					//PIN: D3
	{GPIOB, GPIO_PIN_5, 0},     				//PIN: D4
	{GPIOB, GPIO_PIN_4, 0},     				//PIN: D5
	{GPIOB, GPIO_PIN_10, 0},    				//PIN: D6
	{GPIOA, GPIO_PIN_8, 0},     				//PIN: D7
	{GPIOA, GPIO_PIN_9, 0},     				//PIN: D8
	{GPIOC, GPIO_PIN_7, GPIO_AF2_TIM3},     	//PIN: D9		PWM 	TIM3_CH2
	{GPIOB, GPIO_PIN_6, GPIO_AF2_TIM4},     	//PIN: D10		PWM		TIM4_CH1
	{GPIOA, GPIO_PIN_7, GPIO_AF14_TIM17},     	//PIN: D11		PWM		TIM17_CH1
	{GPIOA, GPIO_PIN_6, 0},     				//PIN: D12
	{GPIOA, GPIO_PIN_5, 0},     				//PIN: D13
	{GPIOB, GPIO_PIN_9, 0},     				//PIN: D14
	{GPIOB, GPIO_PIN_8, 0},     				//PIN: D15
	{GPIOA, GPIO_PIN_0, 0},     				//PIN: A0 - As a Digital PIN
	{GPIOA, GPIO_PIN_1, 0},     				//PIN: A1 - As a Digital PIN
	{GPIOA, GPIO_PIN_4, 0},     				//PIN: A2 - As a Digital PIN
	{GPIOB, GPIO_PIN_0, 0},     				//PIN: A3 - As a Digital PIN
	{GPIOC, GPIO_PIN_1, 0},     				//PIN: A4 - As a Digital PIN
	{GPIOC, GPIO_PIN_0, 0},     				//PIN: A5 - As a Digital PIN
};


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

void gpio_clk_enable_pwm(GPIO_TypeDef *port)
{
	if (port == GPIOA)
		__HAL_RCC_GPIOA_CLK_ENABLE();
	else if (port == GPIOB)
		__HAL_RCC_GPIOB_CLK_ENABLE();
	else if (port == GPIOC)
		__HAL_RCC_TIM3_CLK_ENABLE();
	else if (port == GPIOD)
		__HAL_RCC_GPIOD_CLK_ENABLE();
	else if (port == GPIOE)
		__HAL_RCC_GPIOE_CLK_ENABLE();
	else if (port == GPIOF)
		__HAL_RCC_GPIOF_CLK_ENABLE();
	else if (port == GPIOG)
		__HAL_RCC_GPIOG_CLK_ENABLE();
}

void pwm_clk_enable(GPIO_TypeDef *Alternate)
{
	if (Alternate == GPIO_AF14_TIM17)
		__HAL_RCC_TIM17_CLK_ENABLE();
	else if (Alternate == GPIO_AF2_TIM4)
		__HAL_RCC_TIM4_CLK_ENABLE();
	else if (Alternate == GPIO_AF2_TIM3)
		__HAL_RCC_TIM3_CLK_ENABLE();
	/*else if (port == GPIOD)
		__HAL_RCC_GPIOD_CLK_ENABLE();
	else if (port == GPIOE)
		__HAL_RCC_GPIOE_CLK_ENABLE();
	else if (port == GPIOF)
		__HAL_RCC_GPIOF_CLK_ENABLE();
	else if (port == GPIOG)
		__HAL_RCC_GPIOG_CLK_ENABLE();*/
}


/* ########## Functions for Digital pins ########## */

void pwm_gpio_init_digital_pin(uint8_t pin_index, uint32_t mode, uint32_t pull)
{
	GPIO_InitTypeDef gpio_init_structure_pwm;

	// Set the clock
	gpio_clk_enable_pwm(stm32l476rg_digital_pins_pwm[pin_index].port);

	// Set the init structure
	gpio_init_structure_pwm.Pin = stm32l476rg_digital_pins_pwm[pin_index].pin;
	gpio_init_structure_pwm.Mode = mode;
	gpio_init_structure_pwm.Pull = pull;
	gpio_init_structure_pwm.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_structure_pwm.Alternate = stm32l476rg_digital_pins_pwm[pin_index].Alternate;

	// Init the pin
	HAL_GPIO_Init(stm32l476rg_digital_pins_pwm[pin_index].port, &gpio_init_structure_pwm);
}
