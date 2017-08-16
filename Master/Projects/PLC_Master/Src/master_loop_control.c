/* Includes ------------------------------------------------------------------*/
#include "master_loop_control.h"
#include "uart.h"
#include "cmsis_os.h"
#include "main.h"
#include "lcd_log.h"
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

/* Private variables ------------------------------------------------------- */
uint16_t generate_crc();
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

	master_loop_control_init();

	scan_slaves();

	while (1) {

		/* */
		// system_check();
		// slaves_check();
		scan_inputs();
		execute_program();
		upadte_outputs ();

		osDelay(1);

	}
}

void scan_slaves()
{
	scan_digital_slaves();
	scan_ain_slaves();
	scan_aout_slaves();
}

void test_uart_sender()
{
	msg_command.address = 10;
	msg_command.command = SCAN_SLAVE;
	msg_command.crc = 3333;

	// Set the zeros to 0
	for (uint8_t i = 0; i < 12; i++) {
	msg_command.zeros[i] = 0;
	}

	while (1) {

		UART_send(&msg_command);
		HAL_Delay(500);

	}

}

void test_uart_receiver()
{
	while (interrupt_flag){
		for(uint8_t i = 0; i < 16; i++) {
			LCD_UsrLog("%d, ", RX_buffer[i]);
		}
		LCD_UsrLog("\n");
	}

	HAL_Delay(200);
}





/*	Function name:	  scan_din_slaves
 * 	Function purpose: Scan all the digital slaves in the system.
 * 					  If there is one it will register it to table
 */
void scan_digital_slaves()
{

	uint8_t j = 0;
	num_of_dig_in = 0;

	msg_command.command = SCAN_SLAVE;
	msg_command.crc = generate_crc();

	// Set the zeros to 0
	for (uint8_t i = 0; i < 12; i++) {
	msg_command.zeros[i] = 0;
	}

	// Digital input slave
	for (uint8_t i = 0; i < 4; i++) {
		msg_command.address = digital_input_slaves_address[i];

		UART_send(&msg_command);
		while(!interrupt_flag); // This is not the best
		interrupt_flag = 0;

		if (rec_msg.address == dig_rx_tx.address &&
			rec_msg.command == dig_rx_tx.command &&
			rec_msg.data == dig_rx_tx.data &&
			rec_msg.crc == dig_rx_tx.crc) {
				digital_input_slaves[i].slave_address = dig_rx_tx.address;
				digital_input_slaves[i].digital_pins_state = 0; // Set 0 for safety
		}

		// Digital output slave
		dig_rx_tx.address = digital_output_slaves_address[i];
		dig_rx_tx.crc = generate_crc();

		modbus_transmit(&dig_rx_tx, DIGITAL_RX_TX);
		HAL_UART_Receive(&UartHandle, &rec_msg, DIGITAL_RX_TX , 3);

		if (rec_msg.address == dig_rx_tx.address &&
			rec_msg.command == dig_rx_tx.command &&
			rec_msg.data == dig_rx_tx.data &&
			rec_msg.crc == dig_rx_tx.crc) {
				digital_output_slaves[i].slave_address = dig_rx_tx.address;
				digital_output_slaves[i].digital_pins_state = 0; // Set 0 for safety
		}

			digital_input_slaves[j].slave_address = dig_rx_tx.address;
			digital_input_slaves[j].digital_pins_state = 0;
			j++;
			num_of_dig_in++;
		}
	}

}


void scan_analog_slaves()
{
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
	// Variables made from tables
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

uint16_t generate_crc()
{
	uint16_t crc;
	crc = 10000;
	return crc;
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
