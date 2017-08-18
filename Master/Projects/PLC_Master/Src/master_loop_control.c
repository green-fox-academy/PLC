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

/* Theese arrays contains the addresses of the slaves */
uint8_t digital_input_slaves_address[]  = {1,2,3,4};
uint8_t digital_output_slaves_address[] = {5,6,7,8};
uint8_t analog_input_slaves_address[]   = {9,10,11,12};
uint8_t analog_output_slaves_address[]  = {13,14,15,16};

/* Private functions ------------------------------------------------------- */
uint16_t generate_crc();
void wait_function();
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

	scan_system_slaves();

	while (1) {

		// system_check();
		// slaves_check();
		load_input_tables();
		execute_program();
		upadte_outputs ();

		osDelay(1);

	}
}

void test_uart_sender()
{
	msg_command.address = 10;
	msg_command.command = SCAN_SLAVE;
	msg_command.crc = 3333;

	TX_buffer[0] = msg_command.address;
	TX_buffer[1] = msg_command.command;
	TX_buffer[2] = msg_command.crc;
	TX_buffer[3] = msg_command.crc >> 8;

	while (1) {

		UART_send(TX_buffer);
		wait_function();

		msg_command.address = RX_buffer[0];
		msg_command.command = RX_buffer[1];
		msg_command.crc = RX_buffer[2] + (RX_buffer[3] << 8);

		printf("adr: %d, com: %d, crc: %d\n", msg_command.address, msg_command.command, msg_command.crc);

		HAL_Delay(500);

	}

}

void test_uart_receiver()
{
	while (interrupt_flag){
		for(uint8_t i = 0; i < 16; i++) {
			LCD_UsrLog("%d , ", RX_buffer[i]);
		}
		LCD_UsrLog("\n");
	}

	HAL_Delay(200);
}




void scan_system_slaves()
{

}

void load_input_tables()
{
	load_digital_input_table();
	load_analog_input_table();
}


void load_digital_input_table()
{

}

void load_analog_input_table()
{

}

void update_outputs()
{

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

	if(ok) {
		LCD_UsrLog("Time out\n");
		for (uint8_t i = 0; i < RXBUFFERSIZE; i++) {
			RX_buffer[i] = 0;
		}
	}

	interrupt_flag = 0;
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
