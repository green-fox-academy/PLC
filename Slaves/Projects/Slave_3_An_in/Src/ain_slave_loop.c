/* Includes ------------------------------------------------------------------*/
#include "ain_slave_loop.h"

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

void ain_slave_loop_thread()
{

	while (1)
	{

		// Update 6 ADC data
		adc_valid_measurement();
		
		// If message arrived
		if(interrupt_flag) {

			interrupt_flag = 0;

			// Check if it is for this slave
			if (RX_buffer[0] == slave_address) {

				// Command handle
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

void send_pins_states()
{
	// Set buffer : address and command
	TX_buffer[0] = slave_address;
	TX_buffer[1] = READ_SLAVE;

	// Load valid data array to buffer
	for (uint8_t i = 0; i < 6; i++) {
		TX_buffer[(i + 1) * 2] = valid_data[i];
		TX_buffer[((i + 1) * 2) + 1] = valid_data[i] >> 8;
	}

	TX_buffer[14] = RX_buffer[2];
	TX_buffer[15] = RX_buffer[3];

	UART_send(TX_buffer);

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
