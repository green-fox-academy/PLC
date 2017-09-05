/* Includes -----------p-------------------------------------------------------*/
#include "master_loop_control.h"
#include "uart.h"
#include "cmsis_os.h"
#include "main.h"
#include "lcd_log.h"
#include "plc_user_code.h"
#include "timer.h"

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
uint8_t temp_counter = 1;
uint8_t program_mode = 0;
uint8_t slave_power = 0;


/* These arrays contains the addresses of the slaves */
uint8_t digital_input_slaves_address[]  = {1,2,3,4};
uint8_t digital_output_slaves_address[] = {5,6,7,8};
uint8_t analog_input_slaves_address[]   = {9,10,11,12};
uint8_t analog_output_slaves_address[]  = {13,14,15,16};

/* Private functions ------------------------------------------------------- */

// Functions needed for communication
uint16_t generate_crc();
void empty_output_tables();
uint8_t wait_function();
uint8_t verify_command_address_crc(uint8_t tx_crc_start, uint8_t rx_crc_start);

// Init function
void master_loop_control_init();

// Print functions
void print_out_TX(uint8_t from, uint8_t how_many);
void print_out_RX(uint8_t from, uint8_t how_many);
void print_out_available_slaves();
void print_out_digital_input_table();
void print_out_digital_output_table();
void print_out_analog_input_table();
void print_out_analog_output_table();

// Power management
void turn_on_slaves();
void turn_off_slaves();

void scan_system_slaves();
uint8_t scan_slave(uint8_t slave_address);
uint8_t scan_slave_3_times(uint8_t slave_address);
void slaves_check();

// Message functions
uint8_t set_digital_output_slave_mode(uint8_t mode, uint8_t slave_index);

// Load functions
void load_analog_input_table();
void load_digital_input_table();
void load_input_tables();

// The logic function
void execute_program();
void check_mode_select();

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

	turn_off_slaves();

	HAL_Delay(2000);

	turn_on_slaves();

	HAL_Delay(4000);

	scan_system_slaves();

	print_out_available_slaves();

	HAL_Delay(2000);

	while (1) {


		// system_check();

		load_input_tables();

		// print_out_digital_input_table();
		// print_out_analog_input_table();

		check_mode_select();

		execute_program();

		// print_out_digital_output_table();
		// print_out_analog_output_table();

		update_outputs();

		slaves_check();

		HAL_Delay(50);
	}
}


void turn_on_slaves()
{
	gpio_set_digital_pin(8);
	slave_power = 1;
}

void turn_off_slaves()
{
	gpio_reset_digital_pin(8);
	slave_power = 0;
}

/*	Function name:	 	scan_system_slaves
 * 	Function purpose:	It sends a SCAN_SLAVE message to all possible slaves,
 * 	if it receives correct answer, then puts the slave's address to its place in table.
 */
void scan_system_slaves()
{
	for (uint8_t i = 0; i < 4; i++) {

		// #### SCAN DIGITAL INPUT ####
		if (!scan_slave(digital_input_slaves_address[i])) {
			digital_input_slaves[num_of_dig_in].slave_address = digital_input_slaves_address[i];
			num_of_dig_in++;
		}

		// #### SCAN DIGITAL OUTPUT ####
		if (!scan_slave(digital_output_slaves_address[i])) {
			digital_output_slaves[num_of_dig_out].slave_address = digital_output_slaves_address[i];
			num_of_dig_out++;
		}

		// #### SCAN ANALOG INPUT ####
		if (!scan_slave(analog_input_slaves_address[i])) {
			analog_input_slaves[num_of_an_in].slave_address = analog_input_slaves_address[i];
			num_of_an_in++;
		}

		// #### SCAN ANALOG OUTPUT ####
		if (!scan_slave(analog_output_slaves_address[i])) {
			analog_output_slaves[num_of_an_out].slave_address = analog_output_slaves_address[i];
			num_of_an_out++;
		}
	}
}


/*	Function name: scan slave
 * 	Function purpose: send scan message to a slave
 * 	Function input - : uint8_t slave_address: address of the slave
 * 	Function Output - : 0 if it was OK
 * 	Function Output - : 1-3 if message corrputed
 * 	Function Output - : 4 if timed out
 */
uint8_t scan_slave(uint8_t slave_address)
{
	uint8_t status;

	TX_buffer[0] = slave_address;
	TX_buffer[1] = SCAN_SLAVE;
	TX_buffer[2] = 11;	// CRC LOW
	TX_buffer[3] = 22;	// CRC HIGH

	UART_send(TX_buffer);

	if (!wait_function())
		status = verify_command_address_crc(2,2);
	else
		status = 4;

	return status;
}


void slaves_check()
{
	if (slave_power) {

		for (uint8_t i = 0; i < num_of_dig_in; i++) {
			if (digital_input_slaves[i].slave_status == 4) {
				if (scan_slave_3_times(digital_input_slaves[i].slave_address)) {
					turn_off_slaves();
					LCD_UsrLog("Digital Input Slave adr:[%d] is Offline!\n", digital_input_slaves[i].slave_address);
				}
			}
		}

		for (uint8_t i = 0; i < num_of_dig_out; i++) {
			if (digital_output_slaves[i].slave_status == 4) {
				LCD_UsrLog("Slave adr:[%d] is Offline!\n", digital_output_slaves[i].slave_address);
				if (scan_slave_3_times(digital_output_slaves[i].slave_address)) {
					turn_off_slaves();
					LCD_UsrLog("Digital Input Slave adr:[%d] is Offline!\n", digital_output_slaves[i].slave_address);
				}
			}
		}

		for (uint8_t i = 0; i < num_of_an_in; i++) {
			if (analog_input_slaves[i].slave_status == 4) {
				if (scan_slave_3_times(analog_input_slaves[i].slave_address)) {
					turn_off_slaves();
					LCD_UsrLog("Digital Input Slave adr:[%d] is Offline!\n", analog_input_slaves[i].slave_address);
				}
			}
		}

		for (uint8_t i = 0; i < num_of_an_out; i++) {
			if (analog_output_slaves[i].slave_status == 4) {
				if (scan_slave_3_times(analog_output_slaves[i].slave_address)) {
					turn_off_slaves();
					LCD_UsrLog("Digital Input Slave adr:[%d] is Offline!\n", analog_output_slaves[i].slave_address);
				}
			}
		}
	}
}

uint8_t scan_slave_3_times(uint8_t slave_address)
{
	uint8_t counter = 0;
	uint8_t status = 0;

	while (!status && counter != 3)
	{
		counter++;

		if(!scan_slave(slave_address))
			status = 1;
	}

	if (status)
		// Slave is online
		return 0;
	else
		// Slave is offline
		return 1;
}


void load_input_tables()
{
	load_digital_input_table();
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
	//		print_out_TX(2,6);
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
	}
}

void update_outputs()
{
	update_digital_output_tables();
	update_analog_output_tables();
}

void update_digital_output_tables()
{
	// If there is at least one digital output slave
	if (num_of_dig_out) {

		TX_buffer[1] = WRITE_SLAVE;

		for (uint8_t i = 0; i < num_of_dig_out; i++) {

			// Set slave address
			TX_buffer[0] = digital_output_slaves[i].slave_address;

			// Send mode switch message if mode was changed
			if (digital_output_slaves[i].mode_changed_flag) {

				// If slave mode set successfully
				if (!set_digital_output_slave_mode(digital_output_slaves[i].mode, i)) {
					digital_output_slaves[i].mode_changed_flag = 0;
					//LCD_UsrLog("Dig out[%d] mode: %d", i, digital_output_slaves[i].mode);
				} else {
					LCD_UsrLog("Mode sending error");
				}
			}

			// Normal Out mode
			if (digital_output_slaves[i].mode == SLAVE_MODE_1) {

				TX_buffer[2] = digital_output_slaves[i].digital_pins_state;
				TX_buffer[3] = 22; // CRC low
				TX_buffer[4] = 33; // CRC high

				UART_send(TX_buffer);

				if (!wait_function())
					// Checks the response if it was corrupted
					digital_output_slaves[i].slave_status = verify_command_address_crc(3,3);
				else
					// If it was timed out
					digital_output_slaves[i].slave_status = 4;

			// PWM mode
			} else if (digital_output_slaves[i].mode == SLAVE_MODE_2) {

				TX_buffer[2] = digital_output_slaves[i].pwm_duty_arr[0];
				TX_buffer[3] = digital_output_slaves[i].pwm_duty_arr[1];
				TX_buffer[4] = digital_output_slaves[i].pwm_duty_arr[2];
				TX_buffer[5] = 22; // CRC low
				TX_buffer[6] = 33; // CRC high

				UART_send(TX_buffer);

				if (!wait_function())
					// Checks the response if it was corrupted
					digital_output_slaves[i].slave_status = verify_command_address_crc(5,5);
				else
					// If it was timed out
					digital_output_slaves[i].slave_status = 4;

			}
		}
	}
}

void update_analog_output_tables()
{
	// If there is at least one analog output slave
	if (num_of_an_out) {

		// Set command and CRC
		TX_buffer[1] = WRITE_SLAVE;
		TX_buffer[6] = 33;
		TX_buffer[7] = 44;

		for (uint8_t i = 0; i < num_of_an_out; i++) {

			// Set the address
			TX_buffer[0] = analog_output_slaves[i].slave_address;

			// Set uint16_t pinstate data to TX_buffer, from buffer[2] to buffer[13]
			for (uint8_t j = 0; j < 2; j++) {
				TX_buffer[(j + 1) * 2] = analog_output_slaves[i].analoge_pins_state[j];
				TX_buffer[((j + 1) * 2) + 1] = analog_output_slaves[i].analoge_pins_state[j] >> 8;
			}

			// Send message
			UART_send(TX_buffer);

			if (!wait_function())
				// Checks the response if it was corrupted
				analog_output_slaves[i].slave_status = verify_command_address_crc(6,6);
			else
				// Time out
				analog_output_slaves[i].slave_status = 4;
		}
	}
}



uint8_t set_digital_output_slave_mode(uint8_t mode, uint8_t slave_index)
{
	uint8_t set_done= 0;

	if (slave_index > (num_of_dig_out - 1)) {
		set_done = 1;

	} else {

		TX_buffer[0] = digital_output_slaves[slave_index].slave_address;
		TX_buffer[1] = mode;
		TX_buffer[2] = 3333;
		TX_buffer[3] = 3333 >> 8;

		UART_send(TX_buffer);

		if (!wait_function()) {

			digital_output_slaves[slave_index].slave_status = verify_command_address_crc(2,2);

			// Checks the response if it was corrupted
			if (!digital_output_slaves[slave_index].slave_status)
				digital_output_slaves[slave_index].mode = mode;
			else
				set_done = 1;


		} else {
			// If it was timed out
			digital_output_slaves[slave_index].slave_status = 4;

			set_done = 1;
		}
	}

	return set_done;
}

void check_mode_select()
{
	uint8_t D2_pinstate = gpio_read_digital_pin(2);
	uint8_t D3_pinstate = gpio_read_digital_pin(3);

	if (!D2_pinstate && !D3_pinstate) {
		if (program_mode != 0) {
			empty_output_tables();
			print_out_digital_output_table();
			program_mode = 0;
			LCD_UsrLog("mode: 0\n");
		}

	} else if (D2_pinstate && !D3_pinstate) {
		if (program_mode != 1) {
			empty_output_tables();
			print_out_digital_output_table();
			program_mode = 1;
			LCD_UsrLog("mode: 1\n");
		}

	} else if (D3_pinstate && !D2_pinstate) {
		if (program_mode != 2) {
			empty_output_tables();
			print_out_digital_output_table();
			program_mode = 2;
			LCD_UsrLog("mode: 2\n");
		}
	}
}

void execute_program()
{

	if (program_mode == 0) {

		if (digital_output_slaves[0].mode != SLAVE_MODE_1) {

			digital_output_slaves[0].mode = SLAVE_MODE_1;
			digital_output_slaves[0].mode_changed_flag = 1;

		} else {

			// Direct connection between switchies and LEDS from 1 - 6
			if (DIN1) DOU1_ON; else DOU1_OFF;
			if (DIN2) DOU2_ON; else DOU2_OFF;
			if (DIN3) DOU3_ON; else DOU3_OFF;
			if (DIN4) DOU4_ON; else DOU4_OFF;
			if (DIN5) DOU5_ON; else DOU5_OFF;
			if (DIN6) DOU6_ON; else DOU6_OFF;

			// Direct connection between Potmeter 1,2 and LED 7,8
			AOU1 = AIN1;
			AOU2 = AIN2;
		}

	} else if (program_mode == 1) {

		if (digital_output_slaves[0].mode != SLAVE_MODE_1) {

			digital_output_slaves[0].mode = SLAVE_MODE_1;
			digital_output_slaves[0].mode_changed_flag = 1;

		} else {

			if (DIN1) {
				DOU1_ON;
				AOU1 = AIN1 / 2;
			} else {
				DOU1_OFF;
				AOU1 = 0;
			}

			if (DIN2 && !DIN1) {
				DOU2_ON;
				AOU1 = AIN1;
			} else {
				DOU2_OFF;
				AOU1 = 0;
			}

			if (DIN3) {
				DOU3_ON;
				AOU2 = AIN1 / 2 + AIN2 / 4 + AIN3 / 4;
			} else {
				DOU3_OFF;
				AOU2 = 0;
			}
		}

	} else if (program_mode == 2) {

		if (digital_output_slaves[0].mode != SLAVE_MODE_2) {

			digital_output_slaves[0].mode = SLAVE_MODE_2;
			digital_output_slaves[0].mode_changed_flag = 1;

		} else {

			if (temp_counter >= 15) {
				temp_counter = 1;
			}

			DOU1PWM1 = temp_counter * 5;
			DOU1PWM2 = temp_counter * 5;
			DOU1PWM3 = temp_counter * 5;

			temp_counter++;
		}
	}
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
		if (counter >= 2)
			time_out = 1;
		counter++;
		HAL_Delay(1);
	}

	interrupt_flag = 0;

	return time_out;

/*
	uint8_t time_out = 0;

	HAL_TIM_Base_Start_IT(&tim3_handle);

	while (!interrupt_flag && !time_out_flag);

	if (time_out_flag) {
		time_out = 1;
	}

	HAL_TIM_Base_Stop_IT(&tim3_handle);

	time_out_flag = 0;
	interrupt_flag = 0;

	return time_out;
*/
}

void empty_output_tables()
{
	for(uint8_t i = 0; i < num_of_dig_out; i++) {
		digital_output_slaves[i].digital_pins_state = 0;
		for(uint8_t j = 0; j < 3; j++) {
			digital_output_slaves[i].pwm_duty_arr[j] = 0;
		}
	}

	for(uint8_t i = 0; i < num_of_an_out; i++) {
		for(uint8_t j = 0; j < 3; j++) {
			analog_output_slaves[i].analoge_pins_state[j] = 0;
		}
	}
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

void print_out_digital_output_table()
{
	for (uint8_t i = 0; i < num_of_dig_out; i++) {
		LCD_UsrLog("DOU[%d] adr: %d state: %d\n", i, digital_output_slaves[i].slave_address, digital_output_slaves[i].digital_pins_state);
	}
}

void print_out_analog_input_table()
{
	for (uint8_t i = 0; i < num_of_an_in; i++) {
		LCD_UsrLog("AIN[%d]:  ", i);
		for (uint8_t j = 0; j < 6; j++) {
			LCD_UsrLog("%d, ", analog_input_slaves[i].analoge_pins_state[j]);
		}
		LCD_UsrLog("\n");
	}
}

void print_out_analog_output_table()
{
	for (uint8_t i = 0; i < num_of_an_out; i++) {
		LCD_UsrLog("AOUT[%d]: ", i);
		for (uint8_t j = 0; j < 6; j++) {
			LCD_UsrLog("%d, ", analog_output_slaves[i].analoge_pins_state[j]);
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
		digital_input_slaves[i].mode = SLAVE_MODE_1;
		digital_input_slaves[i].mode_changed_flag = 0;

		digital_output_slaves[i].slave_address = 0;
		digital_output_slaves[i].digital_pins_state = 0;
		digital_output_slaves[i].slave_status = 0;
		digital_output_slaves[i].mode = SLAVE_MODE_1;
		digital_output_slaves[i].mode_changed_flag = 0;

		for (uint8_t j = 0; j < 3; j++) {
			digital_output_slaves[i].pwm_duty_arr[j] = 0;
		}

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
