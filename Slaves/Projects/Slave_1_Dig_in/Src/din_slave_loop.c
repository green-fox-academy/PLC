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

			// Check is it for this slave
			if (RX_buffer[0] == slave_address) {

				// Check the command
				if (RX_buffer[1] == SCAN_SLAVE) {
					response_to_scan();

				} else if (RX_buffer[1] == READ_SLAVE) {
					send_pins_states();

				} else {
					send_false_command_err();
				}
			}
		}
	}
}

/* Response for scan is the scan message */
void response_to_scan()
{
	/*
	TX_buffer[0] = 111;
	TX_buffer[1] = 222;
	TX_buffer[2] = 133;
	TX_buffer[3] = 244;
	*/
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
