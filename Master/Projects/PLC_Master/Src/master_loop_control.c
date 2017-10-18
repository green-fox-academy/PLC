/* Includes -----------p-------------------------------------------------------*/
#include "master_loop_control.h"
#include "uart.h"
#include "cmsis_os.h"
#include "main.h"
#include "lcd_log.h"
#include "plc_user_code.h"
#include "timer.h"
#include "gui.h"

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
uint8_t temp_counter = 0;
uint8_t pwm_duty_counter = 0;
uint8_t light_duty_counter = 0;
uint8_t proc_duty_counter = 0;
uint8_t proc_counter = 1;
uint8_t temp_pwm = 0;
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

// Check system functions
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

// Gui functions
void display_status();

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
	turn_off_slaves();
	osDelay(1000);

	master_loop_control_init();
	LCD_UsrLog("Control - Init done.\n");

	turn_on_slaves();
	LCD_UsrLog("Control - Slaves turned on.\n");

	osDelay(4000);

	scan_system_slaves();

	print_out_available_slaves();

	osDelay(2000);

	starting_screen();

	while (1) {

		load_input_tables();

		check_mode_select();

		execute_program();

		update_outputs();

		slaves_check();

		display_status();

		osDelay(10);

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
					gui_status.message_flag = 1;
					sprintf(gui_status.message,"Slave[%d] Err!", digital_input_slaves[i].slave_address);
				}
			}
		}

		for (uint8_t i = 0; i < num_of_dig_out; i++) {
			if (digital_output_slaves[i].slave_status == 4) {
				if (scan_slave_3_times(digital_output_slaves[i].slave_address)) {
					turn_off_slaves();
					gui_status.message_flag = 1;
					sprintf(gui_status.message,"Slave[%d] Err!", digital_output_slaves[i].slave_address);
				}
			}
		}

		for (uint8_t i = 0; i < num_of_an_in; i++) {
			if (analog_input_slaves[i].slave_status == 4) {
				if (scan_slave_3_times(analog_input_slaves[i].slave_address)) {
					turn_off_slaves();
					gui_status.message_flag = 1;
					sprintf(gui_status.message,"Slave[%d] Err!", analog_input_slaves[i].slave_address);
				}
			}
		}

		for (uint8_t i = 0; i < num_of_an_out; i++) {
			if (analog_output_slaves[i].slave_status == 4) {
				if (scan_slave_3_times(analog_output_slaves[i].slave_address)) {
					turn_off_slaves();
					gui_status.message_flag = 1;
					sprintf(gui_status.message,"Slave[%d] Err!", analog_output_slaves[i].slave_address);
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
			program_mode = 0;
		}

	} else if (D2_pinstate && !D3_pinstate) {
		if (program_mode != 1) {
			empty_output_tables();
			program_mode = 1;
		}

	} else if (D3_pinstate && !D2_pinstate) {
		if (program_mode != 2) {
			empty_output_tables();
			program_mode = 2;
		}
	}
}

void execute_program()
{
	int value = 50;
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
			if (DIN7) DOU7_ON; else DOU7_OFF;
			if (DIN8) DOU8_ON; else DOU8_OFF;

			// Direct connection between Potmeter 1,2 and Analog out 1 2
			AOU1 = AIN1;
			AOU2 = AIN2;
		}

	} else if (program_mode == 1) {

			if (!DIN1 && !DIN2 && !DIN3 && !DIN4 && !DIN5 && !DIN6 && !DIN7 && !DIN8) {
				DOU1_OFF;
				DOU2_OFF;
				DOU3_OFF;
				DOU4_OFF;
				DOU5_OFF;
				DOU6_OFF;
				DOU7_OFF;
				DOU8_OFF;

				temp_counter = 0;
			}

			if (DIN1 && !DIN2 && !DIN3 && !DIN4 && !DIN5 && !DIN6 && !DIN7 && !DIN8) {
				if (digital_output_slaves[0].mode != SLAVE_MODE_2) {

					digital_output_slaves[0].mode = SLAVE_MODE_2;
					digital_output_slaves[0].mode_changed_flag = 1;

				} else {
				// Set PWM with AIN 1 - 3
				DOU1PWM1 = AIN1 / 40.6; // RED
				DOU1PWM2 = AIN2 / 40.6;	// GREEN
				DOU1PWM3 = AIN3 / 40.6;	// BLUE
				}
			}

			if (!DIN1 && !DIN2 && !DIN3 && !DIN4 && DIN5 && !DIN6 && !DIN7 && !DIN8) {
				if (digital_output_slaves[0].mode != SLAVE_MODE_1) {

					digital_output_slaves[0].mode = SLAVE_MODE_1;
					digital_output_slaves[0].mode_changed_flag = 1;

					temp_counter = 0;

				} else {
					if ( temp_counter == 0) {
						if (pwm_duty_counter < value) {
							DOU1_ON;
							pwm_duty_counter++;
						} else {
							DOU8_OFF;
							temp_counter ++;
							pwm_duty_counter = 0;
						}

					}else if ( temp_counter == 1) {
						if (pwm_duty_counter < value) {
							DOU2_ON;
							pwm_duty_counter++;
						} else {
							DOU1_OFF;
							temp_counter ++;
							pwm_duty_counter = 0;
						}
					}else if ( temp_counter == 2) {
						if (pwm_duty_counter < value) {
							DOU3_ON;
							pwm_duty_counter++;
						} else {
							DOU2_OFF;
							temp_counter ++;
							pwm_duty_counter = 0;
						}
					}else if ( temp_counter == 3) {
						if (pwm_duty_counter < value) {
							DOU4_ON;
							pwm_duty_counter++;
						} else {
							DOU3_OFF;
							temp_counter ++;
							pwm_duty_counter = 0;
						}
					}else if ( temp_counter == 4) {
						if (pwm_duty_counter < value) {
							DOU5_ON;
							pwm_duty_counter++;
						} else {
							DOU4_OFF;
							temp_counter ++;
							pwm_duty_counter = 0;
						}
					}else if ( temp_counter == 5) {
						if (pwm_duty_counter < value) {
							DOU6_ON;
							pwm_duty_counter++;
						} else {
							DOU5_OFF;
							temp_counter ++;
							pwm_duty_counter = 0;
						}
					}else if ( temp_counter == 6) {
						if (pwm_duty_counter < value) {
							DOU7_ON;
							pwm_duty_counter++;
						} else {
							DOU6_OFF;
							temp_counter ++;
							pwm_duty_counter = 0;
						}
					}else if ( temp_counter == 7) {
						if (pwm_duty_counter < value) {
							DOU8_ON;
							pwm_duty_counter++;
						} else {
							DOU7_OFF;
							DOU8_OFF;
							temp_counter = 0;
							pwm_duty_counter = 0;
						}
					}
				}
			}


			if (DIN6 && !DIN1 && !DIN2 && !DIN3 && !DIN5 && !DIN8 && !DIN7) {
				if (digital_output_slaves[0].mode != SLAVE_MODE_1) {

					digital_output_slaves[0].mode = SLAVE_MODE_1;
					digital_output_slaves[0].mode_changed_flag = 1;

					temp_counter = 0;

				} else {
					if ( temp_counter == 0) {
						if (light_duty_counter < 102 - (AIN6 / 40.95)) {
							DOU1_ON;
							light_duty_counter++;
						} else {
							DOU1_OFF;
							temp_counter ++;
							light_duty_counter = 0;
						}

					}else if ( temp_counter == 1) {
						if (light_duty_counter < 102 - (AIN6 / 40.95)) {
							DOU2_ON;
							light_duty_counter++;
						} else {
							DOU2_OFF;
							temp_counter ++;
							light_duty_counter = 0;
						}
					}else if ( temp_counter == 2) {
						if (light_duty_counter < 102 - (AIN6 / 40.95)) {
							DOU3_ON;
							light_duty_counter++;
						} else {
							DOU3_OFF;
							temp_counter ++;
							light_duty_counter = 0;
						}
					}else if ( temp_counter == 3) {
						if (light_duty_counter < 102 - (AIN6 / 40.95)) {
							DOU4_ON;
							light_duty_counter++;
						} else {
							DOU4_OFF;
							temp_counter ++;
							light_duty_counter = 0;
						}
					}else if ( temp_counter == 4) {
						if (light_duty_counter < 102 - (AIN6 / 40.95)) {
							DOU5_ON;
							light_duty_counter++;
						} else {
							DOU5_OFF;
							temp_counter ++;
							light_duty_counter = 0;
						}
					}else if ( temp_counter == 5) {
						if (light_duty_counter < 102 - (AIN6 / 40.95)) {
							DOU6_ON;
							light_duty_counter++;
						} else {
							DOU6_OFF;
							temp_counter ++;
							light_duty_counter = 0;
						}
					}else if ( temp_counter == 6) {
						if (light_duty_counter < 102 - (AIN6 / 40.95)) {
							DOU7_ON;
							light_duty_counter++;
						} else {
							DOU7_OFF;
							temp_counter ++;
							light_duty_counter = 0;
						}
					}else if ( temp_counter == 7) {
						if (light_duty_counter < 102 - (AIN6 / 40.95)) {
							DOU8_ON;
							light_duty_counter++;
						} else {
							DOU8_OFF;
							temp_counter = 0;
							light_duty_counter = 0;
						}
					}
				}
			}


			if (DIN7 && !DIN1 && !DIN2 && !DIN3 && !DIN5 && !DIN6 && !DIN8) {
				if (digital_output_slaves[0].mode != SLAVE_MODE_1) {

					digital_output_slaves[0].mode = SLAVE_MODE_1;
					digital_output_slaves[0].mode_changed_flag = 1;

					temp_counter = 0;

				} else {
					if (AIN5 < 455) {
						DOU1_OFF;
						DOU2_OFF;
						DOU3_OFF;
						DOU4_OFF;
						DOU5_OFF;
						DOU6_OFF;
						DOU7_OFF;
						DOU8_OFF;
					}
					if (AIN5 > 455 && AIN5 < 910) {
						DOU1_ON;
						DOU2_OFF;
						DOU3_OFF;
						DOU4_OFF;
						DOU5_OFF;
						DOU6_OFF;
						DOU7_OFF;
						DOU8_OFF;
					}
					if (AIN5 > 910 && AIN5 < 1315) {
						DOU1_ON;
						DOU2_ON;
						DOU3_OFF;
						DOU4_OFF;
						DOU5_OFF;
						DOU6_OFF;
						DOU7_OFF;
						DOU8_OFF;
					}
					if (AIN5 > 1315 && AIN5 < 1820) {
						DOU1_ON;
						DOU2_ON;
						DOU3_ON;
						DOU4_OFF;
						DOU5_OFF;
						DOU6_OFF;
						DOU7_OFF;
						DOU8_OFF;
					}
					if (AIN5 > 1820 && AIN5 < 2275) {
						DOU1_ON;
						DOU2_ON;
						DOU3_ON;
						DOU4_ON;
						DOU5_OFF;
						DOU6_OFF;
						DOU7_OFF;
						DOU8_OFF;
					}
					if (AIN5 > 2275 && AIN5 < 2730) {
						DOU1_ON;
						DOU2_ON;
						DOU3_ON;
						DOU4_ON;
						DOU5_ON;
						DOU6_OFF;
						DOU7_OFF;
						DOU8_OFF;
					}
					if (AIN5 > 2730 && AIN5 < 3185) {
						DOU1_ON;
						DOU2_ON;
						DOU3_ON;
						DOU4_ON;
						DOU5_ON;
						DOU6_ON;
						DOU7_OFF;
						DOU8_OFF;
					}
					if (AIN5 > 3185 && AIN5 < 3640) {
						DOU1_ON;
						DOU2_ON;
						DOU3_ON;
						DOU4_ON;
						DOU5_ON;
						DOU6_ON;
						DOU7_ON;
						DOU8_OFF;
					}
					if (AIN5 > 3640 && AIN5 < 4095) {
						DOU1_ON;
						DOU2_ON;
						DOU3_ON;
						DOU4_ON;
						DOU5_ON;
						DOU6_ON;
						DOU7_ON;
						DOU8_ON;
					}
				}
			}


			if (!DIN1 && !DIN2 && !DIN3 && !DIN4 && !DIN5 && !DIN6 && !DIN7 && DIN8) {
				if (digital_output_slaves[0].mode != SLAVE_MODE_1) {

					digital_output_slaves[0].mode = SLAVE_MODE_1;
					digital_output_slaves[0].mode_changed_flag = 1;

					temp_counter = 0;

				} else {
					if ( temp_counter == 0) {
						if ((proc_counter % 2 == 0) && proc_counter <= 8) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_ON;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else if ((proc_counter % 2 != 0) && proc_counter <= 8) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_OFF;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else {
							DOU1_OFF;
							proc_counter = 0;
							temp_counter ++;
						}
					}else if ( temp_counter == 1) {
						if (proc_counter  == 0) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_ON;
								DOU2_ON;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else if (proc_counter == 1) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_OFF;
								DOU2_OFF;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else {
							DOU1_OFF;
							DOU2_OFF;
							proc_counter = 0;
							temp_counter ++;
						}

					}else if ( temp_counter == 2) {
						if (proc_counter  == 0) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_ON;
								DOU3_ON;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else if (proc_counter == 1) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_OFF;
								DOU3_OFF;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else {
							DOU1_OFF;
							DOU3_OFF;
							proc_counter = 0;
							temp_counter ++;
						}

					}else if ( temp_counter == 3) {
						if (proc_counter  == 0) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_ON;
								DOU4_ON;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else if (proc_counter == 1) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_OFF;
								DOU4_OFF;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else {
							DOU1_OFF;
							DOU4_OFF;
							proc_counter = 0;
							temp_counter ++;
						}

					}else if ( temp_counter == 4) {
						if (proc_counter  == 0) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_ON;
								DOU5_ON;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else if (proc_counter == 1) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_OFF;
								DOU5_OFF;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else {
							DOU1_OFF;
							DOU5_OFF;
							proc_counter = 0;
							temp_counter ++;
						}

					}else if ( temp_counter == 5) {
						if (proc_counter  == 0) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_ON;
								DOU2_ON;
								DOU5_ON;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else if (proc_counter == 1) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_OFF;
								DOU2_OFF;
								DOU5_OFF;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else {
							DOU1_OFF;
							DOU2_OFF;
							DOU5_OFF;
							proc_counter = 0;
							temp_counter ++;
						}

					}else if ( temp_counter == 6) {
						if (proc_counter  == 0) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_ON;
								DOU3_ON;
								DOU5_ON;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else if (proc_counter == 1) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_OFF;
								DOU3_OFF;
								DOU5_OFF;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else {
							DOU1_OFF;
							DOU3_OFF;
							DOU5_OFF;
							proc_counter = 0;
							temp_counter ++;
						}

					}else if ( temp_counter == 7) {
						if (proc_counter  == 0) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_ON;
								DOU4_ON;
								DOU5_ON;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else if (proc_counter == 1) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_OFF;
								DOU4_OFF;
								DOU5_OFF;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else {
							DOU1_OFF;
							DOU4_OFF;
							DOU5_OFF;
							proc_counter = 0;
							temp_counter ++;
						}

					}else if ( temp_counter == 8) {
						if (proc_counter  == 0) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_ON;
								DOU5_ON;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else if (proc_counter == 1) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_OFF;
								DOU5_OFF;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else {
							DOU1_OFF;
							DOU5_OFF;
							proc_counter = 0;
							temp_counter ++;
						}

					}else if ( temp_counter == 9) {
						if (proc_counter  == 0) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_ON;
								DOU2_ON;
								DOU6_ON;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else if (proc_counter == 1) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_OFF;
								DOU2_OFF;
								DOU6_OFF;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else {
							DOU1_OFF;
							DOU2_OFF;
							DOU6_OFF;
							proc_counter = 0;
							temp_counter ++;
						}

					}else if ( temp_counter == 10) {
						if (proc_counter  == 0) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_ON;
								DOU3_ON;
								DOU6_ON;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else if (proc_counter == 1) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_OFF;
								DOU3_OFF;
								DOU6_OFF;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else {
							DOU1_OFF;
							DOU3_OFF;
							DOU6_OFF;
							proc_counter = 0;
							temp_counter ++;
						}

					}else if ( temp_counter == 11) {
						if (proc_counter  == 0) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_ON;
								DOU4_ON;
								DOU6_ON;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else if (proc_counter == 1) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_OFF;
								DOU4_OFF;
								DOU6_OFF;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else {
							DOU1_OFF;
							DOU4_OFF;
							DOU6_OFF;
							proc_counter = 0;
							temp_counter ++;
						}

					}else if ( temp_counter == 12) {
						if (proc_counter  == 0) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_ON;
								DOU5_ON;
								DOU7_ON;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else if (proc_counter == 1) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_OFF;
								DOU5_OFF;
								DOU7_OFF;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else {
							DOU1_OFF;
							DOU5_OFF;
							DOU7_OFF;
							proc_counter = 0;
							temp_counter ++;
						}

					}else if ( temp_counter == 13) {
						if (proc_counter  == 0) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_ON;
								DOU2_ON;
								DOU5_ON;
								DOU7_ON;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else if (proc_counter == 1) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_OFF;
								DOU2_OFF;
								DOU5_OFF;
								DOU7_OFF;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else {
							DOU1_OFF;
							DOU2_OFF;
							DOU5_OFF;
							DOU7_OFF;
							proc_counter = 0;
							temp_counter ++;
						}

					}else if ( temp_counter == 14) {
						if (proc_counter  == 0) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_ON;
								DOU3_ON;
								DOU5_ON;
								DOU8_ON;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else if (proc_counter == 1) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_OFF;
								DOU3_OFF;
								DOU5_OFF;
								DOU8_OFF;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else {
							DOU1_OFF;
							DOU3_OFF;
							DOU5_OFF;
							DOU8_OFF;
							proc_counter = 0;
							temp_counter ++;
						}

					}else if ( temp_counter == 15) {
						if (proc_counter  == 0) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_ON;
								DOU4_ON;
								DOU5_ON;
								DOU8_ON;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else if (proc_counter == 1) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_OFF;
								DOU4_OFF;
								DOU5_OFF;
								DOU8_OFF;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else {
							DOU1_OFF;
							DOU4_OFF;
							DOU5_OFF;
							DOU8_OFF;
							proc_counter = 0;
							temp_counter ++;
						}

					}else if ( temp_counter == 16) {
						if (proc_counter  == 0) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_ON;
								DOU5_ON;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else if (proc_counter == 1) {
							if (proc_duty_counter < 105 - (AIN6 / 40.95)) {
								DOU1_OFF;
								DOU5_OFF;
								proc_duty_counter++;
							} else {
								proc_duty_counter = 0;
								proc_counter++;
							}
						} else {
							DOU1_OFF;
							DOU5_OFF;
							proc_counter = 0;
							temp_counter = 5;
						}
					}
				}
			}



	} else if (program_mode == 2) {

		if (digital_output_slaves[0].mode != SLAVE_MODE_2) {

			digital_output_slaves[0].mode = SLAVE_MODE_2;
			digital_output_slaves[0].mode_changed_flag = 1;

		} else {

			if ( temp_counter == 0) {
				if (pwm_duty_counter < 100) {

					DOU1PWM1 = 100; 				// RED
					DOU1PWM2 = pwm_duty_counter;	// GREEN
					DOU1PWM3 = 0;					// BLUE

					pwm_duty_counter++;
				} else {
					temp_counter ++;
					pwm_duty_counter = 0;
				}

			} else if (temp_counter == 1) {
				if (pwm_duty_counter < 100) {

					DOU1PWM1 = 100 - pwm_duty_counter; 	// RED
					DOU1PWM2 = 100;						// GREEN
					DOU1PWM3 = 0;						// BLUE

					pwm_duty_counter++;
				} else {
					temp_counter ++;
					pwm_duty_counter = 0;
				}

			}  else if (temp_counter == 2) {
				if (pwm_duty_counter < 100) {

					DOU1PWM1 = 0; 					// RED
					DOU1PWM2 = 100;					// GREEN
					DOU1PWM3 = pwm_duty_counter;	// BLUE

					pwm_duty_counter++;
				} else {
					temp_counter ++;
					pwm_duty_counter = 0;
				}

			}  else if (temp_counter == 3) {
				if (pwm_duty_counter < 100) {

					DOU1PWM1 = 0; 						// RED
					DOU1PWM2 = 100 - pwm_duty_counter;	// GREEN
					DOU1PWM3 = 100;						// BLUE

					pwm_duty_counter++;
				} else {
					temp_counter ++;
					pwm_duty_counter = 0;
				}

			}  else if (temp_counter == 4) {
				if (pwm_duty_counter < 100) {

					DOU1PWM1 = pwm_duty_counter; 	// RED
					DOU1PWM2 = 0;					// GREEN
					DOU1PWM3 = 100;					// BLUE

					pwm_duty_counter++;
				} else {
					temp_counter ++;
					pwm_duty_counter = 0;
				}

			} else {
				if (pwm_duty_counter < 100) {

					DOU1PWM1 = 100; 					// RED
					DOU1PWM2 = 0;						// GREEN
					DOU1PWM3 = 100 - pwm_duty_counter;	// BLUE

					pwm_duty_counter++;
				} else {
					temp_counter = 0;
					pwm_duty_counter = 0;
				}
			}
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

void display_status()
{
	gui_status.din_state  = digital_input_slaves[0].digital_pins_state;
	gui_status.dout_state = digital_output_slaves[0].digital_pins_state;

	for (uint8_t i = 0; i < 6; i++) {
		gui_status.ain_state[i] = analog_input_slaves[0].analoge_pins_state[i];
	}

	for (uint8_t i = 0; i < 2; i++) {
		gui_status.aou_state[i] = analog_output_slaves[0].analoge_pins_state[i];
	}

	for (uint8_t i = 0; i < 3; i++) {
		gui_status.pwm[i] = digital_output_slaves[0].pwm_duty_arr[i];
	}

	gui_status.mode = program_mode;

	gui_display_status();
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
