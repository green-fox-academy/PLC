/* Includes ------------------------------------------------------------------*/
#include "din_slave_loop.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

/* Private variables ------------------------------------------------------- */

/* Theese arrays contains the addresses of the slaves */

/* Private functions ------------------------------------------------------- */
uint16_t generate_crc();
void wait_function();
void response_to_scan();
void send_pins_states();
void send_false_command_err();

void din_slave_loop_thread()
{
	while (1)
	{
		// Wait for message arrival
		if(interrupt_flag) {

			interrupt_flag = 0;

			// Check if it is for this slave
			if (RX_buffer[0] == slave_address) {

				switch (RX_buffer[1]) {
					case SCAN_SLAVE :
						response_to_scan();
						break;
					case READ_SLAVE :
						send_pins_states();
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
				// Check the command
			// Check if it is a broadcast message
			} else if (RX_buffer[0] == 255) {

				// Check the command
			}
		}
	}
}

/* Response for scan is the scan message itself */
void response_to_scan()
{
	UART_send(RX_buffer);
}

void send_pins_states()
{
	din_pins_states = gpio_read_8_pin(8, 15);

	TX_buffer[0] = RX_buffer[0];		//Address
	TX_buffer[1] = RX_buffer[1];		//Command
	TX_buffer[2] = din_pins_states;		//Data
	TX_buffer[3] = RX_buffer[2];		//CRC low
	TX_buffer[4] = RX_buffer[3];		//CRC high

	UART_send(TX_buffer);

}

void send_false_command_err()
{

}


void wait_function()
{
	uint8_t counter;
	uint8_t ok = 0;

	while (!interrupt_flag && !ok) {
		counter++;
		if(counter >= 100)
			ok = 1;
		HAL_Delay(10);
	}

	if(ok)
		LCD_UsrLog("Time out\n");

	interrupt_flag = 0;
}

uint16_t generate_crc()
{
	uint16_t crc;
	crc = 10000;
	return crc;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
