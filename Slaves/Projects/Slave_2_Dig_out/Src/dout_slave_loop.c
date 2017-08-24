/* Includes ------------------------------------------------------------------*/
#include "dout_slave_loop.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

/* Private variables ------------------------------------------------------- */

/* Theese arrays contains the addresses of the slaves */

/* Private functions ------------------------------------------------------- */
uint16_t generate_crc();
void wait_function();
void response_to_scan();
void set_pins_state();
void send_false_command_err();

void dout_slave_loop_thread()
{
	while (1)
	{
		// Wait for message arrival
		if(interrupt_flag) {

			interrupt_flag = 0;

			// Check if it is for this slave
			if (RX_buffer[0] == slave_address) {

				// Command handle
				switch (RX_buffer[1]) {
					case SCAN_SLAVE :
						response_to_scan();
						break;
					case WRITE_SLAVE :
						set_pins_state();
						break;
					case MODE_1 :
						set_to_gpio_mode();
						break;
					case MODE_2 :
						set_to_pwm_mode();
						break;
					case HOLD_INPUTS :
						break;
					case HOLD_OUTPUTS :
						break;
					case STOP_SLAVE :
						break;
					case START_SLAVE :
						break;
					default :
						send_false_command_err();
				}

			// Check if it is a broadcast message
			} else if (RX_buffer[0] == 255) {

				// Command handle
				switch (RX_buffer[1]) {
					case HOLD_INPUTS :
						break;
					case HOLD_OUTPUTS :
						break;
					case STOP_SLAVE :
						break;
					case START_SLAVE :
						break;
					default :
						send_false_command_err();

				}
			}
		}
	}
}

/* Response for scan is the scan message itself */
void response_to_scan()
{
	UART_send(RX_buffer);
}

void send_false_command_err()
{

}

void set_pins_state()
{

	if (slave_mode == MODE_1) {
		// Temporally it will send the same msg back.
		UART_send(RX_buffer);

		gpio_set_8_pin(8, 15, RX_buffer[2]);

	} else if (slave_mode == MODE_2) {

		// Temporally it will send the same msg back.
		UART_send(RX_buffer);

		// Checks if the PWM data changed, and sets the duty
		for (uint8_t i = 0; i < 3; i++) {
			if (RX_buffer[i + 2] != pwm_set_arr[i]) {
				pwm_set_duty(RX_buffer[i + 2], i);
				pwm_set_arr[i] = RX_buffer[i + 2];
			}
		}

	}
}

void set_to_gpio_mode()
{
	if (slave_mode != MODE_1) {
		slave_mode = MODE_1;
		init_gpio_digital_pins();
	}

	UART_send(RX_buffer);
}

void set_to_pwm_mode()
{
	if (slave_mode != MODE_2) {
		slave_mode = MODE_2;
		init_pwms();
	}

	UART_send(RX_buffer);
}

void wait_function()
{
	uint8_t counter = 0;
	uint8_t ok = 0;

	while (!interrupt_flag && !ok) {
		counter++;
		if(counter >= 4)
			ok = 1;
		HAL_Delay(1);
	}

	interrupt_flag = 0;
}

uint16_t generate_crc()
{
	uint16_t crc;
	crc = 10000;
	return crc;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
