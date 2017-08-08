/* Includes ------------------------------------------------------------------*/
#include "master_loop_control.h"
#include "modbus.h"
#include "cmsis_os.h"
#include "main.h"
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

/* Private variables ------------------------------------------------------- */

frame_t frame;
uint8_t message[5];

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
	frame.crc = 65535;

	for (uint8_t i = 1; i < 5; i++) {
		frame.address = digital_input_slaves_address[i];
		frame.command = SCAN_SLAVE;
		frame.data_length = 0;

	}

}

uint8_t create_message()
{
	uint8_t message[4 + frame.data_length];

	message[0] = frame.address;
	message[1] = frame.command;

	for (uint8_t i = 2; i < 2 + frame.data_length; i++) {
		message[i] = frame.data[i - 2];
	}

	message[2 + frame.data_length] = frame.crc;
	message[3 + frame.data_length] = frame.crc >> 8;

	return 4 + frame.data_length;
}






/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
