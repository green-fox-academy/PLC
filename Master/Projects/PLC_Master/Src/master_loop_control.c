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

void tx_rx_digital_slave(digital_rx_tx_t message)
{

}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
