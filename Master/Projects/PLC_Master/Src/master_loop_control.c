/* Includes ------------------------------------------------------------------*/
#include "master_loop_control.h"
#include "modbus.h"
#include "cmsis_os.h"
#include "main.h"
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

/* Private variables ------------------------------------------------------- */

/*
 * Loop  cycle:
 * 	1 - CPU check
 * 	2 - SLaves check
 * 	3 - Scan Inputs (update input tables)
 * 	4 - Execute Program (update output tables)
 * 	5 - Update outputs (send data to slaves)
 */

void control_slaves_thread(void const * argument)
{

	scan_slaves();

	while (1) {

		/* */
		// system_check();
		// slaves_check();
		scan_inputs();
		execute_program();
		upadte_outputs();

		osDelay(1);

	}
}

void scan_slaves()
{
	scan_din_slaves();
	scan_dout_slaves();
	scan_ain_slaves();
	scan_aout_slaves();
}

scan_din_slaves()
{
	digital_rx_tx_t rec_msg;
	uint8_t j = 0;
	num_of_dig_in = 0;

	for (uint8_t i = 0; i < 4; i++) {
		dig_rx_tx.address = digital_input_slaves_address[i];
		dig_rx_tx.command = SCAN_SLAVE;
		dig_rx_tx.data = SCAN_SLAVE;
		dig_rx_tx.crc = 10000;

		modbus_transmit(&dig_rx_tx, DIGITAL_RX_TX);
		HAL_UART_Receive(&UartHandle, &rec_msg, DIGITAL_RX_TX , 3);

		if (rec_msg.address == dig_rx_tx.address && rec_msg.command == dig_rx_tx.command
			&& rec_msg.crc == dig_rx_tx.crc && rec_msg.data == dig_rx_tx.data) {

			digital_input_slaves[j].slave_address = dig_rx_tx.address;
			digital_input_slaves[j].digital_pins_state = 0;
			j++;
			num_of_dig_in++;
		}
	}
}

void scan_inputs()
{
	scan_digital_input();
	scan_analog_input();
}

void scan_digital_input(digital_rx_tx_t* digital_frame, uint8_t slave_index)
{
	modbus_transmit(digital_frame, DIGITAL_RX_TX);
	dig_rx_tx = (digital_rx_tx_t)modbus_receive(DIGITAL_RX_TX);

	if (digital_frame.address == dig_rx_tx.address &&
		digital_frame.command == dig_rx_tx.command &&
		digital_frame.crc == dig_rx_tx.crc) {

	}

	digital_input_slaves[slave_index].digital_pins_state = dig_rx_tx.data;
}

void execute_program()
{
	/* Variables made from tables */
	uint8_t din_state = digital_input_slaves[0].digital_pins_state;
	uint8_t dout_state = digital_output_slaves[0].digital_pins_state;

	if (DIN1) DOU1_ON; else DOU1_OFF;
	if (DIN2) DOU2_ON; else DOU2_OFF;
	if (DIN3) DOU3_ON; else DOU3_OFF;
	if (DIN4) DOU4_ON; else DOU4_OFF;
	if (DIN5) DOU5_ON; else DOU5_OFF;
	if (DIN6) DOU6_ON; else DOU6_OFF;
	if (DIN7) DOU7_ON; else DOU7_OFF;
	if (DIN8) DOU8_ON; else DOU8_OFF;

	// Update output table

	digital_output_slaves[0].digital_pins_state = dout_state;
	//a_out_state = aout_state;
}

void master_loop_control_init()
{
	for (int i = 0; i < 4; i++) {
		digital_input_slaves[i].slave_address = digital_input_slaves_address[i];
		digital_input_slaves[i].digital_pins_state = 0;

		digital_output_slaves[i].slave_address = digital_output_slaves_address[i];
		digital_output_slaves[i].digital_pins_state = 0;

		analog_input_slaves[i].slave_address = analog_input_slaves_address[i];
		for (uint8_t j = 0; j < 6; j++) {
			analog_input_slaves[i].analoge_pins_state[j] = 0;
		}

		analog_output_slaves[i].slave_address = analog_output_slaves_address[i];
		for (uint8_t j = 0; j < 6; j++) {
			analog_output_slaves[i].analoge_pins_state[j];
		}
	}
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
