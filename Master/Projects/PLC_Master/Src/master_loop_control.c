/* Includes ------------------------------------------------------------------*/
#include "master_loop_control.h"
#include "uart.h"
#include "cmsis_os.h"
#include "main.h"
#include "lcd_log.h"
#include "plc_user_code.h"

 /*	Function name:
  * 	Function purpose:
  * 	Function input - :
  * 	Function input - :
  * 	Function Output - :
  */


/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

/* Private variables ------------------------------------------------------- */

/* These arrays contains the addresses of the slaves */
uint8_t digital_input_slaves_address[]  = {1,2,3,4};
uint8_t digital_output_slaves_address[] = {5,6,7,8};
uint8_t analog_input_slaves_address[]   = {9,10,11,12};
uint8_t analog_output_slaves_address[]  = {13,14,15,16};

/* Private functions ------------------------------------------------------- */

// Functions needed for communication
uint16_t generate_crc();
uint8_t wait_function();
uint8_t verify_command_address_crc(uint8_t tx_crc_start, uint8_t rx_crc_start);

// Init function
void master_loop_control_init();

// Print functions
void print_out_TX(uint8_t from, uint8_t how_many);
void print_out_RX(uint8_t from, uint8_t how_many);
void print_out_digital_input_table();
void print_out_available_slaves();
void print_out_analog_input_table();

void scan_system_slaves();

// Load functions
void load_analog_input_table();
void load_digital_input_table();
void load_input_tables();

// The logic function
void execute_program();

// Update functions
void update_outputs();
void update_digital_output_tables();
void update_analog_output_tables();

/*
 * Loop  cycle:
 * 	1 - CPU check
 * 	2 - SLaves check
 * 	3 - Scan Inputs (update input tables)
 * 	4 - Execute Program (update output tables)
 * 	5 - Update outputs (send data to output slaves)
 */

void control_slaves_thread()
{

	master_loop_control_init();

	scan_system_slaves();

	print_out_available_slaves();

	HAL_Delay(2000);

	while (1) {

		// system_check();
		// slaves_check();
		load_input_tables();

		print_out_analog_input_table();

		//print_out_digital_input_table();

		//xecute_program();

		//update_outputs();

		HAL_Delay(1000);

	}
}

/*	Function name:	 	scan_system_slaves
 * 	Function purpose:	It sends a SCAN_SLAVE message to all possible slaves,
 * 	if it receives correct answer, then puts the slave's address to its place in table.
 */
void scan_system_slaves()
{
	msg_command.command = SCAN_SLAVE;
	msg_command.crc = 3333;

	TX_buffer[1] = msg_command.command;
	TX_buffer[2] = msg_command.crc;
	TX_buffer[3] = msg_command.crc >> 8;

	for (uint8_t i = 0; i < 4; i++) {

		// #### SCAN DIGITAL INPUT ####
		TX_buffer[0] = digital_input_slaves_address[i];
		UART_send(TX_buffer);
		// If it isn't time out
		if (!wait_function()) {
			// Checks the response if it was corrupted
			if (verify_command_address_crc(2,2) == 0){
				// Loads the DIGITAL INPUT table with address
				digital_input_slaves[num_of_dig_in].slave_address = digital_input_slaves_address[i];
				num_of_dig_in++;
			}
		}

		// #### SCAN DIGITAL OUTPUT ####
		TX_buffer[0] = digital_output_slaves_address[i];
		UART_send(TX_buffer);
		// If it isn't time out
		if (!wait_function()) {
			// Checks the response if it was corrupted
			if (verify_command_address_crc(2,2) == 0){
				// Loads the DIGITAL OUTPUT table with address
				digital_output_slaves[num_of_dig_out].slave_address = digital_output_slaves_address[i];
				num_of_dig_out++;
			}
		}

		// #### SCAN ANALOG INPUT ####
		TX_buffer[0] = analog_input_slaves_address[i];
		UART_send(TX_buffer);

		// If it isn't time out
		if (!wait_function()) {
			// Checks the response if it was corrupted
			if (verify_command_address_crc(2,2) == 0){
				// Loads the ANALOG INPUT table with address
				analog_input_slaves[num_of_an_in].slave_address = analog_input_slaves_address[i];
				num_of_an_in++;
			}
		}

		// #### SCAN ANALOG OUTPUT ####
		TX_buffer[0] = analog_output_slaves_address[i];
		UART_send(TX_buffer);

		// If it isn't time out
		if (!wait_function()) {
			// Checks the response if it was corrupted
			if (verify_command_address_crc(2,2) == 0){
				// Loads the ANALOG OUTPUT table with address
				analog_output_slaves[num_of_an_out].slave_address = analog_output_slaves_address[i];
				num_of_an_out++;
			}
		}
	}
}

void load_input_tables()
{
	// load_digital_input_table();
	load_analog_input_table();
}

void load_digital_input_table()
{
	// If there is at least one digital input slave
	if (num_of_dig_in) {

		// Set message
		TX_buffer[1] = READ_SLAVE;
		TX_buffer[2] = 22;
		TX_buffer[3] = 33;

//print_out_TX(0,4);
		for (uint8_t i = 0; i < num_of_dig_in; i++) {

			// Set the address and send the message
			TX_buffer[0] = digital_input_slaves[i].slave_address;
			UART_send(TX_buffer);

			if (!wait_function()) {
//print_out_RX(0,5);
				digital_input_slaves[i].slave_status = verify_command_address_crc(2,3);
				// If the data isn't corrupted
				if (!digital_input_slaves[i].slave_status)
					// Load the slave's pinstate to the table
					digital_input_slaves[i].digital_pins_state = RX_buffer[2];
			} else {
				// Time out
				digital_input_slaves[i].slave_status = 4;
			}
		}

	} else {
		LCD_UsrLog("There are no digital inputs.\n");
	}
}

void load_analog_input_table()
{
	if (num_of_an_in) {

		// Set message
		TX_buffer[1] = READ_SLAVE;
		TX_buffer[2] = 33;
		TX_buffer[3] = 44;

		for (uint8_t i = 0; i < num_of_an_in; i++) {

			// Set the address and send the message
			TX_buffer[0] = analog_input_slaves[i].slave_address;
			UART_send(TX_buffer);

			if (!wait_function()) {
				// Checks the response if it was corrupted
				analog_input_slaves[i].slave_status = verify_command_address_crc(2,14);
				// If the data isn't corrupted
				if (!analog_input_slaves[i].slave_status) {
					// This loads the analog input pinstate table 16bit dates start from buffer[2] and ends at buffer[13]
					for (uint8_t j = 0; j < 6; j++) {
						analog_input_slaves[i].analoge_pins_state[j] = RX_buffer[(j + 1) * 2] + (RX_buffer[((j + 1) * 2) + 1] << 8);
					}
				}
			} else {
				// Time out
				analog_input_slaves[i].slave_status = 4;
			}
		}

	} else {
		LCD_UsrLog("There are no analog inputs.\n");
	}
}

void update_outputs()
{
	//update_digital_output_tables();
	update_analog_output_tables();
}

void update_digital_output_tables()
{
	// If there is at least one digital output slave
	if (num_of_dig_out) {

		TX_buffer[1] = WRITE_SLAVE;
		TX_buffer[3] = 22; // CRC low
		TX_buffer[4] = 33; // CRC high

		for (uint8_t i = 0; i < num_of_dig_out; i++) {

			TX_buffer[0] = digital_output_slaves[i].slave_address;
			TX_buffer[2] = digital_output_slaves[i].digital_pins_state;
			UART_send(TX_buffer);

			if (!wait_function())
				// Checks the response if it was corrupted
				digital_output_slaves[i].slave_status = verify_command_address_crc(3,3);
			else
				// If it was timed out
				digital_output_slaves[i].slave_status = 4;
		}

	} else {
		LCD_UsrLog("There are no digital outputs.\n");
	}

}

void update_analog_output_tables()
{
	// If there is at least one analog output slave
	if (num_of_an_out) {

		// Set command and CRC
		TX_buffer[1] = WRITE_SLAVE;
		TX_buffer[14] = 33;
		TX_buffer[15] = 44;

		for (uint8_t i = 0; i < num_of_an_out; i++) {

			// Set the address
			TX_buffer[0] = analog_output_slaves[i].slave_address;

			// Set uint16_t pinstate data to TX_buffer, from buffer[2] to buffer[13]
			for (uint8_t j = 0; j < 6; j++) {
				TX_buffer[(j + 1) * 2] = analog_output_slaves[i].analoge_pins_state[j];
				TX_buffer[((j + 1) * 2) + 1] = analog_output_slaves[i].analoge_pins_state[j] >> 8;
			}

			// Send message
			UART_send(TX_buffer);

			if (!wait_function())
				// Checks the response if it was corrupted
				analog_output_slaves[i].slave_status = verify_command_address_crc(14,14);
			else
				// Time out
				analog_output_slaves[i].slave_status = 4;
		}

	} else {
		LCD_UsrLog("There are no analog outputs.\n");
	}
}

void execute_program()
{
/*
	if (DIN1) DOU1_ON; else DOU1_OFF;
	if (DIN2) DOU2_ON; else DOU2_OFF;
*/
	AOU1 = AIN1;
	AOU2 = AIN2;
	AOU3 = AIN3;
	AOU4 = AIN4;
	AOU5 = AIN5;
	AOU6 = AIN6;

}

/*	Function name:		verify_response
 * 	Function purpose:
 * 	Function input - 	uint8_t tx_crc_start: starting index of the TX's CRC number
 * 	Function input -	uint8_t rx_crc_start: starting index of the RX's CRC number
 * 	Function Output - 	0 if it was OK
 * 						1 if address or command msg corrupted
 * 						2 if CRC was corrupted
 * 						3 if address or command and CRC was corrupted
 */
uint8_t verify_command_address_crc(uint8_t tx_crc_start, uint8_t rx_crc_start)
{
	uint8_t msg_ok = 0;

	// Check the address and the command
	if (RX_buffer[0] != TX_buffer[0] || RX_buffer[1] != TX_buffer[1])
		msg_ok++;

	// Check the CRC
	if (TX_buffer[tx_crc_start] != RX_buffer[rx_crc_start] ||
	TX_buffer[tx_crc_start + 1] != RX_buffer[rx_crc_start + 1])
		msg_ok += 2;

	return msg_ok;
}

/*	Function name:		wait_function
 * 	Function purpose:	Waits until message arrives or time out
 * 	Function Output: 	0 - If message arrived
 * 						1 - If timed out
 */
uint8_t wait_function()
{
	uint8_t counter = 0;
	uint8_t time_out = 0;

	while (!interrupt_flag && !time_out) {
		if (counter >= 4)
			time_out = 1;
		counter++;
		HAL_Delay(1);
	}

	interrupt_flag = 0;

	return time_out;
}

/*	Function name:		print_out_available_slaves
 * 	Function purpose:	Prints out available slave's number, address, pinstate
 */
void print_out_available_slaves()
{
	// LOG OUT THE RESULTS

	LCD_UsrLog("Digital inputs number: %d\n", num_of_dig_in);
	if (num_of_dig_in) {
		for (uint8_t i = 0; i < num_of_dig_in; i++) {
			LCD_UsrLog("DI_IN%d adr: %d ", i, digital_input_slaves[i].slave_address);
		}
		LCD_UsrLog("\n");
	}

	LCD_UsrLog("Digital outputs number: %d\n", num_of_dig_out);
	if (num_of_dig_out) {
		for (uint8_t i = 0; i < num_of_dig_out; i++) {
			LCD_UsrLog("DI_OUT%d adr: %d ", i, digital_output_slaves[i].slave_address);
		}
		LCD_UsrLog("\n");
	}

	LCD_UsrLog("Analog inputs number: %d\n", num_of_an_in);
	if (num_of_an_in) {
		for (uint8_t i = 0; i < num_of_an_in; i++) {
			LCD_UsrLog("AN_IN%d adr: %d ", i, analog_input_slaves[i].slave_address);
		}
		LCD_UsrLog("\n");
	}

	LCD_UsrLog("Digital inputs number: %d\n", num_of_an_out);
	if (num_of_an_out) {
		for (uint8_t i = 0; i < num_of_an_out; i++) {
			LCD_UsrLog("AN_OUT%d adr: %d ", i, analog_output_slaves[i].slave_address);
		}
		LCD_UsrLog("\n");
	}
}

void print_out_digital_input_table()
{
	for (uint8_t i = 0; i < num_of_dig_in; i++) {
		LCD_UsrLog("DIN[%d] adr: %d state: %d\n", i, digital_input_slaves[i].slave_address, digital_input_slaves[i].digital_pins_state);
	}
}

print_out_analog_input_table()
{
	for (uint8_t i = 0; i < num_of_an_in; i++) {
		LCD_UsrLog("AIN[%d]: ", i);
		for (uint8_t j = 0; j < 6; j++) {
			LCD_UsrLog("%d, ", analog_input_slaves[i].analoge_pins_state[j]);
		}
		LCD_UsrLog("\n");
	}
}

void print_out_RX(uint8_t from, uint8_t how_many)
{
	LCD_UsrLog("RX: ");

	for (uint8_t i = from; i < (from + how_many); i++) {
		LCD_UsrLog("[%d]: %d ", i, RX_buffer[i]);
	}

	LCD_UsrLog("\n");
}

void print_out_TX(uint8_t from, uint8_t how_many)
{
	LCD_UsrLog("TX: ");

	for (uint8_t i = from; i < (from + how_many); i++) {
		LCD_UsrLog("[%d]: %d ", i, TX_buffer[i]);
	}

	LCD_UsrLog("\n");
}

uint16_t generate_crc()
{
	uint16_t crc;
	crc = 10000;
	return crc;
}

/*	Function name:		master_loop_control_init
 * 	Function purpose:	Init tables
 */
void master_loop_control_init()
{
	num_of_dig_in = 0;
	num_of_dig_out = 0;
	num_of_an_in = 0;
	num_of_an_out = 0;

	for (int i = 0; i < 4; i++) {
		digital_input_slaves[i].slave_address = 0;
		digital_input_slaves[i].digital_pins_state = 0;
		digital_input_slaves[i].slave_status = 0;

		digital_output_slaves[i].slave_address = 0;
		digital_output_slaves[i].digital_pins_state = 0;
		digital_output_slaves[i].slave_status = 0;

		analog_input_slaves[i].slave_address = 0;
		analog_input_slaves[i].slave_status = 0;
		for (uint8_t j = 0; j < 6; j++) {
			analog_input_slaves[i].analoge_pins_state[j] = 0;
		}

		analog_output_slaves[i].slave_address = 0;
		analog_output_slaves[i].slave_status = 0;
		for (uint8_t j = 0; j < 6; j++) {
			analog_output_slaves[i].analoge_pins_state[j] = 0;
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
